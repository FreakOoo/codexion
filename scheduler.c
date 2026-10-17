/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   scheduler.c                                  THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:36 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:36 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_ask_forstick	create_request(t_person *coder)
{
	t_ask_forstick	the_ask;

	the_ask.coder = coder;
	the_ask.permission = coder->config->next_ticket++;
	if (coder->config->scheduler == FIFO)
		the_ask.key = the_ask.permission;
	else
	{
		pthread_mutex_lock(&coder->lock);
		the_ask.key = coder->last_compile + coder->config->time_to_burnout;
		pthread_mutex_unlock(&coder->lock);
	}
	return (the_ask);
}

static int	ready(t_stick *stick, t_config *config)
{
	if (stick->held)
		return (0);
	return (mytime() - config->start >= stick->available_at);
}

static t_stick	*other_stick(t_person *coder, t_stick *stick)
{
	if (coder->left_stick == stick)
		return (coder->right_stick);
	return (coder->left_stick);
}

static int	must_yield(t_person *coder, t_stick *stick)
{
	t_ask_forstick	*first;

	first = heap_peek(stick);
	if (!first || first->coder == coder)
		return (0);
	return (ready(other_stick(first->coder, stick), coder->config));
}

int	my_turn(t_person *coder)
{
	if (!ready(coder->left_stick, coder->config)
		|| !ready(coder->right_stick, coder->config))
		return (0);
	if (must_yield(coder, coder->left_stick))
		return (0);
	return (!must_yield(coder, coder->right_stick));
}
