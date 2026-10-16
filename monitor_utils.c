#include "codexion.h"

void	wake_all(t_person *coders)
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

void	report_burnout(t_person *coder)
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

void	report_success(t_person *coder)
{
	int	already_dead;

	pthread_mutex_lock(&coder->config->dead_lock);
	already_dead = coder->config->dead;
	coder->config->dead = 1;
	if (!already_dead)
		coder->config->finished = 1;
	pthread_mutex_unlock(&coder->config->dead_lock);
}
