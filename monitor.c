#include "codexion.h"

static int	is_burnt_out(t_person *coder, int ttb)
{
	long	elapsed;

	if (pthread_mutex_trylock(&coder->lock) != 0)
		return (0);
	elapsed = (mytime() - coder->config->start) - coder->last_compile;
	pthread_mutex_unlock(&coder->lock);
	return (elapsed >= ttb);
}

static int	all_done(t_person *coders, int noc)
{
	int	i;
	int	done;

	i = 0;
	while (i < noc)
	{
		if (pthread_mutex_trylock(&coders[i].lock) != 0)
			return (0);
		done = (coders[i].compiles
				>= coders[i].config->number_of_compiles_required);
		pthread_mutex_unlock(&coders[i].lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

static void	wake_all(t_person *coders)
{
	int	i;

	i = 0;
	while (i < coders->config->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].left_stick->lock);
		pthread_cond_broadcast(&coders[i].left_stick->waiting_room);
		pthread_mutex_unlock(&coders[i].left_stick->lock);
		i++;
	}
}

static void	report_burnout(t_person *coder)
{
	int	already_dead;

	pthread_mutex_lock(&coder->config->dead_lock);
	already_dead = coder->config->dead;
	coder->config->dead = 1;
	pthread_mutex_unlock(&coder->config->dead_lock);
	if (already_dead)
		return ;
	pthread_mutex_lock(&coder->config->print_lock);
	printf("%ld %d burned out\n",
		mytime() - coder->config->start, coder->name);
	pthread_mutex_unlock(&coder->config->print_lock);
}

static void	report_success(t_person *coder)
{
	int	already_dead;

	pthread_mutex_lock(&coder->config->dead_lock);
	already_dead = coder->config->dead;
	coder->config->dead = 1;
	if (!already_dead)
		coder->config->finished = 1;
	pthread_mutex_unlock(&coder->config->dead_lock);
}

void	*monitor(void *arg)
{
	t_person	*coders;
	int			noc;
	int			i;

	coders = (t_person *)arg;
	noc = coders->config->number_of_coders;
	while (!is_dead(coders->config))
	{
		if (all_done(coders, noc))
		{
			report_success(coders);
			wake_all(coders);
			break ;
		}
		i = 0;
		while (i < noc && !is_dead(coders->config))
		{
			if (is_burnt_out(&coders[i], coders->config->time_to_burnout))
			{
				report_burnout(&coders[i]);
				wake_all(coders);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
