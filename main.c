/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   main.c                                       THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:35 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:35 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
// usage: ./codexion number_of_coders time_to_burnout time_to_compile
//        time_to_debug time_to_refactor number_of_compiles_required
//        dongle_cooldown scheduler(fifo|edf)

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

static int	run(t_config *config, t_world *world)
{
	if (!setup_world(world, config))
		return (0);
	config->start = mytime();
	if (!start_threads(config, world))
	{
		cleanup(world, config->number_of_coders);
		return (0);
	}
	join_threads(world, config->number_of_coders);
	cleanup(world, config->number_of_coders);
	return (1);
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_world		world;
	int			ok;

	if (!parse_args(&config, argc, argv))
	{
		fprintf(stderr, "usage: %s number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown "
			"fifo|edf\n(all values are non-negative integers)\n", argv[0]);
		return (EXIT_FAILURE);
	}
	config.dead = 0;
	config.finished = 0;
	pthread_mutex_init(&config.print_lock, NULL);
	pthread_mutex_init(&config.dead_lock, NULL);
	ok = run(&config, &world);
	pthread_mutex_destroy(&config.print_lock);
	pthread_mutex_destroy(&config.dead_lock);
	if (!ok || !config.finished)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
