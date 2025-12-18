#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

// gcc -g -pthread threads.c -o threads
// ./threads

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) {

    // pthread_t id = pthread_self(); // managed by the pthread library itself
    // printf("Thread ID: %lu\n", id);
    // printf("Thread ID: %ld\n", syscall(SYS_gettid)); // Linux kernel thread ID
    // pthread_t is an opaque data type, and should not be directly dereferenced



    // You can access the argument passed to the thread like this:
    // int index = *((int *) arg);
    // OR
    int *indexPtr = (int *) arg;
    int index = *indexPtr;
    int sum = 0;
    for (int j = 0; j < 5; j++) {
        sum += primes[index + j];
    }
    printf("Local sum: %d\n", sum);
    *((int *) arg) = sum;
    return arg;
    // or instead of returning the argument, you can call pthread_exit(arg);
    // pthread_exit((void*) arg);
    // If pthread_exit is called in the main function, the program will terminate. But
    // unlike exit(), the threads will continue to run. Exit terminates the entire process
    // and all the threads it has created. But calling pthread_exit in the main function
    // will only terminate the main function and the threads will continue to run.
}

int main(int argc, char* argv[]) {

    pthread_t th[2];

    int i;
    for (i = 0; i < 2; i++) {

        int* a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
        // if (pthread_create(th + i, NULL, &routine, a) != 0) {
        //     perror("Failed to create thread");
        // }
    }
    int globalSum = 0;
    for (i = 0; i < 2; i++) {
        int* r;
        // pthread_join waits for the thread to terminate and retrieves the return value of the thread
        // The second argument is a pointer to a pointer to void, which will be set to the return value of the thread
        // If the thread returns a value using pthread_exit, that value will be returned here
        if (pthread_join(th[i], (void**) &r) != 0) {
            perror("Failed to join thread");
        }
        globalSum += *r;
        free(r);
    }
    printf("Global sum: %d\n", globalSum);
    return 0;
}






// Another Example:

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine2(void* arg) {
    int index = *(int*)arg;
    int sum = 0;
    for (int j = 0; j < 5; j++) {
        sum += primes[index + j];
    }
    printf("Local sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    int i;
    for (i = 0; i < 2; i++) {
        int* a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine2, a) != 0) {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (i = 0; i < 2; i++) {
        int* r;
        if (pthread_join(th[i], (void**) &r) != 0) {
            perror("Failed to join thread");
        }
        globalSum += *r;
        free(r);
    }
    printf("Global sum: %d\n", globalSum);
    return 0;
}







// Static initialization of mutex and condition variable

#define THREAD_NUM 2

pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFuel = PTHREAD_COND_INITIALIZER;

void *routine(void *args)
{
}

int main(int argc, char *argv[])
{
  pthread_t th[THREAD_NUM];

  int i;
  for (i = 0; i < THREAD_NUM; i++)
  {
    if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
    {
      perror("Failed to create thread");
    }
  }

  for (i = 0; i < THREAD_NUM; i++)
  {
    if (pthread_join(th[i], NULL) != 0)
    {
      perror("Failed to join thread");
    }
  }
  return 0;
}