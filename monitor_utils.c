/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   monitor_utils.c                              THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 18:58:35 by mchopin                42                */
/*   Updated: 2026/10/17 18:58:35 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_all(t_person *coders)
{
	pthread_mutex_lock(&coders->config->table_lock);
	pthread_cond_broadcast(&coders->config->table_cond);
	pthread_mutex_unlock(&coders->config->table_lock);
}

void	report_burnout(t_person *coder)
{
	int	already_dead;

	pthread_mutex_lock(&coder->config->dead_lock);
	already_dead = coder->config->dead;
	coder->config->dead = 1;
	pthread_mutex_unlock(&coder->config->dead_lock);
	if (already_dead)
		return ;
	pthread_mutex_lock(&coder->config->print_lock);
	printf("%ld %d burned out\n",
		mytime() - coder->config->start, coder->name);
	pthread_mutex_unlock(&coder->config->print_lock);
}

void	report_success(t_person *coder)
{
	int	already_dead;

	pthread_mutex_lock(&coder->config->dead_lock);
	already_dead = coder->config->dead;
	coder->config->dead = 1;
	if (!already_dead)
		coder->config->finished = 1;
	pthread_mutex_unlock(&coder->config->dead_lock);
}
