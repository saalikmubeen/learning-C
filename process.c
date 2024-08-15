#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h> // waitpid
#include <sys/types.h>

void main(int argc,char *argv[]) {

	char buffer[80];
	int r;

  // fork() returns in the child process and the child process is a copy of the parent process
  // The child process has a different process ID from the parent process
  // fork() returns the process ID ( > 1) of the child process in the parent process
  // fork() returns -1 in the parent process if the fork() fails

  // Inside parent process - child process id - it's going to be different than getpid()
  // Inside child process - 0 - it's going to be different than getpid()

	r = fork();

	if (r == 0) { // I am the child
		sprintf(buffer,"I am the Child with pid: %d\n",getpid());
		write(1,buffer,strlen(buffer));

    sprintf(buffer, "My parent is: %d\n", getppid());
    write(1,buffer,strlen(buffer));
	}
	else if (r==-1) {
    printf("Error while calling fork in the parent process\n");
  }

	else { // I am the parent
		sprintf(buffer,"I am the Parent with pid: %d\n",getpid());
    // 1 is the file descriptor for standard output
		write(1,buffer,strlen(buffer));

    sprintf(buffer,"My child's pid is: %d\n",r);
    write(1,buffer,strlen(buffer));
	}


}














void main() {

	char buffer[80];
	sprintf(buffer,"I am the process: %d\n",getpid());
	write(1,buffer,strlen(buffer));


  // execlp() replaces the current process image with a new process image.
  // The new process image is obtained from the file specified by the file argument.
  // The process remains the same, no new process is created. only the process image is replaced.
	execlp("./second","second",(char *) 0);

  // execlp("ls","ls","-t",(char *) 0);

	// This is not going to be displayed
	strcpy(buffer,"After the exec\n");
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
        int err = execlp("ping", "ping", "-c", "3", "google.com", NULL);
        if (err == -1) {
            printf("Could not find program to execute or other error ocurred\n");
            return 2;
        }
    } else {
        int wstatus;
        wait(&wstatus); // Wait for the child process to finish and get the exit status

        if (WIFEXITED(wstatus)) { // Check if the child process exited normally (not killed by a signal)
            int statusCode = WEXITSTATUS(wstatus); // Get the exit status of the child process
            if (statusCode == 0) {
                printf("Success\n");
            } else {
                printf("Failure with status code %d\n", statusCode);
            }
        }
    }

    return 0;
}




// Redirecting the standard output of a process:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/fcntl.h>


// File descriptors:
// file_descriptor:  file
// 0:                STDIN
// 1:                STDOUT
// 2:                STDERR

// int file = open("file.txt", O_WRONLY | O_CREAT, 0777);

// file:             (file descriptor of the) file.txt


// int f1 = dup(file) // Duplicate the file descriptor of file.txt

// f1:               also (the file descriptor to the) file.txt


// int f2 = dup2(file, STDOUT_FILENO); // Redirect the standard output to the file.txt
// This will replace the file for the 1(STDOUT_FILENO) with the file.txt(file)
// Now:
// STDOUT_FILENO(1):    file.txt

// f2:               (file descriptor of the) file.txt



int main(int argc, char* argv[]) {
    int pid = fork();
    if (pid == -1) {
        return 1;
    }

    if (pid == 0) {
      // Child process
        int file = open("pingResults.txt", O_WRONLY | O_CREAT, 0777);
        if (file == -1) {
            return 2;
        }


        printf("The fd to pingResults: %d\n", file);
        dup2(file, STDOUT_FILENO); // Redirects the standard output of the child process to the pingResults.txt file.
        close(file); // Close the file descriptor after redirecting the standard output
        // file and STDOUT_FILENO both now point to the same file i.e pingResults.txt


        int err = execlp("ping", "ping", "-c", "3", "google.com", NULL);
        if (err == -1) {
            printf("Could not find program to execute or other error ocurred\n");
            return 2;
        }
    } else {

        // Parent process
        int wstatus;
        wait(&wstatus);

        if (WIFEXITED(wstatus)) {
            int statusCode = WEXITSTATUS(wstatus);
            if (statusCode == 0) {
                printf("Success\n");
            } else {
                printf("Failure with status code %d\n", statusCode);
            }
        }
    }

    return 0;
}












