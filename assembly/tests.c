#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void do_nothing();

#define BILLION 1000000000

int main()
{
  struct timespec start, end;
  int i,j;
  double accum;

  j = clock_gettime(CLOCK_REALTIME,&start);
  for(i = 0; i < 30000; i++)
    ;

  j = clock_gettime(CLOCK_REALTIME,&end);

  accum =((double)end.tv_nsec - (double)start.tv_nsec)/BILLION + ((double)end.tv_sec-(double)start.tv_sec);
  printf("empty loop: %lf ns\n",accum);

  j = clock_gettime(CLOCK_REALTIME,&start);
  for(i = 0; i < 30000; i++)
    {
      do_nothing();
    }

  j = clock_gettime(CLOCK_REALTIME,&end);
  accum =((double)end.tv_nsec - (double)start.tv_nsec)/BILLION + ((double)end.tv_sec-(double)start.tv_sec);
  printf("empty fcn: %lf ns\n",accum);

  j = clock_gettime(CLOCK_REALTIME,&start);
  for(i = 0; i < 30000; i++)
    {
      getuid();
    }

  j = clock_gettime(CLOCK_REALTIME,&end);
  accum =((double)end.tv_nsec - (double)start.tv_nsec)/BILLION + ((double)end.tv_sec-(double)start.tv_sec);
  printf("system call: %lf ns\n",accum);



}

void do_nothing()
{
  ;
}


