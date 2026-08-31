#include "codexion.h"


//this file is just to test if the timers work,
//will delete later, might stay useful for testing
//
//
int main(void)
{
long start;
long now;
long elapsed;
long limit;

limit = 100;
start = mytime();
while(1)
{
  now = mytime();
  elapsed = now - start;
  printf("elapsed: %ld ms\n", elapsed);
  if(elapsed >= limit)
  {
    printf("burnout !\n");
    break;
  }
  usleep(10000);
}
return(0);
}
