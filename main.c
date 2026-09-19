// config is read from stdin as KEY=VALUE lines, one per word, e.g.:
//   NOC=100
//   TTB=800
// any key left unspecified keeps its DEFAULT_* value from codexion.h
//
// keys: NOC TTB TTC TTD TTR COMPILES DONGLE_COOLDOWN SCHEDULER
// (see codexion.h for what each one means)

#include "codexion.h"

int	start_threads(t_config *config, t_world *world)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		if (pthread_create(&world->threads[i], NULL,
				coder_routine, &world->coders[i]))
			return (0);
		i++;
	}
	if (pthread_create(&world->monitor_thread, NULL, monitor, world->coders))
		return (0);
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

int	main(void)
{
	t_config	config;
	t_world		world;
	int			was_dead;

	print_prompt();
	read_config(&config);
	if (config.number_of_coders <= 0)
		return (EXIT_FAILURE);
	config.dead = 0;
	pthread_mutex_init(&config.print_lock, NULL);
	pthread_mutex_init(&config.dead_lock, NULL);
	if (!setup_world(&world, &config))
		return (EXIT_FAILURE);
	config.start = mytime();
	if (!start_threads(&config, &world))
		return (EXIT_FAILURE);
	join_threads(&world, config.number_of_coders);
	was_dead = config.dead;
	cleanup(&world, config.number_of_coders);
	pthread_mutex_destroy(&config.print_lock);
	pthread_mutex_destroy(&config.dead_lock);
	if (was_dead)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
