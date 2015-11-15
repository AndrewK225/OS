/*
  Andrew Koe
  PS 7
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include "fifo.h"


void signal_handler(int signum) 
{
  ;
}

int main()
{
  int state;
  int child;
  int fd,i,j;
  unsigned long buffer[128];
  struct fifo *afifo;
  struct sigaction act;

  //Miraj helped me with the signal code below
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = signal_handler;

  if(sigaction(SIGUSR1, &act, NULL))
    {
      fprintf(stderr, "Error with sigaction: %s\n",strerror(errno));
      exit(-1);
    }

  if((afifo = mmap(0,sizeof(struct fifo), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON,-1,0)) == MAP_FAILED)
    {
      fprintf(stderr,"Error mappiing region: %s\n", strerror(errno));
      exit(-1);
    }

  fifo_init(afifo);
  child = fork();
  
  switch (child) {
  case -1:
    fprintf(stderr,"error with fork: %s\n",strerror(errno));
    break;

  case 0:
     printf("in the child\n");
    for(j = 0; j < 128; j++)
      {
	buffer[j] = fifo_rd(afifo);
	printf("%lu\n",buffer[j]);
      }
    break;

  default:
    for(i = 0; i < 1000;i++)
      {
	fifo_wr(afifo,i);
      }
          
    break;

  }

  if(munmap(afifo,sizeof(struct fifo)) < 0)
    {
      fprintf(stderr,"munmap error: %s\n",strerror(errno));
    }
  return 0;
}
