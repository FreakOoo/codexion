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

static int	lock_same_stick(t_person *coder, t_stick *first)
{
	if (!acquire(first, coder))
		return (0);
	print_dongle(coder);
	coder->held_sticks[0] = first;
	coder->held_count = 1;
	while (!is_dead(coder->config))
		usleep(1000);
	return (0);
}

static int	lock_two_sticks(t_person *coder, t_stick *first, t_stick *second)
{
	if (first > second)
	{
		first = coder->right_stick;
		second = coder->left_stick;
	}
	if (!acquire(first, coder))
		return (0);
	print_dongle(coder);
	if (!acquire(second, coder))
	{
		release(first);
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
