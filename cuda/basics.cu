



// nvcc compiler is used to compile CUDA code

// Compilation model:

// 1. Off-line compilation model:
// nvcc separates the code into host code and device code
// Host code is compiled to x86-64 assembly using a standard C++ compiler (like g++)
// and is modified to run kernel launches on the GPU
// Device code is compiled to PTX (Parallel Thread Execution) assembly using nvcc which
// is stable across GPU architectures and generations.

// 2. Just-in-time (JIT) compilation model:
// PTX code is compiled to native GPU instructions
// i.e SASS (Streaming Assembler) code. This compilation happens when the program is loaded
// onto the GPU for execution. The JIT compiler is part of the GPU driver and is specific
// to the GPU architecture and generation. This allows the same PTX code to run on different
// GPU architectures and generations without recompilation and allows for future GPU architectures
// (forward compatibility).



// There is a quite extensive library that gets linked in with you code at the runtime
// called as cudart (CUDA runtime library) which provides a lot of functionality like
// device memory management, kernel launching, error handling, shared memory management(__shared__),
// synchronization(__syncthreads()), asynchronous execution, etc.


// __device__ is a "Declaration Specifier" that indicates to the compiler that this function
// is going to run on the device(GPU) and can only be called from the device(GPU).

// __host__ is a "Declaration Specifier" that indicates to the compiler that this function
// is going to run on the host(CPU) and can only be called from the host(CPU). This is the default.

// __global__ is a "Declaration Specifier" that indicates to the compiler that this function
// is a kernel function that executes on the GPU(device) and can be called from the CPU(host)

