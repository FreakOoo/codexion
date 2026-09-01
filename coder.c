#include "codexion.h"

void	*compile(void *arg)
{
	t_person	*coder;

	coder = (t_person *)arg;
	coder->compiles++;
	pthread_mutex_lock(&coder->config->print_lock);

  // ADD TIMESTAMP HERE
	printf("coder %d has compiled (compile # %d )\n", coder->name, coder->compiles);
	pthread_mutex_unlock(&coder->config->print_lock);
	return (NULL);
}

static void	lock_sticks(t_person *coder)
{
	t_stick	*first;
	t_stick	*second;

	first = coder->left_stick;
	second = coder->right_stick;
	if (first == second)
	{
		pthread_mutex_lock(&first->lock);
    //ADD TIMESTAMP, lock first dongle
    //also make sure it prints the coder number here
    //
    // HAVE YET TO FEED THESE PRINTS A TIMESTAMP.
    //
    printf(" %d has taken a dongle\n", coder->name);
		coder->held_sticks[0] = first;
		coder->held_count = 1;
		return ;
	}
	if (first > second)
	{
		first = coder->right_stick;
		second = coder->left_stick;
	}
	pthread_mutex_lock(&first->lock);
    //ADD TIMESTAMP Dongle 1
    //also make sure it prints the coder number here
    printf(" %d has taken a dongle\n", coder->name);
	pthread_mutex_lock(&second->lock);
    //ADD TIMESTAMP Dongle 2
    //also make sure it prints the coder number here
    printf(" %d has taken a dongle\n", coder->name);
	coder->held_sticks[0] = coder->left_stick;
	coder->held_sticks[1] = coder->right_stick;
	coder->held_count = 2;
}

static void	unlock_sticks(t_person *coder)
{
	pthread_mutex_unlock(&coder->left_stick->lock);
	if (coder->right_stick != coder->left_stick)
		pthread_mutex_unlock(&coder->right_stick->lock);
	coder->held_sticks[0] = NULL;
	coder->held_sticks[1] = NULL;
	coder->held_count = 0;
}
//
//
//fake ass function to be done later
void	*coder_routine(void *arg)
{
	t_person	*coder;

	coder = (t_person *)arg;
	while (coder->compiles < coder->config->number_of_compiles_required)
	{
		lock_sticks(coder);
		compile(coder);
		unlock_sticks(coder);
	}
	return (NULL);
}

