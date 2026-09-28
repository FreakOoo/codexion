#include "codexion.h"

void	print_prompt(void)
{
	printf("you can select the following values as KEY=VALUE, ");
	printf("one per line:\n");
	printf("  NOC TTB TTC TTD TTR COMPILES DONGLE_COOLDOWN SCHEDULER\n\n");
	printf("or press ctrl for the defaults from"
		" codexion.h:\n");
	printf("  NOC=%d TTB=%d TTC=%d TTD=%d TTR=%d\n",
		DEFAULT_NOC, DEFAULT_TTB, DEFAULT_TTC, DEFAULT_TTD, DEFAULT_TTR);
	printf("  COMPILES=%d DONGLE_COOLDOWN=%d SCHEDULER=%d\n",
		DEFAULT_COMPILES, DEFAULT_DONGLE_COOLDOWN, DEFAULT_SCHEDULER);
}

static void	set_field(char *field, char *equal, t_config *config)
{
	if (!strcmp(field, "NOC"))
		config->number_of_coders = atoi(equal + 1);
	else if (!strcmp(field, "TTB"))
		config->time_to_burnout = atoi(equal + 1);
	else if (!strcmp(field, "TTC"))
		config->time_to_compile = atoi(equal + 1);
}

static void	set_field2(char *field, char *equal, t_config *config)
{
	if (!strcmp(field, "TTD"))
		config->time_to_debug = atoi(equal + 1);
	else if (!strcmp(field, "TTR"))
		config->time_to_refactor = atoi(equal + 1);
	else if (!strcmp(field, "COMPILES"))
		config->number_of_compiles_required = atoi(equal + 1);
	else if (!strcmp(field, "DONGLE_COOLDOWN"))
		config->dongle_cooldown = atoi(equal + 1);
	else if (!strcmp(field, "SCHEDULER"))
		config->scheduler = atoi(equal + 1);
}

static void	apply_defaults(t_config *config)
{
	config->number_of_coders = DEFAULT_NOC;
	config->time_to_burnout = DEFAULT_TTB;
	config->time_to_compile = DEFAULT_TTC;
	config->time_to_debug = DEFAULT_TTD;
	config->time_to_refactor = DEFAULT_TTR;
	config->number_of_compiles_required = DEFAULT_COMPILES;
	config->dongle_cooldown = DEFAULT_DONGLE_COOLDOWN;
	config->scheduler = DEFAULT_SCHEDULER;
}

void	read_config(t_config *config)
{
	char	*field;
	size_t	cap;
	ssize_t	len;
	char	*equal;

	apply_defaults(config);
	field = NULL;
	cap = 0;
	len = getline(&field, &cap, stdin);
	while (len != -1)
	{
		if (len > 0 && field[len - 1] == '\n')
			field[len - 1] = '\0';
		equal = strchr(field, '=');
		if (equal)
		{
			*equal = '\0';
			set_field(field, equal, config);
			set_field2(field, equal, config);
		}
		len = getline(&field, &cap, stdin);
	}
	free(field);
}

int	validate_config(t_config *config)
{
	if (config->number_of_coders <= 0)
		return (0);
	if (config->number_of_compiles_required <= 0)
		return (0);
	if (config->time_to_burnout <= 0)
		return (0);
	if (config->time_to_compile <= 0)
		return (0);
	if (config->time_to_debug <= 0)
		return (0);
	if (config->time_to_refactor <= 0)
		return (0);
	if (config->dongle_cooldown < 0)
		return (0);
	return (1);
}