// * wait() system call
// When a child process finsihes and terminates (exit(0) say), the patrent process is notified
// by the kernel. The parent process receives a SIGCHLD signal. The parent process can then
// call wait() to get the exit status of the child process. The wait() system call blocks the parent
// process until the child process terminates. The wait() system call returns the process ID of the
// child process that terminated. If the parent process has multiple child processes, it can call
// wait() multiple times to get the exit status of each child process. The wait() system call is
// used to prevent the parent process from terminating before the child process. The cild process
// becomes a zombie process if the parent process does not call wait() to get the exit status of the
// child process. A zombie process is a process that has terminated but whose exit status has not been
// collected by the parent process. The kernel keeps track of the exit status of the zombie process.
// So the child process remains in the ZOMBIE state until the parent process calls wait() to collect
// the exit status of the child process. If the parent process terminates before the child process,
// the child process becomes an orphan process and is adopted by the init process. The init process
// is the first process that is started by the kernel when the system boots. The init process
// has a process ID of 1. The init process is the parent of all orphan processes.


// *waitpid() system call
// The waitpid() system call is similar to the wait() system call. The waitpid() system call
// blocks the parent process until the child process terminates. The waitpid() system call
// takes three arguments: the process ID of the child process, a pointer to an integer to store
// the exit status of the child process, and a set of options. The waitpid() system call can be
// used to wait for a specific child process to terminate. The waitpid() system call can also be
// used to wait for any one of the child process to terminate. The waitpid() system call
// can also be used to wait for all child processes to terminate.
// waitpid(-1, &status, 0) // wait for any child process to terminate
// waitpid(pid, &status, 0) // wait for a specific child process to terminate

// waitpid(pid, &status, WNOHANG) // non-blocking wait
// waitpid(pid, &status, 0) // blocking wait

// waitpid on success returns the process ID of the child process that terminated
// On failure, it returns -1



void main() {

  int num_processes = 5;
  int ret;


  // create multiple child processes and
  // executing them in sequential scheme or order.
  for (int i = 0; i < num_processes; i++) {
    ret = fork();
    if (ret == 0) {
      // child process
      printf("I am the child with pid: %d\n", getpid());
      sleep(5);
      exit(0); // exit the child process
    }

    if (ret == -1) {
      printf("Error while calling fork in the parent process\n");
    }

    if (ret > 0) {
      // parent process
      printf("I am the parent with pid: %d\n", getpid());

      // wait for the child process to terminate

      waitpid(ret, NULL, 0);
    }
  }







  // create multiple child processes and
  // executing them in parallel scheme or order.
  for (int i = 0; i < num_processes; i++) {
    ret = fork();
    if (ret == 0) {
      // child process
      printf("I am the child with pid: %d\n", getpid());
      sleep(5);
      exit(0); // exit the child process
    }

    if (ret == -1) {
      printf("Error while calling fork in the parent process\n");
    }
  }


  // wait for all the child processes to terminate
  // for (int i = 0; i < num_processes; i++) {
  //   wait(NULL);
  // }

  while(waitpid(-1, NULL, 0) > 0) {
    // wait for any child process to terminate
  }

}








// Communicating between parent and child processes using pipes


// When you call pipe(fd), the operating system creates a unidirectional data channel
// (the pipe) and associates two file descriptors with it: fd[0] for reading and fd[1] for writing.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int fd[2]; // file descriptors
    // fd[0] is the read end of the pipe
    // fd[1] is the write end of the pipe

    // fd[0]: This is the file descriptor for the read end of the pipe.
    //        The process can read data from the pipe through this descriptor.
    // fd[1]: This is the file descriptor for the write end of the pipe.
    //        The process can write data into the pipe through this descriptor.

    if (pipe(fd) == -1) {
        printf("An error ocurred with opening the pipe\n");
        return 1;
    }

    int id = fork();
    if (id == -1) {
        printf("An error ocurred with fork\n");
        return 2;
    }

    if (id == 0) {
        // Child process
        close(fd[0]);  // Close the read end of the pipe
        int x;
        printf("Input a number: ");
        scanf("%d", &x);
        printf("Sleeping for 10 seconds");
        sleep(10);
        if (write(fd[1], &x, sizeof(int)) == -1) {
            printf("An error ocurred with writing to the pipe\n");
            return 3;
        }
        close(fd[1]);  // Close the write end after writing
    } else {
        // Parent process
        close(fd[1]);  // Close the write end of the pipe
        int y;
        printf("Waiting for child process to write to the pipe\n");
        if (read(fd[0], &y, sizeof(int)) == -1) {
            printf("An error ocurred with reading from the pipe\n");
            return 4;
        }
        printf("Got from child process %d\n", y);
        y = y * 3;
        printf("Result is %d\n", y);
        close(fd[0]);  // Close the read end after reading
    }

    return 0;
}






