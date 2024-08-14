#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define THREAD_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* args) {
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("Incremented fuel to %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    pthread_mutexattr_t recursiveMutexAttributes;
    pthread_mutexattr_init(&recursiveMutexAttributes);

    // Recursice mutex is a mutex that can be locked multiple times by the same thread.
    pthread_mutexattr_settype(&recursiveMutexAttributes, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutexFuel, &recursiveMutexAttributes);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    printf("Fuel: %d\n", fuel);
    pthread_mutexattr_destroy(&recursiveMutexAttributes);
    pthread_mutex_destroy(&mutexFuel);
    return 0;
}




// Lock vs Trylock




pthread_mutex_t mutex;

void* routine_lock(void* arg) {
    pthread_mutex_lock(&mutex);
    printf("Got lock\n");
    sleep(1);
    pthread_mutex_unlock(&mutex);
}

void* routine_trylock(void* arg) {

    // If the mutex is already locked, pthread_mutex_trylock() will return immediately
    // with a return value of EBUSY. But if the mutex is not locked, pthread_mutex_trylock()
    // will lock the mutex and return 0.
    // In contrast, pthread_mutex_lock() will block until the mutex is available.
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("Got lock\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Didn't get lock\n");
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Error at creating thread");
        }
    }
    for (int i = 0; i < 4; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Error at joining thread");
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}