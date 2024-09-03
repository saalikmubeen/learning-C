#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutexFuel;

// Condition variable to signal that fuel is filled
pthread_cond_t condFuel;


int fuel = 0;

void* fuel_filling(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Filled fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        // pthread_cond_signal is used to signal only one thread waiting on condFuel
        // The thread is picked randomly by the system scheduler to be woken up
        // If you want to wake up all the threads waiting on condFuel, use pthread_cond_broadcast
        pthread_cond_signal(&condFuel);
        sleep(1);
    }
}

void* car(void* arg) {
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");
        // Make the thread wait on a condition (until the fuel is filled) and give up the mutex(lock)
        pthread_cond_wait(&condFuel, &mutexFuel);
        // Equivalent to:
        // pthread_mutex_unlock(&mutexFuel);
        // wait for signal on condFuel
        // Once the signal is received, lock the mutex again:
        // pthread_mutex_lock(&mutexFuel);
    }
    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);
    for (int i = 0; i < 2; i++) {
        if (i == 1) {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);
    return 0;
}



// No fuel. Waiting...
// Filled fuel... 15
// No fuel. Waiting...
// Filled fuel... 30
// No fuel. Waiting...
// Filled fuel... 45
// Got fuel. Now left: 5
// Filled fuel... 20
// Filled fuel... 35





// Static initialization of mutex and condition variable

#define THREAD_NUM 2

pthread_mutex_t mutexFuel = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFuel = PTHREAD_COND_INITIALIZER;

void* routine(void* args) {
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];

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
    return 0;
}








// Operations on Condition Variables
//
// • Condition variable: is a queue of threads waiting for something (some condition to change)
//   inside a critical section
// • Condition variables support three operations:
//      1. Wait(Lock lock): atomic (release lock, go to sleep), when the process wakes up it re-acquires lock.
//      2. Signal(): wake up waiting thread, if one exists. Otherwise, it does nothing.
//      3. Broadcast(): wake up all waiting threads
// • Rule: thread must hold the lock when doing condition variable operations.



// Mesa versus Hoare Monitors
// What should happen when signal() is called?
//      – No waiting threads => the signaler continues and the signal is effectively lost
//        (unlike what happens with semaphores).
//      – If there is a waiting thread, one of the threads starts executing, others must wait
// • Mesa-style: (Nachos, Java, and most real operating systems)
//     – The thread that signals keeps the lock (and thus the processor).
//     – The waiting thread waits for the lock to be released. When you are woken up by a signal,
//        you are not explicitly given the lock (not guaranteed to get the lock, some other thread may get it
//         before you)
// • Hoare-style: (most textbooks)
//      – The thread that signals gives up the lock and the waiting thread gets the lock.
//      – When the thread that was waiting and is now executing exits or waits again,
//         it releases the lock back to the signaling thread. You are explicitly given the lock when
//         you are woken up by a signal.




// Monitors: A Formal Definition

// • A Monitor defines a lock and zero or more condition variables for managing concurrent access to shared data.
//      – The monitor uses the lock to insure that only a single thread is active in the monitor at any instance.
//      – The lock also provides mutual exclusion for shared data.
//      – Condition variables enable threads to go to sleep inside of critical sections, by releasing
//        their lock at the same time it puts the thread to sleep.

// • Monitor operations:
//     – Encapsulates the shared data you want to protect.
//     – Acquires the mutex at the start.
//     – Operates on the shared data.
//     – Temporarily releases the mutex if it can't complete.
//     – Reacquires the mutex when it can continue.
//     – Releases the mutex at the end.