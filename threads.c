/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   threads.c                                    THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 21:28:02 by mchopin                42                */
/*   Updated: 2026/10/17 21:28:03 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	abort_threads(t_config *config, t_world *world, int created)
{
	int	i;

	pthread_mutex_lock(&config->dead_lock);
	config->dead = 1;
	pthread_mutex_unlock(&config->dead_lock);
	wake_all(world->coders);
	i = 0;
	while (i < created)
		pthread_join(world->threads[i++], NULL);
}

int	start_threads(t_config *config, t_world *world)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		if (pthread_create(&world->threads[i], NULL,
				coder_routine, &world->coders[i]))
		{
			abort_threads(config, world, i);
			return (0);
		}
		i++;
	}
	if (pthread_create(&world->monitor_thread, NULL, monitor, world->coders))
	{
		abort_threads(config, world, i);
		return (0);
	}
	return (1);
}

void	join_threads(t_world *world, int n)
{
	int	i;

	i = 0;
	while (i < n)
		pthread_join(world->threads[i++], NULL);
	pthread_join(world->monitor_thread, NULL);
}
