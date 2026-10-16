#include "codexion.h"

static int	parse_int(const char *str, int *out)
{
	long	value;
	int		i;

	if (!str[0])
		return (0);
	value = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		value = value * 10 + (str[i] - '0');
		if (value > INT_MAX)
			return (0);
		i++;
	}
	*out = (int)value;
	return (1);
}

static int	parse_scheduler(const char *str, int *out)
{
	if (!strcmp(str, "fifo"))
		*out = FIFO;
	else if (!strcmp(str, "edf"))
		*out = EDF;
	else
		return (0);
	return (1);
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
	return (1);
}

int	parse_args(t_config *config, int argc, char **argv)
{
	if (argc != 9)
		return (0);
	if (!parse_int(argv[1], &config->number_of_coders)
		|| !parse_int(argv[2], &config->time_to_burnout)
		|| !parse_int(argv[3], &config->time_to_compile)
		|| !parse_int(argv[4], &config->time_to_debug)
		|| !parse_int(argv[5], &config->time_to_refactor)
		|| !parse_int(argv[6], &config->number_of_compiles_required)
		|| !parse_int(argv[7], &config->dongle_cooldown)
		|| !parse_scheduler(argv[8], &config->scheduler))
		return (0);
	return (validate_config(config));
}
