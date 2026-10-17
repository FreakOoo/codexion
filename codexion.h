/* ************************************************************************** */
/*                                                                            */
/*                                                I AM HIJACKING THE 42LOGO   */
/*   codexion.h                                   THIS MESSAGE IS BROADCAST   */
/*                                                       BY FREAKO            */
/*   By: mchopin <mchopin@student.codam.nl>       WAWAWAWAWAWAWAWAWAWAWAWAW   */
/*                                                HEHE HERE'S YOUR NEW LOGO   */
/*   Created: 2026/10/17 19:00:49 by mchopin                42                */
/*   Updated: 2026/10/17 19:00:50 by mchopin      get shrinkified nerds >:)   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define FIFO 0
# define EDF 1

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_config
{
	int				number_of_coders;
	int				number_of_compiles_required;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				dongle_cooldown;
	int				scheduler;
	int				dead;
	int				finished;
	long			start;
	long			next_ticket;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	table_lock;
	pthread_cond_t	table_cond;
}	t_config;

typedef struct s_ask_forstick
{
	struct s_person	*coder;
	long			key;
	long			permission;
}	t_ask_forstick;

typedef struct s_stick
{
	int				held;
	long			available_at;
	t_ask_forstick	*heap;
	int				size;
}	t_stick;

typedef struct s_person
{
	int				name;
	int				compiles;
	long			last_compile;
	t_stick			*left_stick;
	t_stick			*right_stick;
	t_config		*config;
	pthread_mutex_t	lock;
}	t_person;

typedef struct s_world
{
	t_stick		*sticks;
	t_person	*coders;
	pthread_t	*threads;
	pthread_t	monitor_thread;
}	t_world;

void			heap_push(t_stick *stick, t_ask_forstick req);
void			heap_remove(t_stick *stick, t_person *coder);
t_ask_forstick	*heap_peek(t_stick *stick);
int				heap_less(t_ask_forstick *a, t_ask_forstick *b);
void			sift_up(t_ask_forstick *heap, int i);
void			sift_down(t_ask_forstick *heap, int size, int i);

long			mytime(void);
int				is_dead(t_config *config);
void			log_state(t_person *coder, const char *msg);
int				parse_args(t_config *config, int argc, char **argv);
int				validate_config(t_config *config);

int				setup_world(t_world *world, t_config *config);
void			cleanup(t_world *world, t_config *config);
int				start_threads(t_config *config, t_world *world);
void			join_threads(t_world *world, int n);

void			*coder_routine(void *arg);
void			*compile(t_person *coder);
void			*debug(t_person *coder);
void			*refactor(t_person *coder);
void			unlock_sticks(t_person *coder);
int				lock_sticks(t_person *coder);

void			*monitor(void *arg);
void			wake_all(t_person *coders);
void			report_burnout(t_person *coder);
void			report_success(t_person *coder);

t_ask_forstick	create_request(t_person *coder);
int				acquire(t_person *coder);
void			release(t_person *coder);
int				my_turn(t_person *coder);

#endif
