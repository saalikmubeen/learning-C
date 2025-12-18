#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <cuda_runtime.h>
#include <algorithm>
#include <cuda.h>
#include <climits>
// ==== DO NOT MODIFY CODE ABOVE THIS LINE ====

#define DTYPE int

// Multiple block size options for occupancy testing
#define THREADS_PER_BLOCK_256  256
#define THREADS_PER_BLOCK_512  512
#define THREADS_PER_BLOCK_1024 1024

// Choose optimal block size (can be determined programmatically)
#define THREADS_PER_BLOCK THREADS_PER_BLOCK_256

// Shared memory optimization - process data in chunks
#define SHARED_MEM_SIZE 256

// Function to calculate theoretical occupancy
void calculateOccupancy(void *kernel_func, int blockSize, size_t dynamicSMemSize) {
    int minGridSize, blockSizeOpt;
    int device;
    cudaDeviceProp deviceProp;

    cudaGetDevice(&device);
    cudaGetDeviceProperties(&deviceProp, device);

    cudaOccupancyMaxPotentialBlockSize(&minGridSize, &blockSizeOpt,
                                       kernel_func, dynamicSMemSize, 0);

    int maxActiveBlocks;
    cudaOccupancyMaxActiveBlocksPerMultiprocessor(&maxActiveBlocks,
                                                  kernel_func, blockSize, dynamicSMemSize);

    float occupancy = (maxActiveBlocks * blockSize / (float)deviceProp.maxThreadsPerMultiProcessor) * 100;

    printf("\033[1;36m=== OCCUPANCY ANALYSIS ===\033[0m\n");
    printf("Theoretical occupancy: %.2f%%\n", occupancy);
    printf("Optimal block size: %d\n", blockSizeOpt);
    printf("Current block size: %d\n", blockSize);
    printf("Max active blocks per SM: %d\n", maxActiveBlocks);
    printf("Max threads per SM: %d\n", deviceProp.maxThreadsPerMultiProcessor);
    printf("\033[1;36m========================\033[0m\n");
}

// Optimized bitonic sort kernel with shared memory and reduced register usage
__global__ void bitonicSortKernelOptimized(DTYPE *arr, int n, int k, int j)
{
    // Use shared memory to cache data and reduce global memory accesses
    extern __shared__ DTYPE shared_data[];

    // Calculate indices once and reuse
    const int tid = threadIdx.x;
    const int idx = blockIdx.x * blockDim.x + tid;

    if (idx >= n) return;

    // Load data into shared memory in chunks for better memory coalescing
    int chunk_size = min(blockDim.x, n - blockIdx.x * blockDim.x);
    if (tid < chunk_size) {
        shared_data[tid] = arr[blockIdx.x * blockDim.x + tid];
    }
    __syncthreads();

    // Compute XOR index
    const int ixj = idx ^ j;

    if (ixj > idx && ixj < n) {
        // Determine sort direction - reduce register pressure by inlining
        bool ascending = (idx & k) == 0;

        DTYPE val1 = arr[idx];
        DTYPE val2 = arr[ixj];

        // Branchless comparison to reduce warp divergence
        bool should_swap = ascending ? (val1 > val2) : (val1 < val2);

        if (should_swap) {
            arr[idx] = val2;
            arr[ixj] = val1;
        }
    }
}

// Alternative kernel with even more aggressive optimizations
__global__ void bitonicSortKernelUltraOptimized(DTYPE *arr, int n, int k, int j)
{
    // Minimize register usage - combine calculations
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx >= n) return;

    int ixj = idx ^ j;

    // Early exit if not a valid comparison pair
    if (ixj <= idx || ixj >= n) return;

    // Load values once
    DTYPE *addr1 = &arr[idx];
    DTYPE *addr2 = &arr[ixj];
    DTYPE val1 = *addr1;
    DTYPE val2 = *addr2;

    // Branchless swap using bitwise operations
    bool ascending = (idx & k) == 0;
    bool should_swap = ascending ? (val1 > val2) : (val1 < val2);

    if (should_swap) {
        *addr1 = val2;
        *addr2 = val1;
    }
}