// __global__ is a "Declaration Specifier" that indicates to the compiler that that this function is a kernel
// function that runs on the GPU and can be called from the CPU
__global__ void kernel(...)
{ // Kernel function to be executed on GPU

  // Perform this operation for every thread

  // Each thread has it own thread index within a block
  // Each block has its own block index within a grid

  // Thread can have as many as 3 dimensions (x, y, z)
  // Each thread has its own unique thread index within a block which is accessible within
  // the kernel using threadIdx.x, threadIdx.y, threadIdx.z variables
  // But that index is only unique within the block and not globally unique across the entire grid

  // Say we launch a kernel with 2 blocks and 4 threads per block
  // Block 0(blockIdx.x = 0) -> Thread 0(threadIdx.x = 0), Thread 1(threadIdx.x = 1), Thread 2(threadIdx.x = 2), Thread 3(threadIdx.x = 3)
  // Block 1(blockIdx.x = 1) -> Thread 0(threadIdx.x = 0), Thread 1(threadIdx.x = 1), Thread 2(threadIdx.x = 2), Thread 3(threadIdx.x = 3)
  // So, threadIdx.x is not unique across the entire grid
  // To get a globally unique thread index across the entire grid, we need to use blockIdx.x and blockDim.x
  // blockIdx.x -> index of the block within the grid (here 0 or 1)
  // blockDim.x -> number of threads per block (here 4)
  // So, the global thread index can be calculated as:
  // global_thread_index = (blockIdx.x * blockDim.x) + threadIdx.x
  // this is if we are using a 1D grid and 1D blocks


  // Similarly, for 2D and 3D grids, we can calculate the global thread index as:
  // global_thread_index_x = blockIdx.x * blockDim.x + threadIdx.x
  // global_thread_index_y = blockIdx.y * blockDim.y + threadIdx.y
  // global_thread_index_z = blockIdx.z * blockDim.z + threadIdx.z



  // Local Memory: Each thread has its own private local memory which is not shared with other threads
  int local_var = 0; // This variable is private to this thread and stored in local memory
  local_var += 1; // Each thread will have its own copy of local_var
  // Variables declared in a kernel function are stored in registers if possible. It's on-chip memory and very fast.
  // If there are too many variables or if the variables are too large like a large array,
  // they may be spilled to local memory which is off-chip and slower to access.
  // Local refers to the scope of the variable, not the location of the memory. It's local to each thread.
  // Local memory is compiler controlled.

  // Shared Memory: Each block has its own shared memory which is shared among all threads in that block
  __shared__ int shared_var; // This variable is shared among all threads in the block
  if (threadIdx.x == 0) {
    shared_var = 0; // Initialize shared variable by one thread
  }
  __syncthreads(); // Synchronize all threads in the block to ensure shared_var is initialized
  shared_var += 1; // All threads in the block can access and modify shared_var
  __syncthreads(); // Synchronize again to ensure all threads have updated shared_var
  // Note: shared memory is much faster than global memory but is limited in size (typically 48KB per block)
  // Shared memory allows threads within a block to cooperate and communicate with each other.
  // it can be thought of as user defined L1 cache.


  // Global Memory: All threads in all blocks can access global memory which is the main memory of the GPU
  // Accessing global memory is much slower than accessing local or shared memory
  // Global memory is allocated using cudaMalloc() and accessed using pointers

  // GPUs don't have a very big cache like CPUs, we can implement a very simple kind of cache using constant memory.
  // Constant memory is very large since it's located in the devices global memory(DRAM). All threads have access to it
  // but it's read-only memory. We want to use this memory space for data that doesn't change over the course
  // of the kernel execution and is accessed frequently. Although it's physically located in off-chip global memory, it's
  // content is aggressively cached on-chip memory which makes it much faster to access than regular global memory.
  // Constant Memory: A small amount of read-only memory that is cached and optimized for broadcast to all threads.
  // Therefore using constant memory can substantially reduce global memory traffic when all threads read the same value.
  __constant__ int const_var; // This variable is read-only and can be accessed by all threads



  // Cuda cores are grouped into Streaming Multiprocessors (SMs)
  // Each SM can execute multiple blocks of threads concurrently
  // The GPU scheduler schedules blocks to SMs for execution

  // Shared memory and registers are limited resources on each SM and are shared among all blocks running on that SM
  // and is called "on-chip device memory"

  // The global and local memory are stored in "off-chip device memory" which is much larger but slower to access
  // and is not located on the GPU chip itself

  // Speed:
  // Registers > Shared Memory > Local Memory > Global Memory > Host Memory (CPU RAM)


  // A group of 32 execution contexts is kinda like CPU thread with vector registers.
  // Nvidia calls this concept a warp. So warp is a block of 32 execution contexts for 32
  // CUDA threads that when those CUDA threads are doing the same thing, those 32 CUDA threads
  // actually get SIMD-like execution on this GPU hardware. So threads in a warp are executed
  // in a SIMD manner if they are at the same point in the program and it's upto the hardware
  // to identify that it's at the same point in the program and if yes then it can execute them
  // in one clock cycle SIMD-like fashion. If they are not at the same point in the program
  // then it has to execute them in multiple clock cycles. So it's very important to write
  // code in such a way that threads in a warp are at the same point in the program as much as possible.
  // This is called warp divergence and it can cause performance degradation.
  // Warp is the smallest unit of creation, management, scheduling and execution of threads on the GPU.
  // Each thread in the warp start at the same program address and execute the same instruction, has it's
  // own program counter, register state and local memory state and can branch and execute independently.
  // Warps can be decomposed into half-warps and quarter-warps of 16 and 8 threads respectively.
  // There is one control circuit that's responsible for managing the execution of all 32 threads in a warp.
  // Maximum efficiency is achieved when all threads in a warp follow the same execution path.
  // If paths diverge due to conditional branches, within a warp, the warp serially executes each branch path
  // causing a performance penalty. So the part of the code inside an "if" statement is executed serially
  // one thread at a time and the part of the code inside the "else" statement is temporarily disabled.
  // And then it's going to switch, enable the "else" part and disable the "if" part and execute
  // the "else" part serially one thread at a time. When all threads reconverge, it resumes parallel execution.
  // This is called warp divergence and it can cause performance degradation.

  // Warps are independent of each other and can be scheduled and executed independently.
  // Execution contexts of warps stay on-chip for the lifetime of the warp(program counter, registers, shared memory)
  // Warp-to-warp context switching is free and very fast since the context stays on-chip.

  // Number of blocks or warps that fit on an SM is limited by the amount of shared memory and registers
  // available on that SM. This is called occupancy. Higher occupancy can help hide memory latency
  // and improve performance. But higher occupancy does not always mean better performance.

  // All 32 threads in a warp execute the same instruction at the same time
  // but on different data. This is called Single Instruction Multiple Thread (SIMT) architecture.
  // If threads in a warp diverge due to conditional statements, the warp serially executes each branch path
  // causing a performance penalty. Therefore, it's important to minimize thread divergence within a warp.
  // Each SM can have multiple warps resident at the same time. This is called warp scheduling.
  // Each SM has a warp scheduler that selects which warp to execute next. The scheduler uses a round-robin
  // or priority-based algorithm to select the next warp to execute.
  // Warp is a hardware implementation detail and is not exposed to the programmer.
  // A block is a software abstraction that is exposed to the programmer.


  // NVIDIA v100 GPU has 80 SMs
  // Each SM has 64 warps (i.e 64 * 32 = 2048 threads)
  // 64 warps are divided into 4 groups of 16 warps each called warp groups
  // Each warp group has its own warp selector and it's own fetch and decode unit
  // Each SM has 4 warp schedulers (one for each warp group)
  // Each warp group have their own ALU units, load/store units and special function units
  // Each warp group has 64KB registers and thus total 256KB registers per SM
  // All the 4 warp groups share "Shared Memory" and "L1 Cache" of 128KB
  // Each SM has 48KB of shared memory (configurable to 96KB) and 128KB of L1 cache
  // Inside each warp group is:
  //  1. SIMD fp32 function unit with 16 lanes (16 MUL> ADD fp32 operations can be performed in
  //      parallel in one clock cycle. so if there is one 32 wide SIMD instruction, it takes 2 clock cycles to execute),
  //  2. SIMD INT32 function unit with 16 lanes,
  //  3. SIMD fp64 function unit with 8 lanes, (so if there is one 32 wide SIMD instruction, it takes 4 clock cycles to execute)
  //. 4. Tensor Core unit with 8 lanes (for matrix operations),
  //  5. Load/Store unit with 8 lanes (16 memory operations can be performed in parallel in one clock cycle)

  // In a given clock cycle, only one warp can run on each warp group. So 4 warps can run in
  // parallel on each SM in a given clock cycle (one warp per warp group).
  // So, in one clock cycle, total number of threads that can run on one SM = 4 warps * 32 threads/warp = 128 threads
  // and a total of 4 * 16 = 64 SIMD fp32 operations can be performed in parallel

  //  NVIDIA v100 GPU has 80 of these SMs. ALl SMs share access to the same global memory which
  // is of size 16GB and is located off-chip. The global memory is divided into
  // multiple memory partitions (typically 32 or 16) to increase memory bandwidth.
  // All 80 SMs also share L2 cache of size 6MB which is located on-chip.

  //  NVIDIA v100 GPU geometry:
  // If clock speed = 1.245 GHz,
  //     80 SMs per core chip,
  //  80 * 4 * 16 = 5120 fp32 mul-add operations per clock cycle
  //     = 12.7 TeraFLOPS (add-mul counted as 2 FLOPs)
  // Upto 80 * 4 * 16 = 5120 interleaved warps per core chip
  //     = 5120 * 32 = 163840 threads per core chip


  // GPU chip is divided into multiple "Graphics Processing Clusters" (GPCs)
  // Each GPC has multiple SMs (typically 4 or 8 or 12).
  // Each SM has 4 warp groups (16 warps each).
  // So number of cores per GPC = number of SMs per GPC * 4 warp groups * 16 warps * 32 threads
  // Each warp has thus 32 CUDA cores and one tensor core

  // Gigathread engine is responsible for managing the GPCs and SMs within the GPCs.

  // Thread block is the granularity of synchronization, shared memory sharing and
  // data locality when writing a CUDA program.

  // Communication between threads blocks is possible, but you cannot make assumptions
  // about the order in which blocks will execute. So thread blocks can interact with each other
  // but cannot make assumptions about the order in which they execute. While as threads within a block,
  // you can assume they are all running at the same time and can communicate and synchronize
  // with each other using shared memory and barriers and __syncthreads() function.

  // Thread block (aka block): a group of threads that can share an on-chip scratchpad called shared memory
  // and can synchronize with __syncthreads.
  // Blocks are independent units that the hardware schedules on SMs.
  // Hardware mapping: blocks are scheduled onto SMs; each SM hosts multiple active warps
  // concurrently (to hide latency), switching between them cycle-by-cycle
  // A block is assigned to one SM. All threads in a block run on the same SM and share
  // its shared memory and registers. A block cannot span multiple SMs because shared memory
  // is private to each SM. A block can have as many as 1024 threads (for modern GPUs).

  // A grid is distributed across multiple SMs.Hardware decides which blocks go to which SMs.
  // You don’t control this scheduling(by design).

  // CUDA cores:
  // the scalar ALU lanes that run most shader/compute instructions.
  // Each CUDA core is a simple ALU that executes one thread’s instruction in a warp.
  // A warp has 32 threads → requires 32 CUDA cores(ALU units) to execute one instruction for all of them in a single cycle.
  // If an SM has, say, 128 CUDA cores, it can execute 4 warps(128 threads) worth of scalar instructions per cycle.
  // NVIDIA used “CUDA cores” as a marketing term to draw analogy to CPU cores. But architecturally, they’re ALUs inside an SM.
  // CPU core = big, independent engine with control logic, out-of-order execution, multiple pipelines.
  // CUDA core = simple datapath(an ALU) that executes one instruction from one thread in a warp.

  // Tensor cores:
  // Specialized hardware inside SMs.
  // Instead of doing 32 independent multiply - adds, a tensor core can do a matrix - matrix
  // multiply(like 4×4 or 8×8) in a single operation.
  // Warps can be scheduled onto tensor cores when you use libraries like cuBLAS / cuDNN.

  // Ray tracing cores(RT cores):
  // In GPUs for graphics, these accelerate ray-box and ray-triangle intersections.
  // They are side units that SMs can use but aren’t involved in normal CUDA compute.






















  // Calculate global thread index
  // for 2D grid and 2D blocks

  // We’re working with 2D problems (like matrices or images).
  // A matrix has rows and columns.
  // CUDA gives us a hierarchy: grid → blocks → threads.
  // But the GPU doesn’t magically know which thread should work on which row/col of the matrix.
  // So we compute a global row index and global column index from the thread’s coordinates.

  // The ingredients (CUDA built-in variables):

  // 1.threadIdx: This is the thread's index inside its own block.
  // For a 2D block:
  // threadIdx.x → horizontal index inside the block
  // threadIdx.y → vertical index inside the block

  // 2.blockDim: This is the size of the block(number of threads along each dimension).
  // blockDim.x → number of threads along x in a block
  // blockDim.y → number of threads along y in a block

  // 3. blockIdx: This is the block’s index in the grid.
  // blockIdx.x → horizontal block index
  // blockIdx.y → vertical block index

  // Each thread’s global position in the entire grid of threads (and thus the data) is:
  // Essentially: "Which row and column in the full data array should this thread work on?"
  int row = blockIdx.x * blockDim.x + threadIdx.x; // global thread index along the x-dimension of the grid.
  int col = blockIdx.y * blockDim.y + threadIdx.y; // global thread index along the x-dimension of the grid.

  // If you want a single flattened ID, you usually do:
  int globalId = row * (gridDim.x * blockDim.x) + col;









  // Alternatively, we can think of the entire grid as a 3D apartment complex


  // dim3 blocksPerGrid(2, 3, 4); // 2 blocks in x, 3 blocks in y, 4 blocks in z
  // dim3 threadsPerBlock(3, 3, 3); // 3 threads in x, 3 threads in y, 3 threads in z

  // Here, we have a grid of 2 * 3 * 4 = 24 blocks (apartments) and each block has
  // 3 * 3 * 3 = 27 threads (people) in it. So total number of threads in the grid is
  // 24 * 27 = 648 threads (people) in the entire grid (apartment complex)
  // Each block (apartment) can be identified by its blockIdx (x, y, z)
  // Each thread (person) within a block can be identified by its threadIdx (x, y, z)
  // To get a unique global thread id (person id) in the entire grid (apartment complex)

  int global_x = blockIdx.x * blockDim.x + threadIdx.x;
  int global_y = blockIdx.y * blockDim.y + threadIdx.y;
  int global_z = blockIdx.z * blockDim.z + threadIdx.z;

  // which block we are in
  int block_id =
      blockIdx.x +                        // apartment number on this floor (points across)
      blockIdx.y * gridDim.x +            // floor number in this building (rows high)
      blockIdx.z * gridDim.x * gridDim.y; // building number in this city (panes deep)

  //
  int block_offset =
      block_id *                            // times our apartment number
      blockDim.x * blockDim.y * blockDim.z; // total threads per block (people per apartment)

  int thread_offset =
      threadIdx.x +
      threadIdx.y * blockDim.x +
      threadIdx.z * blockDim.x * blockDim.y;

  int id = block_offset + thread_offset; // global person id in the entire apartment complex

  printf("%04d | Block(%d %d %d) = %3d | Thread(%d %d %d) = %3d\n",
         id,
         blockIdx.x, blockIdx.y, blockIdx.z, block_id,
         threadIdx.x, threadIdx.y, threadIdx.z, thread_offset);

  // Result:
  // 0351 | Block(1 0 2) =  351 | Thread(0 0 0) =   0
  // 0352 | Block(1 0 2) =  351 | Thread(1 0 0) =   1
  // 0353 | Block(1 0 2) =  351 | Thread(2 0 0) =   2
  // 0354 | Block(1 0 2) =  351 | Thread(3 0 0) =   3
  // 0355 | Block(1 0 2) =  351 | Thread(0 1 0) =   4
  // 0356 | Block(1 0 2) =  351 | Thread(1 1 0) =   5
  // 0357 | Block(1 0 2) =  351 | Thread(2 1 0) =   6
  // 0358 | Block(1 0 2) =  351 | Thread(3 1 0) =   7
  // ......
  // 0377 | Block(1 0 2) =  351 | Thread(2 2 2) =  26

  // 0270 | Block(0 2 1) =  270 | Thread(0 0 0) =   0
  // 0271 | Block(0 2 1) =  270 | Thread(1 0 0) =   1
  // 0272 | Block(0 2 1) =  270 | Thread(2 0 0) =   2




  // Match dimensions of data
  // 1D data → use 1D blocks and 1D grid.
  // 2D data (matrix, image) → 2D blocks and 2D grid.
  // 3D data (volume, video, voxel grids) → 3D blocks and 3D grid.
  // This makes the mapping between threads and data natural: each thread handles one element (or a small chunk) of data.


  // Consider block size for efficiency
  // CUDA hardware schedules threads in warps of 32 threads.
  // So the x-dimension of a block is usually a multiple of 32.
  // Typical 2D blocks for images: 16x16 or 32x8 threads.
  // Rule of thumb
  // 1D blocks → multiple of 32 threads
  // 2D blocks → product blockDim.x * blockDim.y = multiple of 32, ideally ≤ 1024
  // This ensures max efficiency and avoids wasting GPU resources.

  // Why 16x16 or 32x8 for 2D blocks
  // For 2D data (like images), we use 2D blocks. Each block has blockDim.x * blockDim.y threads.
  // Examples:
  // 16x16 → 256 threads per block → 256 / 32 = 8 warps
  // 32x8 → 256 threads per block → 256 / 32 = 8 warps
  // Both fully utilize warps and match typical GPU hardware limits (max threads per block = 1024 for most GPUs).
  // Also, a square-ish block (16x16) often gives better memory access patterns for 2D arrays,
  // making global memory access coalesced (faster).
  // If block size is 4 X 10 = 40 threads, then 32 threads will be in one warp and 8 threads
  // will be in another warp. The second warp will be only 25% utilized which is inefficient.


  // How Many CUDA Blocks on One Stream Multiprocessor?
  // How many CUDA blocks can be executed on one SM can be decided by the following factors.
  // Number of registers, shared memory, and number of threads.
  // Three main hardware constraints for occupancy
  // On any GPU, the number of blocks that can reside on an SM is limited by three things:
  // Max threads per SM (hard limit, e.g., 2048 on many architectures).
  // Register file size (each SM has a fixed pool of registers).
  // Shared memory size (each SM has a fixed pool of shared memory).
  // Whichever runs out first limits occupancy.

  // 1. hreads vs warps
  // An SM can have up to 2048 active threads (on Ampere, for example).
  // But threads don’t execute one by one. They are grouped into warps (32 threads per warp).
  // So 2048 threads = 2048 ÷ 32 = 64 active warps max per SM.

  // 2. What “active” means
  // Active threads/warps = they have resources allocated (registers, shared memory)
  // and are eligible to run.
  // They are all resident inside the SM at once.
  // Their registers are stored in the SM’s register file, and their block’s shared memory is allocated in the SM’s scratchpad.
  // So yes, all 2048 are “active” simultaneously in that sense.

  // 3. But are they all executing in the same cycle?
  // No.
  // The SM has a limited number of execution units (ALUs, tensor cores, load/store units).
  // A warp scheduler picks one warp per cycle (per scheduler), and issues instructions.
  // On modern GPUs, each SM has 4 warp schedulers, so up to 4 warps can issue instructions per cycle.
  // The other warps are waiting in the ready queue.
  // Think of it like:
  // 2048 threads are “seated” inside the SM.
  // But only a few dozen instructions from a handful of warps execute per clock.
  // Over time, the schedulers rotate through the warps so all threads make progress.

  // 4. Why keep so many active threads?
  // Because of latency hiding.
  // If one warp stalls on a global memory load (hundreds of cycles), the SM instantly
  // switches to another warp.
  // With 64 warps ready, the SM almost always has something to do.
  // That’s how GPUs achieve massive throughput even with high-latency memory.
  // On most SMs only 4 warps out of 64 are actually executing in any given cycle.

  // 5. Warp schedulers per SM
  // Modern NVIDIA SMs (e.g., Ampere, Ada, Hopper) have 4 warp schedulers.
  // Each warp scheduler can issue one instruction per cycle from a warp it manages.
  // That means up to 4 warps can issue instructions per cycle on one SM.
  // So out of the 64 active warps (2048 threads), only 4 are running (issuing) in a given cycle.

  // 6. But all warps are “ready”
  // The other 60 warps aren’t dead — they are active and waiting.
  // If the currently running warps stall (say on global memory access), the scheduler can
  // instantly swap in other ready warps.
  // This swap is zero-cost because all registers are already resident in the SM.


  // Let’s take a closer look at one example. We will explore the example with 256 threads
  // and 64 kilobyte of register files and 32 kilobytes of shared memory.
  // In the software side, say each CUDA block has 32 threads and two kilobytes of shared memory, and
  // each thread has 64 registers.
  // If the occupancy is constrained by running number of threads, the total number of CUDA blocks
  // per SM would be 256/32, which means 8 CUDA blocks.
  // If the occupancy is constrained by the registers, it will be 641,024/6432, which means 32 CUDA blocks.
  // If the occupancy is constrained by the shared memory size, it will be 32 kilobyte divided by
  // 2 kilobyte equals 16 CUDA blocks.
  // So the final answer is the minimum of all constraints, so it will be 8 CUDA blocks per SM.




  // Perform computations here using idx, idy, idz
}

