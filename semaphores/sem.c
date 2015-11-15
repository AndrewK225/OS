#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "sem.h"


//Miraj suggested the use of separate mutex fcns
//these operations help make the P and V ops atomic

void mutex_init(struct mutex *mut)
{
  mut -> lock = 0;
  mut -> waiter = 0;
}

void mutex_lock(struct mutex *mut)
{
  while(tas(&(mut -> lock)))  
    {
      sigset_t mask;
      mut -> proc_q[(mut -> waiter++)] = getpid(); // keeps track of the waiting procs
      
      sigfillset(&mask);
      sigdelset(&mask,SIGUSR1);
      sigdelset(&mask,SIGINT);
      sigsuspend(&mask);

    }
}

void mutex_unlock(struct mutex *mut)
{
  mut -> lock = 0;
  if(mut -> waiter > 0)
    {
      kill(mut -> proc_q[--(mut -> waiter)], SIGUSR1);
    }
}



void sem_init(struct sem *s, int count)
{
  s -> value = count;
  s -> waiter = 0;
  mutex_init(&s -> locks);
}

int sem_try(struct sem *s)
{
  mutex_lock(&s -> locks);  //lock

  if((s -> value) == 0)   //is P op possible? (cannot be neg)
    {
      mutex_unlock(&s -> locks);
      return 0;
    }
  s -> value--;
  mutex_unlock(&s -> locks);  
  return 1;
}

void sem_inc(struct sem *s) //v op
{
  mutex_lock(&s -> locks);
  s -> value++;
  if(s -> waiter > 0)
    {
      kill(s -> proc_q[--(s -> waiter)],SIGUSR1);
    }
  mutex_unlock(&s -> locks);
}

void sem_wait(struct sem *s)
{
  sigset_t mask;
  mutex_lock(&s -> locks);
  
  if(s -> value == 0) // cannot do the P op
    {
      s -> proc_q[s -> waiter++] = getpid();
      sigfillset(&mask);
      sigdelset(&mask, SIGUSR1);
      sigdelset(&mask,SIGINT);
      sigsuspend(&mask);
    }
  
  s -> value--;

  mutex_unlock(&s -> locks);

}