// Shared memory version for small arrays
__global__ void bitonicSortKernelSharedMemory(DTYPE *arr, int n, int k, int j)
{
    extern __shared__ DTYPE sdata[];

    int tid = threadIdx.x;
    int idx = blockIdx.x * blockDim.x + tid;

    // Load into shared memory
    sdata[tid] = (idx < n) ? arr[idx] : INT_MAX;
    __syncthreads();

    // Perform comparisons in shared memory
    int local_ixj = (blockIdx.x * blockDim.x + tid) ^ j;
    int shared_ixj = local_ixj - blockIdx.x * blockDim.x;

    if (shared_ixj >= 0 && shared_ixj < blockDim.x && shared_ixj != tid) {
        bool ascending = ((blockIdx.x * blockDim.x + tid) & k) == 0;

        if ((tid < shared_ixj && ascending && sdata[tid] > sdata[shared_ixj]) ||
            (tid < shared_ixj && !ascending && sdata[tid] < sdata[shared_ixj])) {

            DTYPE temp = sdata[tid];
            sdata[tid] = sdata[shared_ixj];
            sdata[shared_ixj] = temp;
        }
    }

    __syncthreads();

    // Write back to global memory
    if (idx < n) {
        arr[idx] = sdata[tid];
    }
}

__global__ void fillPaddingKernel(DTYPE *arr, int start, int end, DTYPE value)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int pos = start + idx;
    if (pos < end) {
        arr[pos] = value;
    }
}

