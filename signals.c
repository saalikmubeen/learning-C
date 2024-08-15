#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>



// SIGUSR1 and SIGUSR2 are user-defined signals that can be used by the programmer
// to handle specific events in the program. These signals are not used by the system
// and can be used for any purpose by the programmer.

void f_sigint(int s) {
    char buffer[128];
    sprintf(buffer,"Welcome to the island!\n");
    write(1,buffer,strlen(buffer));
    exit(0);
}


void main() {

    char buffer[128];
    struct sigaction manage;


    // the mask defines a set of signals that should be blocked
    // (i.e., temporarily ignored) while the signal handler is executing.
    // The mask is useful when you want to ensure that certain signals are not
    // delivered to the process while it is handling another signal.
    // For example, if you wanted to prevent the process from receiving other signals
    // like SIGTERM while handling SIGINT, you would add SIGTERM to the mask.

    // sigset_t is a data type used to represent a signal set,
    // which is essentially a collection of signals.
    sigset_t mask;
    sigemptyset(&mask);
    manage.sa_mask = mask;

    // The sa_flags field in the sigaction structure is used to modify the
    // behavior of the signal handler. It allows you to specify certain options
    // or flags that change how the signal is handled by the program
    manage.sa_flags = 0; // 0 means no flags (default behavior of the signal handler)


    // The sa_handler field in the sigaction structure is a pointer to the
    // signal handler function that should be called when the signal is received.
    manage.sa_handler = f_sigint;


    // sigaction() is a system call used to change the management of a signal.
    // i.e it is used to specify how a signal should be handled by the process.
    // sigaction(signal_number, &new_behaviour, &old_behaviour)
    // Note that the default management/behavior of the following signals cannot be changed:
    // SIGKILL, SIGSTOP, SIGCONT
    // Interrupt from the keyboard (Ctrl+C) can be used to send SIGINT signal to the process
    // and the default action of the process is to terminate/end the process.
    sigaction(SIGINT, &manage, NULL);
    // when the process receives SIGINT
    // it will execute f_sigint

    // OR:
    // signal(SIGINT, f_sigint); // signal() is an older way to handle signals

    /*while(1) {
        sprintf(buffer,"Doing something\n");
        write(1,buffer,strlen(buffer));
    }*/

    sprintf(buffer,"I am sleeping\n");
    write(1,buffer,strlen(buffer));
    sleep(15);


    // kill doesn't kill the process or terminates any process.
    // It's just a way to send a signal to a process.
    kill(getpid(), SIGINT); // send SIGINT signal to the process (self)

    // We won't reach this point because the process will terminate (exit(0) in f_sigint)
    // when it receives the SIGINT signal
    sprintf(buffer,"Message after SIGINT\n");
    write(1,buffer,strlen(buffer));
}








#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (pid == 0) {
        while (1) {
            printf("Some output\n");
            usleep(50000);
        }
    } else {
        kill(pid, SIGSTOP);
        int t;
        do {
            printf("Time in seconds for execution: ");
            scanf("%d", &t);

            if (t > 0) {
                kill(pid, SIGCONT);
                sleep(t);
                kill(pid, SIGSTOP); // kill(pid, SIGTSTP); // SIGTSTP stops and sends the process to the background
            }
        } while (t > 0);

        kill(pid, SIGKILL);
        wait(NULL);
    }

    return 0;
}