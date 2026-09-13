#include "codexion.h"

void* monitor(t_person * coders,int NOC, int TTB)
{
  int live;
  live = 1;

  while(live)
  {
    while(coders->name <= NOC)
    {
      //ai'm locking wrong, idea is right, thought need to pass NOC in.
      pthread_mutex_lock(coders->name);
      //lock
      t_person guy = coders[i];
      if(guy->last_compile >= TTB)
      {
        printf("coder %d is burnt out", coder->name);
        exit(1);
      }
      //read last compile
      //use that to see if we hit deadline
      //if past deadline, print death message
      //also need to lock coders to avoid race condition
      //reading monitor without lock causes a race condition
    }
    //sleep needs to be finer
    sleep(0.1);
  }

}
