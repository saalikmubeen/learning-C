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