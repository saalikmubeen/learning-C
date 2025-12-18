// kernel.cu
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <cuda_runtime.h>
#include <algorithm>
#include <climits>

// ==== DO NOT MODIFY CODE ABOVE THIS LINE ====

#define DTYPE int

// Tunable parameters
// SHARED_SIZE is the largest bitonic subsequence length we'll fully sort inside shared memory.
// Keep it a power of two. Set to 1024 for portability; if you target H100 you may increase.
#define SHARED_SIZE 1024
// threads used by the global kernel (tunable)
#define THREADS_PER_BLOCK 256

// Simple CUDA error macro
#define CUDA_CHECK(call)                                                                    \
  do                                                                                        \
  {                                                                                         \
    cudaError_t e = (call);                                                                 \
    if (e != cudaSuccess)                                                                   \
    {                                                                                       \
      fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(e)); \
      exit(1);                                                                              \
    }                                                                                       \
  } while (0)

// Shared-memory bitonic kernel:
// Each block sorts one contiguous subsequence of length 'segLen' entirely inside shared memory.
// segLen must be <= blockDim.x and a power of two. We use one thread per element.
__global__ void bitonicSharedKernel(DTYPE *d_arr, int segLen, int totalN)
{
  extern __shared__ DTYPE s[]; // segLen * sizeof(DTYPE)
  int bid = blockIdx.x;
  int tid = threadIdx.x;
  int base = bid * segLen;
  int idx = base + tid;

  // load (with padding)
  if (tid < segLen)
  {
    if (idx < totalN)
      s[tid] = d_arr[idx];
    else
      s[tid] = INT_MAX;
  }
  __syncthreads();

  // bitonic network inside shared memory for length = segLen
  for (int k = 2; k <= segLen; k <<= 1)
  {
    for (int j = k >> 1; j > 0; j >>= 1)
    {
      int partner = tid ^ j;
      if (partner > tid && partner < segLen)
      {
        // Ascend/descend must be computed from the GLOBAL index, not local tid
        bool ascend = (((base + tid) & k) == 0);
        DTYPE a = s[tid];
        DTYPE b = s[partner];
        if ((a > b) == ascend)
        {
          s[tid] = b;
          s[partner] = a;
        }
      }
      __syncthreads();
    }
  }

  // write back
  if (idx < totalN && tid < segLen)
    d_arr[idx] = s[tid];
}

// Global compare-exchange kernel for (k, j) stage.
// Each thread handles index i and its partner p = i ^ j. Only threads with p > i perform swaps.
__global__ void bitonicGlobalKernel(DTYPE *d_arr, int N, int k, int j)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i >= N)
    return;
  int p = i ^ j;
  if (p <= i || p >= N)
    return;

  bool ascend = ((i & k) == 0);
  DTYPE a = d_arr[i];
  DTYPE b = d_arr[p];

  if ((a > b) == ascend)
  {
    // swap; only this thread writes both locations so races avoided because p > i and only one thread handles the pair
    d_arr[i] = b;
    d_arr[p] = a;
  }
}

/* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s <array_size>\n", argv[0]);
    return 1;
  }

  cudaError_t err = cudaDeviceReset();
  if (err != cudaSuccess)
  {
    fprintf(stderr, "Failed to reset CUDA device: %s\n", cudaGetErrorString(err));
    return -1;
  }

  int size = atoi(argv[1]);

  srand((unsigned)time(NULL));

  DTYPE *arrCpu = (DTYPE *)malloc(size * sizeof(DTYPE));

  for (int i = 0; i < size; i++)
  {
    arrCpu[i] = rand() % 1000000; // larger range to exercise sorting
  }

  float gpuTime = 0, h2dTime = 0, d2hTime = 0, cpuTime = 0;

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  cudaEventRecord(start);

  /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

  // arrCpu contains the input random array
  // arrSortedGpu should contain the sorted array copied from GPU to CPU
  DTYPE *arrSortedGpu = (DTYPE *)malloc(size * sizeof(DTYPE));

  // Transfer data (arr_cpu) to device
  DTYPE *d_arr = nullptr;
  CUDA_CHECK(cudaMalloc(&d_arr, size * sizeof(DTYPE)));
  CUDA_CHECK(cudaMemcpy(d_arr, arrCpu, size * sizeof(DTYPE), cudaMemcpyHostToDevice));

  /* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&h2dTime, start, stop);

  cudaEventRecord(start);

  /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

  // Perform bitonic sort on GPU
  // Bitonic requires power-of-two length; pad if necessary
  int N = size;
  if ((N & (N - 1)) != 0)
  {
    int p2 = 1;
    while (p2 < N)
      p2 <<= 1;
    DTYPE *d_padded;
    CUDA_CHECK(cudaMalloc(&d_padded, p2 * sizeof(DTYPE)));
    CUDA_CHECK(cudaMemcpy(d_padded, d_arr, N * sizeof(DTYPE), cudaMemcpyDeviceToDevice));
    // pad remainder with INT_MAX
    int pad = p2 - N;
    if (pad > 0)
    {
      DTYPE *tmp = (DTYPE *)malloc(pad * sizeof(DTYPE));
      for (int i = 0; i < pad; ++i)
        tmp[i] = INT_MAX;
      CUDA_CHECK(cudaMemcpy(d_padded + N, tmp, pad * sizeof(DTYPE), cudaMemcpyHostToDevice));
      free(tmp);
    }
    CUDA_CHECK(cudaFree(d_arr));
    d_arr = d_padded;
    N = p2;
  }

  // Hybrid strategy:
  // For k <= SHARED_SIZE, perform per-segment shared-memory sort with segLen = k.
  // For k > SHARED_SIZE, perform global kernels for each (k, j) pair.
  for (int k = 2; k <= N; k <<= 1)
  {
    if (k <= SHARED_SIZE)
    {
      int segments = N / k;
      int threads = k; // one thread per element in segment
      if (threads > 1024)
      {
        // if seg too large for 1024 threads, have each thread handle multiple elements.
        // For portability we keep SHARED_SIZE <= 1024 in this implementation.
        fprintf(stderr, "SHARED_SIZE too large for max threads per block. Reduce SHARED_SIZE.\n");
        return 1;
      }
      int blocks = segments;
      size_t shmBytes = k * sizeof(DTYPE);
      bitonicSharedKernel<<<blocks, threads, shmBytes>>>(d_arr, k, N);
      CUDA_CHECK(cudaDeviceSynchronize());
    }
    else
    {
      // global path: run inner j loops
      for (int j = k >> 1; j > 0; j >>= 1)
      {
        int threads = THREADS_PER_BLOCK;
        int blocks = (N + threads - 1) / threads;
        bitonicGlobalKernel<<<blocks, threads>>>(d_arr, N, k, j);
      }
      CUDA_CHECK(cudaDeviceSynchronize());
    }
  }

  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&gpuTime, start, stop);

  cudaEventRecord(start);

  /* ==== DO NOT MODIFY CODE ABOVE THIS LINE ==== */

  // Transfer sorted data back to host (only original 'size' elements)
  CUDA_CHECK(cudaMemcpy(arrSortedGpu, d_arr, size * sizeof(DTYPE), cudaMemcpyDeviceToHost));

  /* ==== DO NOT MODIFY CODE BELOW THIS LINE ==== */
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&d2hTime, start, stop);

  auto startTime = std::chrono::high_resolution_clock::now();

  // CPU sort for performance comparison
  std::sort(arrCpu, arrCpu + size);

  auto endTime = std::chrono::high_resolution_clock::now();
  cpuTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
  cpuTime = cpuTime / 1000;

  int match = 1;
  for (int i = 0; i < size; i++)
  {
    if (arrSortedGpu[i] != arrCpu[i])
    {
      match = 0;
      break;
    }
  }

  // Cleanup
  if (d_arr)
    CUDA_CHECK(cudaFree(d_arr));
  free(arrCpu);
  free(arrSortedGpu);

  if (match)
    printf("\033[1;32mFUNCTIONAL SUCCESS\n\033[0m");
  else
  {
    printf("\033[1;31mFUNCTIONCAL FAIL\n\033[0m");
    return 0;
  }

  printf("\033[1;34mArray size         :\033[0m %d\n", size);
  printf("\033[1;34mCPU Sort Time (ms) :\033[0m %f\n", cpuTime);
  float gpuTotalTime = h2dTime + gpuTime + d2hTime;
  float meps = size / (gpuTotalTime * 0.001) / 1e6;
  printf("\033[1;34mGPU Sort Time (ms) :\033[0m %f\n", gpuTotalTime);
  printf("\033[1;34mGPU Sort Speed     :\033[0m %f million elements per second\n", meps);
  if (gpuTotalTime < cpuTime)
  {
    printf("\033[1;32mPERF PASSING\n\033[0m");
    int speedup = (int)(cpuTime / gpuTotalTime + 0.5);
    printf("\033[1;34mGPU Sort is \033[1;32m %dx \033[1;34mfaster than CPU !!!\033[0m\n", speedup);
    printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
    printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
    printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
  }
  else
  {
    int speedup = (int)(gpuTotalTime / cpuTime + 0.5);
    printf("\033[1;31mPERF FAILING\n\033[0m");
    printf("\033[1;34mGPU Sort is \033[1;31m%dx \033[1;34mslower than CPU, optimize further!\n", speedup);
    printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
    printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
    printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
    return 0;
  }

  return 0;
}
