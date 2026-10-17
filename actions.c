/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   actions.c                                    THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:34 by mchopin                42                */
/*   Updated: 2026/10/17 19:00:28 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	busy_sleep(t_config *config, int *action)
{
	long	end;

	end = mytime() + *action;
	while (mytime() < end && !is_dead(config))
		usleep(500);
}

void	*compile(t_person *coder)
{
	pthread_mutex_lock(&coder->lock);
	coder->last_compile = mytime() - coder->config->start;
	pthread_mutex_unlock(&coder->lock);
	log_state(coder, "is compiling");
	busy_sleep(coder->config, &coder->config->time_to_compile);
	if (is_dead(coder->config))
		return (NULL);
	pthread_mutex_lock(&coder->lock);
	coder->compiles++;
	pthread_mutex_unlock(&coder->lock);
	return (NULL);
}

static void	timed_action(t_person *coder, int duration, char *msg)
{
	log_state(coder, msg);
	busy_sleep(coder->config, &duration);
}

void	*debug(t_person *coder)
{
	timed_action(coder, coder->config->time_to_debug, "is debugging");
	return (NULL);
}

void	*refactor(t_person *coder)
{
	timed_action(coder, coder->config->time_to_refactor, "is refactoring");
	return (NULL);
}