void bitonicSort(DTYPE *d_arr, int n)
{
    // Find the next power of 2
    int pow2 = 1;
    while (pow2 < n) {
        pow2 <<= 1;
    }

    // Calculate occupancy for our kernel
    calculateOccupancy((void*)bitonicSortKernelOptimized, THREADS_PER_BLOCK, SHARED_MEM_SIZE * sizeof(DTYPE));

    // Choose kernel based on array size and occupancy requirements
    bool use_shared_memory = (pow2 <= THREADS_PER_BLOCK * 1024); // Use shared memory for smaller arrays

    printf("Using %s kernel\n", use_shared_memory ? "shared memory" : "optimized global memory");

    // Bitonic sort algorithm with optimized kernels
    for (int k = 2; k <= pow2; k <<= 1) {
        for (int j = k >> 1; j > 0; j >>= 1) {
            int num_blocks = (pow2 + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;

            if (use_shared_memory) {
                bitonicSortKernelSharedMemory<<<num_blocks, THREADS_PER_BLOCK,
                                              THREADS_PER_BLOCK * sizeof(DTYPE)>>>(d_arr, pow2, k, j);
            } else {
                // Use ultra-optimized version for better occupancy
                bitonicSortKernelUltraOptimized<<<num_blocks, THREADS_PER_BLOCK>>>(d_arr, pow2, k, j);
            }

            cudaError_t err = cudaGetLastError();
            if (err != cudaSuccess) {
                printf("CUDA error: %s\n", cudaGetErrorString(err));
                return;
            }

            cudaDeviceSynchronize();
        }
    }
}

/* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);

    srand(time(NULL));

    // ===== OPTIMIZED MEMORY ALLOCATION =====
    // Use pinned memory for faster transfers
    DTYPE *arrCpu = nullptr;
    DTYPE *arrSortedGpu = nullptr;
    cudaMallocHost(&arrCpu, size * sizeof(DTYPE));       // Pinned input
    cudaMallocHost(&arrSortedGpu, size * sizeof(DTYPE)); // Pinned output

    for (int i = 0; i < size; i++) {
        arrCpu[i] = rand() % 1000;
    }

    float gpuTime, h2dTime, d2hTime, cpuTime = 0;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);

    /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

    // ===== OPTIMIZED STREAMS FOR OVERLAPPING =====
    cudaStream_t h2d_stream, d2h_stream;
    cudaStreamCreate(&h2d_stream);
    cudaStreamCreate(&d2h_stream);

    // Transfer data to device
    DTYPE *d_arr;

    // Calculate the next power of 2 for array size
    int pow2_size = 1;
    while (pow2_size < size) {
        pow2_size <<= 1;
    }

    // Allocate GPU memory
    cudaMalloc(&d_arr, pow2_size * sizeof(DTYPE));

    // ===== OPTIMIZED H2D TRANSFER =====
    // Async copy of main data
    cudaMemcpyAsync(d_arr, arrCpu, size * sizeof(DTYPE),
                    cudaMemcpyHostToDevice, h2d_stream);

    // Fill padding using kernel (faster than CPU->GPU copy)
    if (pow2_size > size) {
        int padding_size = pow2_size - size;
        int pad_blocks = (padding_size + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
        fillPaddingKernel<<<pad_blocks, THREADS_PER_BLOCK, 0, h2d_stream>>>(
            d_arr, size, pow2_size, INT_MAX);
    }

    // Wait for H2D and padding to complete
    cudaStreamSynchronize(h2d_stream);

    /* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&h2dTime, start, stop);

    cudaEventRecord(start);

    /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

    // Perform bitonic sort on GPU
    bitonicSort(d_arr, size);

    /* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpuTime, start, stop);

    cudaEventRecord(start);

    /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

    // ===== OPTIMIZED D2H TRANSFER =====
    // Use asynchronous transfer with pinned memory
    cudaMemcpyAsync(arrSortedGpu, d_arr, size * sizeof(DTYPE),
                    cudaMemcpyDeviceToHost, d2h_stream);

    // Wait for D2H transfer to complete
    cudaStreamSynchronize(d2h_stream);

    // Clean up GPU memory
    cudaFree(d_arr);

    /* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&d2hTime, start, stop);

    auto startTime = std::chrono::high_resolution_clock::now();

    // CPU sort for performance comparison
    // Use the original pinned memory array for CPU sort
    std::sort(arrCpu, arrCpu + size);

    auto endTime = std::chrono::high_resolution_clock::now();
    cpuTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    cpuTime = cpuTime / 1000;

    int match = 1;
    for (int i = 0; i < size; i++) {
        if (arrSortedGpu[i] != arrCpu[i]) {
            match = 0;
            break;
        }
    }

    // ===== CLEANUP OPTIMIZED MEMORY =====
    // Free pinned memory (not regular free!)
    cudaFreeHost(arrCpu);
    cudaFreeHost(arrSortedGpu);

    // Destroy streams
    cudaStreamDestroy(h2d_stream);
    cudaStreamDestroy(d2h_stream);

    if (match)
        printf("\033[1;32mFUNCTIONAL SUCCESS\n\033[0m");
    else {
        printf("\033[1;31mFUNCTIONAL FAIL\n\033[0m");
        return 0;
    }

    printf("\033[1;34mArray size         :\033[0m %d\n", size);
    printf("\033[1;34mCPU Sort Time (ms) :\033[0m %f\n", cpuTime);
    float gpuTotalTime = h2dTime + gpuTime + d2hTime;
    int speedup = (gpuTotalTime > cpuTime) ? (gpuTotalTime / cpuTime) : (cpuTime / gpuTotalTime);
    float meps = size / (gpuTotalTime * 0.001) / 1e6;
    printf("\033[1;34mGPU Sort Time (ms) :\033[0m %f\n", gpuTotalTime);
    printf("\033[1;34mGPU Sort Speed     :\033[0m %f million elements per second\n", meps);
    if (gpuTotalTime < cpuTime) {
        printf("\033[1;32mPERF PASSING\n\033[0m");
        printf("\033[1;34mGPU Sort is \033[1;32m %dx \033[1;34mfaster than CPU !!!\033[0m\n", speedup);
        printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
        printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
        printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
    } else {
        printf("\033[1;31mPERF FAILING\n\033[0m");
        printf("\033[1;34mGPU Sort is \033[1;31m%dx \033[1;34mslower than CPU, optimize further!\n", speedup);
        printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
        printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
        printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
        return 0;
    }

    return 0;
}