// Named pipes (FIFOs)


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    // 0777 means that the file is readable, writable, and executable by everyone.
    // This will create a FIFO file named myfifo1 in the current directory.
    if (mkfifo("myfifo1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    printf("Opening...\n");

    // This will block until the other end of the pipe is opened by some
    // other process to read from it.
    int fd = open("myfifo1", O_WRONLY);
    printf("Opened\n");
    int x = 97;
    if (write(fd, &x, sizeof(x)) == -1) {
        return 2;
    }

    printf("Written\n");
    close(fd);
    printf("Closed\n");







    int arr[5];
    srand(time(NULL));
    int i;
    for (i = 0; i < 5; i++) {
        arr[i] = rand() % 100;
        printf("Generated %d\n", arr[i]);
    }

    // NOTE: You will have to create the fifo using the command "mkfifo sum"
    int fd = open("sum", O_WRONLY);
    if (fd == -1) {
        return 1;
    }

    if (write(fd, arr, sizeof(int) * 5) == -1) {
        return 2;
    }
    // OR
    // for (i = 0; i < 5; i++) {
    //     if (write(fd, &arr[i], sizeof(int)) == -1) {
    //         return 2;
    //     }
    // }
    close(fd);


    // To read from it in another process:
    // int fd = open("sum", O_RDONLY);
    // if (fd == -1) {
    //     return 1;
    // }
    // int arr[5];
    // if (read(fd, arr, sizeof(int) * 5) == -1) {
    //     return 2;
    // }
    // OR
    // for (i = 0; i < 5; i++) {
    //     if (read(fd, &arr[i], sizeof(int)) == -1) {
    //         return 2;
    //     }
    // }
    // close(fd);


    return 0;
}





// Two way communication between parent and child processes using pipes:

// Using the same pipe for both reading and writing in a bidirectional communication
// scenario between two processes is not typically feasible due to the nature of pipes and
// how they are designed to work.

// Explanation:
//     Pipes Are Unidirectional:
//       A pipe in Unix-like operating systems is inherently unidirectional.
//       It has a read end and a write end. Data written to the write end can be read from the read end.
//       However, a single pipe cannot be used for both reading and writing simultaneously in the
//       same process or between two processes.

//     Full-Duplex Communication Requires Two Pipes:
//       To achieve full-duplex communication (where both processes can send and receive data),
//       you need two pipes:
//           One pipe for communication from the parent to the child.
//           Another pipe for communication from the child to the parent.

// What Happens If You Use One Pipe for Both:
//     Race Conditions:
//       If you use a single pipe for both reading and writing, it can lead to race conditions,
//       deadlocks, and unpredictable behavior. For instance, if both processes attempt to
//       write to and read from the same pipe simultaneously, they could block each other.

//     Ambiguity in Data Direction:
//       The pipe doesn't inherently distinguish between data flowing in different directions.
//       Using the same pipe could result in a process reading its own data, causing confusion.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) {
    int p1[2]; // Parent to child (parent will write, child will read)
    int p2[2]; // Child to parent (child will write, parent will read)

    if (pipe(p1) == -1) {
        return 1;
    }
    if (pipe(p2) == -1) {
        return 2;
    }
    int pid = fork();

    if (pid == -1) {
        return 3;
    }

    if (pid == 0) {
        // Child process
        // Child process will read the data from the parent process using p1
        // and write the result back to the parent process using p2
        close(p1[1]); // Close the write end of p1
        close(p2[0]); // Close the read end of p2

        int x;
        if (read(p1[0], &x, sizeof(int)) == -1) {
            return 3;
        }
        printf("Received %d\n", x);

        x *= 4;

        if (write(p2[1], &x, sizeof(int)) == -1) {
            return 4;
        }
        printf("Wrote %d\n", x);
        close(p1[0]); // Close the read end of p1 after reading is done
        close(p2[1]); // Close the write end of p2 after writing is done
    } else {
        // Parent process
        close(p1[0]); // Close the read end of p1
        close(p2[1]); // Close the write end of p2

        srand(time(NULL));
        int y = rand() % 10;


        if (write(p1[1], &y, sizeof(y)) == -1) {
            return 5;
        }
        printf("Wrote %d\n", y);
        if (read(p2[0], &y, sizeof(y)) == -1) {
            return 6;
        }
        printf("Result is %d\n", y);

        close(p1[1]); // Close the write end of p1 after writing is done
        close(p2[0]); // Close the read end of p2 after reading is done
        wait(NULL);
    }

    return 0;
}