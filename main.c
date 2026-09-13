// config is read from stdin as KEY=VALUE lines, one per word, e.g.:
//   NOC=100
//   TTB=800
// any key left unspecified keeps its DEFAULT_* value from codexion.h
//
// keys: NOC TTB TTC TTD TTR COMPILES DONGLE_COOLDOWN SCHEDULER
// (see codexion.h for what each one means)

// TODO: value validation (reject negatives etc.), debug/refactor phases,
// burnout timer, monitor thread, scheduler

#include "codexion.h"

static void	init_sticks(t_stick *sticks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		sticks[i].available = 1;
		sticks[i].last_use_timer = 0;
		pthread_mutex_init(&sticks[i].lock, NULL);
		i++;
	}
}

static void	init_coders(t_person *coders, t_stick *sticks,
		t_config *config, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		coders[i].name = i + 1;
		coders[i].compiles = 0;
		coders[i].last_compile = 0;
		coders[i].left_stick = &sticks[i];
		coders[i].right_stick = &sticks[(i + 1) % n];
		coders[i].held_sticks[0] = NULL;
		coders[i].held_sticks[1] = NULL;
		coders[i].held_count = 0;
		coders[i].config = config;
		pthread_mutex_init(&coders[i].lock, NULL);
		i++;
	} 
}

static void	cleanup(t_stick *sticks, t_person *coders,
		pthread_t *threads, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&sticks[i].lock);
		pthread_mutex_destroy(&coders[i].lock);
		i++;
	}
	free(sticks);
	free(coders);
	free(threads);
}

// checks the part of the word before '=' against each known key and,
// on a match, stores the part after '=' into that key's variable
static void	set_field(char *field, char *equal, int *n, int *ttb, int *ttc)
{
	if (!strcmp(field, "NOC"))
		*n = atoi(equal + 1);
	else if (!strcmp(field, "TTB"))
		*ttb = atoi(equal + 1);
	else if (!strcmp(field, "TTC"))
		*ttc = atoi(equal + 1);
}

static void	set_field2(char *field, char *equal, int *ttd, int *ttr,
		int *compiles)
{
	if (!strcmp(field, "TTD"))
		*ttd = atoi(equal + 1);
	else if (!strcmp(field, "TTR"))
		*ttr = atoi(equal + 1);
	else if (!strcmp(field, "COMPILES"))
		*compiles = atoi(equal + 1);
}

static void	set_field3(char *field, char *equal, int *cooldown, int *scheduler)
{
	if (!strcmp(field, "DONGLE_COOLDOWN"))
		*cooldown = atoi(equal + 1);
	else if (!strcmp(field, "SCHEDULER"))
		*scheduler = atoi(equal + 1);
}

// reads KEY=VALUE lines from stdin until EOF, updating whichever
// variable matches; keys that never appear keep their default value
static void	read_config(int *n, int *ttb, int *ttc, int *ttd, int *ttr,
		int *compiles, int *cooldown, int *scheduler)
{
	char	*field;
	size_t	cap;
	ssize_t	len;
	char	*equal;

	field = NULL;
	cap = 0;
	while ((len = getline(&field, &cap, stdin)) != -1)
	{
		if (len > 0 && field[len - 1] == '\n')
			field[len - 1] = '\0';
		equal = strchr(field, '=');
		if (equal)
		{
			*equal = '\0';
			set_field(field, equal, n, ttb, ttc);
			set_field2(field, equal, ttd, ttr, compiles);
			set_field3(field, equal, cooldown, scheduler);
		}
	}
	free(field);
}

int	main(void)
{
	int			n;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			dongle_cooldown;
	int			scheduler;
	t_config	config;
	t_stick		*sticks;
	t_person	*coders;
	pthread_t	*threads;
	pthread_t	monitor_thread;
	int			i;

	n = DEFAULT_NOC;
	time_to_burnout = DEFAULT_TTB;
	time_to_compile = DEFAULT_TTC;
	time_to_debug = DEFAULT_TTD;
	time_to_refactor = DEFAULT_TTR;
	config.number_of_compiles_required = DEFAULT_COMPILES;
	dongle_cooldown = DEFAULT_DONGLE_COOLDOWN;
	scheduler = DEFAULT_SCHEDULER;
	read_config(&n, &time_to_burnout, &time_to_compile, &time_to_debug,
		&time_to_refactor, &config.number_of_compiles_required,
		&dongle_cooldown, &scheduler);
	if (n <= 0)
		return (EXIT_FAILURE);
	config.number_of_coders = n;
	config.time_to_burnout = time_to_burnout;
	pthread_mutex_init(&config.print_lock, NULL);
	// TO-DO: wire the rest of these into t_person / t_stick once the
	// debug and refactor phases exist; not read anywhere yet.
	(void)time_to_compile;
	(void)time_to_debug;
	(void)time_to_refactor;
	(void)dongle_cooldown;
	(void)scheduler;
	sticks = malloc(sizeof(t_stick) * n);
	coders = malloc(sizeof(t_person) * n);
	threads = malloc(sizeof(pthread_t) * n);
	init_sticks(sticks, n);
	init_coders(coders, sticks, &config, n);
	config.start = mytime();
	i = 0;
	while (i < n)
	{
		if (pthread_create(&threads[i], NULL, coder_routine, &coders[i]))
			return (EXIT_FAILURE);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, coders))
		return (EXIT_FAILURE);
	i = 0;
	while (i < n)
		pthread_join(threads[i++], NULL);
	pthread_join(monitor_thread, NULL);
	cleanup(sticks, coders, threads, n);
	pthread_mutex_destroy(&config.print_lock);
	return (0);
}
