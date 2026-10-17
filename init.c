/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   init.c                                       THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:35 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:35 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_sticks(t_stick *sticks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sticks[i].heap = malloc(sizeof(t_ask_forstick) * 2);
		if (!sticks[i].heap)
		{
			while (i-- > 0)
				free(sticks[i].heap);
			return (0);
		}
		sticks[i].held = 0;
		sticks[i].available_at = 0;
		sticks[i].size = 0;
		i++;
	}
	return (1);
}

static void	init_coders(t_person *coders, t_stick *sticks,
		t_config *config, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		coders[i].name = i + 1;
		coders[i].compiles = 0;
		coders[i].last_compile = 0;
		coders[i].left_stick = &sticks[i];
		coders[i].right_stick = &sticks[(i + 1) % n];
		coders[i].config = config;
		pthread_mutex_init(&coders[i].lock, NULL);
		i++;
	}
}

int	setup_world(t_world *world, t_config *config)
{
	int	n;

	n = config->number_of_coders;
	world->sticks = malloc(sizeof(t_stick) * n);
	world->coders = malloc(sizeof(t_person) * n);
	world->threads = malloc(sizeof(pthread_t) * n);
	if (!world->sticks || !world->coders || !world->threads
		|| !init_sticks(world->sticks, n))
	{
		free(world->sticks);
		free(world->coders);
		free(world->threads);
		return (0);
	}
	init_coders(world->coders, world->sticks, config, n);
	return (1);
}

void	cleanup(t_world *world, t_config *config)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		free(world->sticks[i].heap);
		pthread_mutex_destroy(&world->coders[i].lock);
		i++;
	}
	free(world->sticks);
	free(world->coders);
	free(world->threads);
}
