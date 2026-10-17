/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   monitor.c                                    THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:35 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:35 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burnt_out(t_person *coder, int ttb)
{
	long	elapsed;

	if (pthread_mutex_trylock(&coder->lock) != 0)
		return (0);
	elapsed = (mytime() - coder->config->start) - coder->last_compile;
	pthread_mutex_unlock(&coder->lock);
	return (elapsed >= ttb);
}

static int	all_done(t_person *coders, int noc)
{
	int	i;
	int	done;

	i = 0;
	while (i < noc)
	{
		if (pthread_mutex_trylock(&coders[i].lock) != 0)
			return (0);
		done = (coders[i].compiles
				>= coders[i].config->number_of_compiles_required);
		pthread_mutex_unlock(&coders[i].lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
}

static void	check_burnouts(t_person *coders, int noc)
{
	int	i;

	i = 0;
	while (i < noc && !is_dead(coders->config))
	{
		if (is_burnt_out(&coders[i], coders->config->time_to_burnout))
		{
			report_burnout(&coders[i]);
			wake_all(coders);
		}
		i++;
	}
}

void	*monitor(void *arg)
{
	t_person	*coders;
	int			noc;

	coders = (t_person *)arg;
	noc = coders->config->number_of_coders;
	while (!is_dead(coders->config))
	{
		if (all_done(coders, noc))
		{
			report_success(coders);
			wake_all(coders);
			break ;
		}
		check_burnouts(coders, noc);
		usleep(1000);
	}
	return (NULL);
}