int main(void) { // Host code

  // Do some CPU work here



  // Allocate and initialize memory on GPU or device
  cudaMalloc(...);
  // cudaMalloc(&ptr, num_bytes)
  // ptr -> pointer to the address of allocated memory in GPU
  // num_bytes -> number of bytes to allocate

  // Copy data from CPU to GPU (Host to Device)
  cudaMemcpy(...);
  // cudaMemcpy(dest, src, num_bytes, direction)
  // direction -> cudaMemcpyHostToDevice or cudaMemcpyDeviceToHost


  // Launch kernel on GPU with N blocks and M threads per block
  // N -> Grid size -> Number of blocks in the grid
  // M -> Block size -> Number of threads per block

  // configure the number of blocks and threads
  dim3 N(3, 2); // 6 blocks in the grid
  dim3 M(4, 3, 1); // 12 threads per block

  // Launch the kernel
  kernel<<<N, M>>>(...);

  // Copy results from GPU back to CPU (Device to Host)
  cudaMemcpy(...);


  // free GPU memory
  cudaFree(...);
  // cudaFree(ptr)



  // Force the CPU to wait until all preceding GPU tasks are complete
  cudaDeviceSynchronize();
  // This is necessary because kernel launches and memory copies are asynchronous


  // Do some more CPU work here



}




// CPU: few powerful cores, optimized for low latency, complex logic.
// GPU: many simple cores, optimized for high throughput, massive parallelism.

// GPU
//  ├─ GPCs(Graphics Processing Clusters)
//  │   ├─ SMs(Streaming Multiprocessors)
//  │   │   ├─ Warp Schedulers
//  │   │   ├─ CUDA cores(scalar ALUs)
//  │   │   ├─ Tensor cores(matrix ALUs)
//  │   │   ├─ RT cores(ray tracing)
//  │   │   ├─ Shared memory + Registers
//  │   │   └─ L1 Cache
//  │   └─ Raster / Texture units(graphics path)
//  └─ L2 Cache + Memory controllers
//      └─ HBM / GDDR VRAM
