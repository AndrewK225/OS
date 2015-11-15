#ifndef _FIFO_H
#define _FIFO_H

#include "sem.h"

struct fifo
{
  unsigned long pipe[1024];
  int rstart, wstart;
  struct sem data;
  struct sem buf;

  struct mutex rdwrlock;
};

void fifo_init(struct fifo *f);
void fifo_wr(struct fifo *f, unsigned long data);
unsigned long fifo_rd(struct fifo *f);

#endif

