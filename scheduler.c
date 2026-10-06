#include "codexion.h"

t_person compare(t_person a, t_person b)
{
  // need to add a dhoclose to death in config
  // but this returns coder or next coder
  //
  // it should be coder or other coder trying to get the key at the 
  // same time.
  if(a->config->time_to_burnout < b->config->time_to_burnout)
    return a;
  else
    return b;
}

void	*scheduler(void *arg)
{
  if(scheduler == EDF)
  {
    //edf logic
  }
  else if (scheduler == FIFO)
  {
    //fifo logic
  }

}

