/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   sticks.c                                     THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:36 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:36 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	lock_sticks(t_person *coder)
{
	if (!acquire(coder))
		return (0);
	log_state(coder, "has taken a dongle");
	if (coder->left_stick == coder->right_stick)
	{
		while (!is_dead(coder->config))
			usleep(1000);
		return (0);
	}
	log_state(coder, "has taken a dongle");
	return (1);
}
