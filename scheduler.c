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

t_ask_forstick	create_request(t_person *coder, t_stick *stick)
{
	t_ask_forstick	the_ask;

	the_ask.coder = coder->name;
	the_ask.permission = stick->next_inline;
	stick->next_inline++;
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
