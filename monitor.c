#include "codexion.h"

static int	is_burnt_out(t_person *coder, int ttb)
{
	long	elapsed;

	pthread_mutex_lock(&coder->lock);
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
		pthread_mutex_lock(&coders[i].lock);
		done = (coders[i].compiles
				>= coders[i].config->number_of_compiles_required);
		pthread_mutex_unlock(&coders[i].lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
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
	printf("%ld coder %d is burnt out\n",
		mytime() - coder->config->start, coder->name);
	pthread_mutex_unlock(&coder->config->print_lock);
}

void	*monitor(void *arg)
{
	t_person	*coders;
	int			noc;
	int			i;

	coders = (t_person *)arg;
	noc = coders->config->number_of_coders;
	while (!is_dead(coders->config) && !all_done(coders, noc))
	{
		i = 0;
		while (i < noc && !is_dead(coders->config))
		{
			if (is_burnt_out(&coders[i], coders->config->time_to_burnout))
				report_burnout(&coders[i]);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
