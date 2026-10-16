#include "codexion.h"

int	acquire(t_stick *stick, t_person *coder)
{
  pthread_mutex_lock(&stick->lock);
  heap_push(stick,create_request(coder,stick));
  while(!my_turn(stick, coder))
  {
    if(is_dead(coder->config))
    {
      pthread_mutex_unlock(&stick->lock);
      return 0;
    }
    wait_turn(stick,coder);
  }
  heap_pop(stick);
  stick->held=1;
  pthread_mutex_unlock(&stick->lock);
  return 1;
}

void	release(t_stick *stick)
{
  mutex_unlock stick 1;
  mutex_unlock stick 2;
	coder->stick = 0;
}

int	my_turn(t_stick *stick, t_person *coder)
{
  t_ask_forstick *ichiban;
  ichiban = heap_peek(stick);

  if(!ichiban || ichiban->coder != coder->name)
    return 0;
  if(stick->held)
    return 0;
  return(mytime() - coder->config->start >=stick->available_at);

}
static void wait_turn(t_stick *stick, t_person *coder)
{
  t_ask_forstick *ichiban;
  struct timespec moment;
  long wake;

  ichiban = heap_peek(stick);
  if(ichiban && top->coder == coder->name && !stick->held))
  {
    wake = coder->config->start + stick->available_at;
    moment.tv_sec = wake / 1000;
    moment.tv_nsec = (wake % 1000) * 1000000;
    pthread_cond_timedwait(&stick->waiting_room, &stick->lock, &moment)
  }
  else 
    pthread_cond_wait(&stick->waiting_room, &stick->lock);

}
