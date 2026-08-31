#ifndef CODEXION_H
# define CODEXION_H


//Currently the most important thing is implementing timers
//

// For unspecified values, chosen kinda randomly
// adjust values if sensible

// NOC number of coders; TTB time do burnout etc.
#define DEFAULT_NOC 100
#define DEFAULT_TTB 800
#define DEFAULT_TTC 200
#define DEFAULT_TTD  200
#define DEFAULT_TTR  200
#define DEFAULT_COMPILES  3
#define DEFAULT_DONGLE_COOLDOWN 100
#define DEFAULT_SCHEDULER 0


# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

// shared, read-only once threads start: how many times each coder must
// individually compile before stopping, and the lock guarding stdout so
// two coders printing at once don't garble each other

//check this config sghit it's weird

//tv stands for timevalue
long mytime(void);

typedef struct s_config
{
	int				number_of_compiles_required;
	pthread_mutex_t	print_lock;
}	t_config;
typedef struct s_stick
{
	int				available;
	long			last_use_timer;
	pthread_mutex_t	lock;
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

void	*coder_routine(void *arg);
void	*compile(void *arg);

#endif
