#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 16

sem_t semaphore;

void* routine(void* args) {
    printf("(%d) Waiting in the login queue\n", *(int*)args);

    // sem_wait() decrements the semaphore value by 1 if the value is greater than 0
    // If the value is 0, the calling thread will be blocked until the semaphore value is greater than 0
    // sem_wait() is a blocking call if the semaphore value is 0 and will not return until
    // the semaphore value is greater than 0
    sem_wait(&semaphore);
    printf("(%d) Logged in\n", *(int*)args);
    sleep(rand() % 5 + 1);
    printf("(%d) Logged out\n", *(int*)args);
    sem_post(&semaphore); // sem_post() increments the semaphore value by 1
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];

    // 0 if the semaphore is shared between threads of the same process,
    // 1 if shared between processes
    // 0 if we only have one process
    sem_init(&semaphore, 0, 32);
    // sem_init(&semaphore, 0, 1); // Binary semaphore (can have values 0 or 1 only)
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}




// Difference between binary semaphores and mutex:



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

pthread_mutex_t mutexFuel;
sem_t semFuel;
int fuel = 50;

void* routine1(void* args) {
    while (1) {
        pthread_mutex_lock(&mutexFuel);

        // sem_wait(&semFuel);
        fuel += 50;
        printf("Current value is %d\n", fuel);
    }
}

void* routine2(void* args) {
    while (1) {
        // Not allowed. We can't lock a mutex in one thread and unlock it in another.
        // Lock and unlock must be in the same thread.
        pthread_mutex_unlock(&mutexFuel);


        // Allowed. We can wait on a semaphore in one thread and post on it in another.
        // sem_post(&semFuel);

        usleep(5000);
    }
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    sem_init(&semFuel, 0, 1);
    pthread_mutex_init(&mutexFuel, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(&th[i], NULL, &routine1, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &routine2, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&semFuel);
    return 0;
}




// Implementing Signal and Wait


/*
class Semaphore {
  public:
    void Wait(Process P);
    void Signal();

  private:
    int value;
    Queue Q; // queue of processes;

};


Semaphore(int val) {
  value = val;
  Q = empty;
};



void Semaphore::Wait(Process P) {
    value = value - 1;
    if (value < 0) {
        add P to Q;
        P->block();
    }
}

void Semaphore::Signal() {
  value = value + 1;
  if (value <= 0){
    remove P from Q;
    wakeup(P);
  }
}


*/
