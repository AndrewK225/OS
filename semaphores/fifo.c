#include <stdio.h>
#include "fifo.h"
#include "sem.h"


void fifo_init(struct fifo *f)
{
  f -> rstart = 0;
  f -> wstart = 0;

  sem_init(&f -> buf,1024);
  sem_init(&f -> data,0);
  mutex_init(&(f -> rdwrlock));
}


void fifo_wr(struct fifo *f, unsigned long data)
{

  char check;
  check = 0;
  while (check == 0)
    {
      mutex_lock(&(f -> rdwrlock));
      if(sem_try(&(f -> buf)) == 0)  //if op not possible
	{
	  mutex_unlock(&(f -> rdwrlock));
	  check = 0;  //retry 
	}
      else
	{
	  f -> pipe[f -> wstart++] = data;
	  f -> wstart %= 1024;
	  sem_inc(&f -> data);
	  mutex_unlock(&(f -> rdwrlock));
	  check = 1; //exit the while
	}	
    }
}


unsigned long fifo_rd(struct fifo *f)
{
  unsigned long data;
 retry:

  mutex_lock(&(f -> rdwrlock));
      if(sem_try(&(f -> data)) == 0)
	{
	  mutex_unlock(&(f -> rdwrlock));
	  goto retry;  //retry
	}
      
	  data = f -> pipe[f -> rstart++];
	  f -> rstart %= 1024;
	  sem_inc(&(f -> buf));
	  mutex_unlock(&(f -> rdwrlock));
	
    
  return data;
}
