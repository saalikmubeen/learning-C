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

    // if fuel in the tank is less than 40, wait for it to be filled at least to 40
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");
        // Make the thread wait on a condition (until the fuel is filled) and give up the mutex(lock)

        // The condition itself is not contained in the condition variable but contained in the while loop itself.
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
            // this thread will fill the fuel in the tank
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            // this thread will represent a car that needs fuel and draws fuel from the tank
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







// Key points in below simulation:
// Looping forever: both producer(filler) and consumers(cars) keep looping.
// Condition variables:
//   Cars wait if fuel < 40.
//   Filler waits if fuel >= 100(arbitrary tank max).
// Broadcast vs signal: filler uses pthread_cond_broadcast since multiple cars might be waiting.
//                      Cars only signal in case the filler should wake up.
// Randomized driving delay: cars sleep for 1–3 seconds between refuels to stagger requests.
// This now behaves like a continuous producer–consumer simulation.



pthread_mutex_t mutexFuel2;
pthread_cond_t condFuel2;

int fuel2 = 0;

void *fuel_filling2(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&mutexFuel2);
    while (fuel2 >= 100)
    {
      // Tank is full enough, no need to fill right now
      pthread_cond_wait(&condFuel2, &mutexFuel2);
    }
    fuel2 += 30;
    printf("[Filler] Added 30 fuel. Tank = %d\n", fuel2);
    pthread_cond_broadcast(&condFuel2); // wake up all waiting cars
    pthread_mutex_unlock(&mutexFuel2);

    sleep(1); // simulate time to fetch and pour fuel
  }
  return NULL;
}

void *car2(void *arg)
{
  int id = *(int *)arg;
  free(arg);

  while (1)
  {
    pthread_mutex_lock(&mutexFuel2);
    while (fuel2 < 40)
    {
      printf("[Car %d] Not enough fuel. Waiting...\n", id);
      pthread_cond_wait(&condFuel2, &mutexFuel2);
    }
    fuel2 -= 40;
    printf("[Car %d] Took 40 fuel. Tank left = %d\n", id, fuel2);
    pthread_cond_signal(&condFuel2); // signal filler in case tank is too low
    pthread_mutex_unlock(&mutexFuel2);

    sleep(rand() % 3 + 1); // simulate drive time before refueling again
  }
  return NULL;
}

int main()
{
  srand(time(NULL));
  pthread_t filler;
  pthread_t cars[3];

  pthread_mutex_init(&mutexFuel2, NULL);
  pthread_cond_init(&condFuel2, NULL);

  // start fuel filler
  if (pthread_create(&filler, NULL, &fuel_filling2, NULL) != 0)
  {
    perror("Failed to create filler thread");
    return 1;
  }

  // start multiple cars
  for (int i = 0; i < 3; i++)
  {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    if (pthread_create(&cars[i], NULL, &car2, id) != 0)
    {
      perror("Failed to create car thread");
      return 1;
    }
  }

  // let simulation run forever
  pthread_join(filler, NULL);
  for (int i = 0; i < 3; i++)
  {
    pthread_join(cars[i], NULL);
  }

  pthread_mutex_destroy(&mutexFuel2);
  pthread_cond_destroy(&condFuel2);
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