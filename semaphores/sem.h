#ifndef SEM_H_
#define SEM_H_


int tas(volatile char* lock);  //provided code

struct mutex
{
  char lock; //can be 1 or 0
  char waiter;
  int proc_q[64];
};
//some of the following functions will make use of tas
void mutex_init(struct mutex *mut);
void mutex_lock(struct mutex *mut);
void mutex_check(struct mutex *mut);
void mutex_unlock(struct mutex *mut);


struct sem
{
  int value;
  int waiter;
  struct mutex locks;
  int proc_q[64];  
};


void sem_init(struct sem* s, int count);
int sem_try(struct sem *s);
void sem_wait(struct sem *s);
void sem_inc(struct sem *s);


#endif
