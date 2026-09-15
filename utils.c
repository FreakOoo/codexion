#include "codexion.h"

long mytime(void)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000L + tv.tv_usec/1000);
}


int is_dead(t_config * config)
{
  int dead;
  pthread_mutex_lock(&config->dead_lock);
  dead = config->dead;

  pthread_mutex_unlock(&config->dead_lock);

    return (dead);
}
