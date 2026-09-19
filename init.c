#include "codexion.h"

static void	init_sticks(t_stick *sticks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sticks[i].available = 1;
		sticks[i].last_use_timer = 0;
		pthread_mutex_init(&sticks[i].lock, NULL);
		i++;
	}
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
		coders[i].held_sticks[0] = NULL;
		coders[i].held_sticks[1] = NULL;
		coders[i].held_count = 0;
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
	if (!world->sticks || !world->coders || !world->threads)
		return (0);
	init_sticks(world->sticks, n);
	init_coders(world->coders, world->sticks, config, n);
	return (1);
}

void	cleanup(t_world *world, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&world->sticks[i].lock);
		pthread_mutex_destroy(&world->coders[i].lock);
		i++;
	}
	free(world->sticks);
	free(world->coders);
	free(world->threads);
}
