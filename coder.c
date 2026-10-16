#include "codexion.h"

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
	coder->last_compile = mytime() - coder->config->start;
	pthread_mutex_unlock(&coder->lock);
	log_state(coder, "is compiling");
	busy_sleep(coder->config, &coder->config->time_to_compile);
	if (is_dead(coder->config))
		return (NULL);
	pthread_mutex_lock(&coder->lock);
	coder->compiles++;
	pthread_mutex_unlock(&coder->lock);
	return (NULL);
}

static void	timed_action(t_person *coder, int duration, char *msg)
{
	log_state(coder, msg);
	busy_sleep(coder->config, &duration);
}

void	*debug(t_person *coder)
{
	timed_action(coder, coder->config->time_to_debug, "is debugging");
	return (NULL);
}

void	*refactor(t_person *coder)
{
	timed_action(coder, coder->config->time_to_refactor, "is refactoring");
	return (NULL);
}

void	unlock_sticks(t_person *coder)
{
	release(coder->left_stick, coder->config);
	if (coder->right_stick != coder->left_stick)
		release(coder->right_stick, coder->config);
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
