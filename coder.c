#include "codexion.h"

void	unlock_sticks(t_person *coder)
{
	release(coder->left_stick, coder->config);
	if (coder->right_stick != coder->left_stick)
		release(coder->right_stick, coder->config);
	coder->held_sticks[0] = NULL;
	coder->held_sticks[1] = NULL;
	coder->held_count = 0;
}

void	*coder_routine(void *arg)
{
	t_person	*coder;

	coder = (t_person *)arg;
	while (!is_dead(coder->config)
		&& coder->compiles < coder->config->number_of_compiles_required)
	{
		if (!lock_sticks(coder))
			break ;
		compile(coder);
		unlock_sticks(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
