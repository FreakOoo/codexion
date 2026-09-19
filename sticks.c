#include "codexion.h"

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

static int	lock_same_stick(t_person *coder, t_stick *first)
{
	if (!tlock_or_die(&first->lock, coder->config))
		return (0);
	print_dongle(coder);
	coder->held_sticks[0] = first;
	coder->held_count = 1;
	return (1);
}

static int	lock_two_sticks(t_person *coder, t_stick *first, t_stick *second)
{
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

int	lock_sticks(t_person *coder)
{
	if (coder->left_stick == coder->right_stick)
		return (lock_same_stick(coder, coder->left_stick));
	return (lock_two_sticks(coder, coder->left_stick, coder->right_stick));
}
