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
	pthread_mutex_t	print_lock;
	pthread_mutex_t	dead_lock;
}	t_config;

typedef struct s_ask_forstick
{
	int				coder;
	long			key;
	long			permission;
}	t_ask_forstick;

typedef struct s_stick
{
	int				held;
	long			available_at;
	t_ask_forstick	*heap;
	int				size;
	long			next_inline;
	pthread_mutex_t	lock;
	pthread_cond_t	waiting_room;
}	t_stick;

typedef struct s_person
{
	int				name;
	int				compiles;
	long			last_compile;
	t_stick			*left_stick;
	t_stick			*right_stick;
	t_stick			*held_sticks[2];
	int				held_count;
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
t_ask_forstick	heap_pop(t_stick *stick);
t_ask_forstick	*heap_peek(t_stick *stick);
int				heap_less(t_ask_forstick *a, t_ask_forstick *b);
void			sift_up(t_ask_forstick *heap, int i);
void			sift_down(t_ask_forstick *heap, int size, int i);

long	mytime(void);
int		is_dead(t_config *config);
int		parse_args(t_config *config, int argc, char **argv);
int		validate_config(t_config *config);
void	log_state(t_person *coder, const char *msg);

int		setup_world(t_world *world, t_config *config);
void	cleanup(t_world *world, int n);

int		start_threads(t_config *config, t_world *world);
void	join_threads(t_world *world, int n);

void	*coder_routine(void *arg);
void	*compile(t_person *coder);
void	*debug(t_person *coder);
void	*refactor(t_person *coder);
void	unlock_sticks(t_person *coder);

int		lock_sticks(t_person *coder);

void	*monitor(void *arg);
t_ask_forstick	create_request(t_person *coder, t_stick *stick);

int		acquire(t_stick *stick, t_person *coder);
void	release(t_stick *stick, t_config *config);
int		my_turn(t_stick *stick, t_person *coder);
#endif

