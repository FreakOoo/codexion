#include "codexion.h"

// rechecks the death flag every ~1ms instead of sleeping the full duration
// in one shot, so an in-progress action aborts quickly on a burnout
static void	busy_sleep(t_config *config, int *action)
{
	long	leftover;
	long	step;

	leftover = (long)*action * 1000;
	while (leftover > 0 && !is_dead(config))
	{
		step = 1000;
		if (leftover < step)
			step = leftover;
		usleep(step);
		leftover -= step;
	}
}

void	*compile(t_person *coder)
{
	pthread_mutex_lock(&coder->lock);
	busy_sleep(coder->config, &coder->config->time_to_compile);
	if (is_dead(coder->config))
	{
		pthread_mutex_unlock(&coder->lock);
		return (NULL);
	}
	coder->compiles++;
	coder->last_compile = mytime() - coder->config->start;
	pthread_mutex_unlock(&coder->lock);
	pthread_mutex_lock(&coder->config->print_lock);
	if (!is_dead(coder->config))
	{
		printf("%ldms coder %d has compiled (compile # %d )\n",
			mytime() - coder->config->start, coder->name, coder->compiles);
	}
	pthread_mutex_unlock(&coder->config->print_lock);
	return (NULL);
}
void *debug(t_person *coder)
{
  pthread_mutex_lock(&coder->lock);
  busy_sleep(coder->config,&coder->config->time_to_debug);
  if (is_dead(coder->config))
  {
    pthread_mutex_unlock(&coder->lock);
    return  (NULL);
  }
  pthread_mutex_unlock(&coder->lock);
  pthread_mutex_lock(&coder->config->print_lock);
  if(!is_dead(coder->config))
  {
    printf("%ldms coder %d is debugging\n", mytime() - coder->config->start, coder->name);
  }
  pthread_mutex_unlock(&coder->config->print_lock);
  return (NULL);
}

void *refactor(t_person *coder)
{
pthread_mutex_lock(&coder->lock);
  busy_sleep(coder->config,&coder->config->time_to_refactor);
  if (is_dead(coder->config))
  {
    pthread_mutex_unlock(&coder->lock);
    return  (NULL);
  }
  pthread_mutex_unlock(&coder->lock);
  pthread_mutex_lock(&coder->config->print_lock);
  if(!is_dead(coder->config))
  {
    printf("%ldms coder %d is refactoring\n", mytime() - coder->config->start, coder->name);
  }
  pthread_mutex_unlock(&coder->config->print_lock);
  return (NULL);
}
void	unlock_sticks(t_person *coder)
{
	long	now;

	now = mytime() - coder->config->start;
	coder->left_stick->last_use_timer = now;
	pthread_mutex_unlock(&coder->left_stick->lock);
	if (coder->right_stick != coder->left_stick)
	{
		coder->right_stick->last_use_timer = now;
		pthread_mutex_unlock(&coder->right_stick->lock);
	}
	coder->held_sticks[0] = NULL;
	coder->held_sticks[1] = NULL;
	coder->held_count = 0;
}

void	*coder_routine(void *arg)
{
	t_person	*coder;

	coder = (t_person *)arg;
	while (!is_dead(coder->config)
		&& coder->compiles < coder->config->number_of_compiles_required)
	{
		if (!lock_sticks(coder))
			break ;
		compile(coder);
		unlock_sticks(coder);
    debug(coder);
    refactor(coder);
	}
	return (NULL);
}
