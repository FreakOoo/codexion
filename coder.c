
#include "codexion.h"

// sleeps out time_to_compile in small chunks, rechecking the death flag
// between each one so an in-progress compile aborts within ~1ms of a
// burnout instead of sleeping out the full duration regardless
static void	busy_sleep(t_config *config)
{
	long	leftover;
	long	step;

	leftover = (long)config->time_to_compile * 1000;
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
	busy_sleep(coder->config);
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
		printf("%ld coder %d has compiled (compile # %d )\n",
			mytime() - coder->config->start, coder->name, coder->compiles);
	}
	pthread_mutex_unlock(&coder->config->print_lock);
	return (NULL);
}

static void	print_dongle(t_person *coder)
{
	pthread_mutex_lock(&coder->config->print_lock);
	if (!is_dead(coder->config))
	{
		printf("%ld %d has taken a dongle\n",
			mytime() - coder->config->start, coder->name);
	}
	pthread_mutex_unlock(&coder->config->print_lock);
}

// tries to grab a dongle without blocking forever: gives up (returning 0)
// the moment the death flag flips, instead of staying stuck on a mutex
// held by a coder who may not release it again anytime soon
static int	tlock_or_die(pthread_mutex_t *lock, t_config *config)
{
	while (!is_dead(config))
	{
		if (pthread_mutex_trylock(lock) == 0)
			return (1);
		usleep(200);
	}
	return (0);
}

static int	lock_sticks(t_person *coder)
{
	t_stick	*first;
	t_stick	*second;

	first = coder->left_stick;
	second = coder->right_stick;
	if (first == second)
	{
		if (!tlock_or_die(&first->lock, coder->config))
			return (0);
		print_dongle(coder);
		coder->held_sticks[0] = first;
		coder->held_count = 1;
		return (1);
	}
	if (first > second)
	{
		first = coder->right_stick;
		second = coder->left_stick;
	}
	if (!tlock_or_die(&first->lock, coder->config))
		return (0);
	print_dongle(coder);
	if (!tlock_or_die(&second->lock, coder->config))
	{
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	print_dongle(coder);
	coder->held_sticks[0] = coder->left_stick;
	coder->held_sticks[1] = coder->right_stick;
	coder->held_count = 2;
	return (1);
}

static void	unlock_sticks(t_person *coder)
{
	pthread_mutex_unlock(&coder->left_stick->lock);
	if (coder->right_stick != coder->left_stick)
		pthread_mutex_unlock(&coder->right_stick->lock);
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
	}
	return (NULL);
}

