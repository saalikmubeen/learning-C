CUDA (Compute Unified Device Architecture) is a parallel computing platform and application programming interface (API) model created by NVIDIA. It allows developers to use a CUDA-enabled graphics processing unit (GPU) for general-purpose processing – an approach known as GPGPU (General-Purpose computing on Graphics Processing Units). CUDA provides a C/C++-like programming language and APIs for managing GPU resources, memory, and execution of parallel tasks.

Key concepts in CUDA include:

1. **Kernels**: Functions that run on the GPU and are executed in parallel by multiple threads.
2. **Threads**: The smallest unit of execution in CUDA, with each thread executing a kernel.
3. **Blocks**: A group of threads that execute a kernel together and can cooperate through shared memory.
4. **Grids**: A collection of blocks that execute a kernel, allowing for scalable parallelism.
5. **Memory Hierarchy**: CUDA provides different types of memory (global, shared, local) with varying scopes and lifetimes to optimize performance.

Thread gets mapped to a single core in the GPU

A block gets mapped to a single Streaming Multiprocessor (SM) in the GPU
A grid gets mapped to the entire GPU

Thread hierarchy:

- Thread: The smallest unit of execution, each thread has its own registers and local memory.
- Block: A group of threads that can share data through shared memory and synchronize their execution.
- Grid: A collection of blocks that execute a kernel function.
