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

static void	init_sync(t_config *config)
{
	config->dead = 0;
	config->finished = 0;
	config->next_ticket = 0;
	pthread_mutex_init(&config->print_lock, NULL);
	pthread_mutex_init(&config->dead_lock, NULL);
	pthread_mutex_init(&config->table_lock, NULL);
	pthread_cond_init(&config->table_cond, NULL);
}

static void	destroy_sync(t_config *config)
{
	pthread_mutex_destroy(&config->print_lock);
	pthread_mutex_destroy(&config->dead_lock);
	pthread_mutex_destroy(&config->table_lock);
	pthread_cond_destroy(&config->table_cond);
}

static int	run(t_config *config, t_world *world)
{
	if (!setup_world(world, config))
		return (0);
	config->start = mytime();
	if (!start_threads(config, world))
	{
		cleanup(world, config);
		return (0);
	}
	join_threads(world, config->number_of_coders);
	cleanup(world, config);
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
	init_sync(&config);
	ok = run(&config, &world);
	destroy_sync(&config);
	if (!ok || !config.finished)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
