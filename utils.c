#include "codexion.h"

long	mytime(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

int	is_dead(t_config *config)
{
	int	dead;

	pthread_mutex_lock(&config->dead_lock);
	dead = config->dead;
	pthread_mutex_unlock(&config->dead_lock);
	return (dead);
}

void	log_state(t_person *coder, const char *msg)
{
	pthread_mutex_lock(&coder->config->print_lock);
	if (!is_dead(coder->config))
		printf("%ld %d %s\n", mytime() - coder->config->start,
			coder->name, msg);
	pthread_mutex_unlock(&coder->config->print_lock);
}
