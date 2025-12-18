#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <cuda_runtime.h>
#include <algorithm>
// ==== DO NOT MODIFY CODE ABOVE THIS LINE ====

#define DTYPE int
#include <cuda.h>
#include <climits>

#define THREADS_PER_BLOCK 512

__global__ void bitonicSortKernel(DTYPE *arr, int n, int k, int j)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;

  if (idx >= n)
    return;

  int ixj = idx ^ j;

  if (ixj > idx)
  {
    if ((idx & k) == 0)
    {
      // Ascending order
      if (arr[idx] > arr[ixj])
      {
        DTYPE temp = arr[idx];
        arr[idx] = arr[ixj];
        arr[ixj] = temp;
      }
    }
    else
    {
      // Descending order
      if (arr[idx] < arr[ixj])
      {
        DTYPE temp = arr[idx];
        arr[idx] = arr[ixj];
        arr[ixj] = temp;
      }
    }
  }
}

__global__ void fillPaddingKernel(DTYPE *arr, int start, int end, DTYPE value)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int pos = start + idx;
  if (pos < end)
  {
    arr[pos] = value;
  }
}

void bitonicSort(DTYPE *d_arr, int n)
{
  // Find the next power of 2
  int pow2 = 1;
  while (pow2 < n)
  {
    pow2 <<= 1;
  }

  // Bitonic sort algorithm
  for (int k = 2; k <= pow2; k <<= 1)
  {
    for (int j = k >> 1; j > 0; j >>= 1)
    {
      int num_blocks = (pow2 + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
      bitonicSortKernel<<<num_blocks, THREADS_PER_BLOCK>>>(d_arr, pow2, k, j);
      cudaDeviceSynchronize();
    }
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

  int size = atoi(argv[1]);

  srand(time(NULL));

  // ===== OPTIMIZED MEMORY ALLOCATION =====
  // Use pinned memory for faster transfers
  DTYPE *arrCpu = nullptr;
  DTYPE *arrSortedGpu = nullptr;
  cudaMallocHost(&arrCpu, size * sizeof(DTYPE));       // Pinned input
  cudaMallocHost(&arrSortedGpu, size * sizeof(DTYPE)); // Pinned output

  for (int i = 0; i < size; i++)
  {
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
  while (pow2_size < size)
  {
    pow2_size <<= 1;
  }

  // Allocate GPU memory
  cudaMalloc(&d_arr, pow2_size * sizeof(DTYPE));

  // ===== OPTIMIZED H2D TRANSFER =====
  // Async copy of main data
  cudaMemcpyAsync(d_arr, arrCpu, size * sizeof(DTYPE),
                  cudaMemcpyHostToDevice, h2d_stream);

  // Fill padding using kernel (faster than CPU->GPU copy)
  if (pow2_size > size)
  {
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
  for (int i = 0; i < size; i++)
  {
    if (arrSortedGpu[i] != arrCpu[i])
    {
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
  else
  {
    printf("\033[1;31mFUNCTIONCAL FAIL\n\033[0m");
    return 0;
  }

  printf("\033[1;34mArray size         :\033[0m %d\n", size);
  printf("\033[1;34mCPU Sort Time (ms) :\033[0m %f\n", cpuTime);
  float gpuTotalTime = h2dTime + gpuTime + d2hTime;
  int speedup = (gpuTotalTime > cpuTime) ? (gpuTotalTime / cpuTime) : (cpuTime / gpuTotalTime);
  float meps = size / (gpuTotalTime * 0.001) / 1e6;
  printf("\033[1;34mGPU Sort Time (ms) :\033[0m %f\n", gpuTotalTime);
  printf("\033[1;34mGPU Sort Speed     :\033[0m %f million elements per second\n", meps);
  if (gpuTotalTime < cpuTime)
  {
    printf("\033[1;32mPERF PASSING\n\033[0m");
    printf("\033[1;34mGPU Sort is \033[1;32m %dx \033[1;34mfaster than CPU !!!\033[0m\n", speedup);
    printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
    printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
    printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
  }
  else
  {
    printf("\033[1;31mPERF FAILING\n\033[0m");
    printf("\033[1;34mGPU Sort is \033[1;31m%dx \033[1;34mslower than CPU, optimize further!\n", speedup);
    printf("\033[1;34mH2D Transfer Time (ms):\033[0m %f\n", h2dTime);
    printf("\033[1;34mKernel Time (ms)      :\033[0m %f\n", gpuTime);
    printf("\033[1;34mD2H Transfer Time (ms):\033[0m %f\n", d2hTime);
    return 0;
  }

  return 0;
}

// Achieved Occupancy: 42.86
// Memory Throughput: 43.2
// FUNCTIONAL SUCCESS
// Array size         : 100000000
// CPU Sort Time (ms) : 16409.003906
// GPU Sort Time (ms) : 153.734146
// GPU Sort Speed     : 650.473572 million elements per second
// PERF PASSING
// GPU Sort is  106x faster than CPU !!!
// H2D Transfer Time (ms): 7.969056
// Kernel Time (ms)      : 138.139969
// D2H Transfer Time (ms): 7.625120

// FUNCTIONAL SUCCESS
// Array size         : 100000000
// CPU Sort Time (ms) : 16465.550781
// GPU Sort Time (ms) : 153.664688
// GPU Sort Speed     : 650.767578 million elements per second
// PERF PASSING
// GPU Sort is  107x faster than CPU !!!
// H2D Transfer Time (ms): 7.906656
// Kernel Time (ms)      : 138.142471
// D2H Transfer Time (ms): 7.615552

// FUNCTIONAL SUCCESS
// Array size         : 100000000
// CPU Sort Time (ms) : 16665.945312
// GPU Sort Time (ms) : 154.491226
// GPU Sort Speed     : 647.285950 million elements per second
// PERF PASSING
// GPU Sort is  107x faster than CPU !!!
// H2D Transfer Time (ms): 7.989888
// Kernel Time (ms)      : 138.186874
// D2H Transfer Time (ms): 8.314464

// Kernel Time: 138.139969ms, Score: 5.791
// Memory Transfer Time: 15.522208ms, Score: 4
// Million elements per second: 650.778
// Total Score: 15.79 pts