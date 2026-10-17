/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   dongle.c                                     THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:34 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:34 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	earliest(t_stick *stick, t_config *config, long wake)
{
	long	end;

	end = config->start + stick->available_at;
	if (!stick->held && end < wake && end > mytime())
		return (end);
	return (wake);
}

static void	wait_turn(t_person *coder)
{
	struct timespec	moment;
	long			wake;

	wake = earliest(coder->right_stick, coder->config, mytime() + 5);
	wake = earliest(coder->left_stick, coder->config, wake);
	moment.tv_sec = wake / 1000;
	moment.tv_nsec = (wake % 1000) * 1000000;
	pthread_cond_timedwait(&coder->config->table_cond,
		&coder->config->table_lock, &moment);
}

static void	enqueue(t_person *coder, int add)
{
	t_ask_forstick	ask;

	if (add)
	{
		ask = create_request(coder);
		heap_push(coder->left_stick, ask);
		if (coder->right_stick != coder->left_stick)
			heap_push(coder->right_stick, ask);
		return ;
	}
	heap_remove(coder->left_stick, coder);
	if (coder->right_stick != coder->left_stick)
		heap_remove(coder->right_stick, coder);
}

int	acquire(t_person *coder)
{
	t_config	*config;

	config = coder->config;
	pthread_mutex_lock(&config->table_lock);
	enqueue(coder, 1);
	while (!my_turn(coder))
	{
		if (is_dead(config))
		{
			enqueue(coder, 0);
			pthread_mutex_unlock(&config->table_lock);
			return (0);
		}
		wait_turn(coder);
	}
	enqueue(coder, 0);
	coder->left_stick->held = 1;
	coder->right_stick->held = 1;
	pthread_cond_broadcast(&config->table_cond);
	pthread_mutex_unlock(&config->table_lock);
	return (1);
}

void	release(t_person *coder)
{
	t_config	*config;
	long		ready_at;

	config = coder->config;
	pthread_mutex_lock(&config->table_lock);
	ready_at = mytime() - config->start + config->dongle_cooldown;
	coder->left_stick->held = 0;
	coder->left_stick->available_at = ready_at;
	coder->right_stick->held = 0;
	coder->right_stick->available_at = ready_at;
	pthread_cond_broadcast(&config->table_cond);
	pthread_mutex_unlock(&config->table_lock);
}
