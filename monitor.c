#include "codexion.h"

void* monitor(char * coders)
{
  int live;
  live = 1;

  while(live)
  {
    int i;
    i = 0;
    while(coders[i])
    {
      //lock
      //read last compile
      //use that to see if we hit deadline
      //if past deadline, print death message
      //also need to lock coders to avoid race condition
      //reading monitor without lock causes a race condition
    }
    //sleep needs to be finer
    sleep(1);
  }

}
