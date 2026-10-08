#include "codexion.h"

// last_use_timer starts before any real elapsed time can reach, so the
// dongle_cooldown check in tlock_or_die never gates a stick's first use
static int	init_sticks(t_stick *sticks, int n, int dongle_cooldown)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sticks[i].heap = malloc(sizeof(t_ask_forstick) * n);
		if (!sticks[i].heap)
		{
			while (i-- > 0)
				free(sticks[i].heap);
			return (0);
		}
		sticks[i].available = 1;
		sticks[i].held = 0;
		sticks[i].available_at = 0;
		sticks[i].size = 0;
		sticks[i].next_inline = 0;
		sticks[i].last_use_timer = -dongle_cooldown;
		pthread_mutex_init(&sticks[i].lock, NULL);
		pthread_cond_init(&sticks[i].waiting_room, NULL);
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
	if (!init_sticks(world->sticks, n, config->dongle_cooldown))
		return (0);
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
		pthread_cond_destroy(&world->sticks[i].waiting_room);
		free(world->sticks[i].heap);
		pthread_mutex_destroy(&world->coders[i].lock);
		i++;
	}
	free(world->sticks);
	free(world->coders);
	free(world->threads);
}
