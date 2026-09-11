#include "codexion.h"

//maybe it takes in coders as arguments to check their times
//feeding in the array of t_persons

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
      //
      //if past deadline, print death message
    }
    //stuff here 
    sleep(1);
  }

}
