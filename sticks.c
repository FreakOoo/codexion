#include "codexion.h"

static void	print_dongle(t_person *coder)
{
	pthread_mutex_lock(&coder->config->print_lock);
	if (!is_dead(coder->config))
	{
		printf("%ldms %d has taken a dongle\n",
			mytime() - coder->config->start, coder->name);
	}
	pthread_mutex_unlock(&coder->config->print_lock);
}

// gives up (returns 0) the moment the death flag flips, instead of
// staying stuck on a mutex a dead coder may never release; a stick that's
// free but still within its cooldown window is put right back and treated
// as busy, same as one another coder is holding
static int	tlock_or_die(t_stick *stick, t_config *config)
{
	long	now;

	while (!is_dead(config))
	{
		if (pthread_mutex_trylock(&stick->lock) == 0)
		{
			now = mytime() - config->start;
			if (now - stick->last_use_timer >= config->dongle_cooldown)
				return (1);
			pthread_mutex_unlock(&stick->lock);
		}
		usleep(200);
	}
	return (0);
}

static int	lock_same_stick(t_person *coder, t_stick *first)
{
	if (!tlock_or_die(first, coder->config))
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
	if (!tlock_or_die(first, coder->config))
		return (0);
	print_dongle(coder);
	if (!tlock_or_die(second, coder->config))
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
