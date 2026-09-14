
#include "codexion.h"

void	*compile(t_person *coder)
{
	pthread_mutex_lock(&coder->lock);
  
  //the addition of the line below made the program
  //only print that users have taken dongles
  //prob just need to scale the sleep time, hopefully it's that simple
  sleep(coder->config->time_to_compile * 1000);
	coder->compiles++;
	coder->last_compile = mytime() - coder->config->start;
	pthread_mutex_unlock(&coder->lock);
	pthread_mutex_lock(&coder->config->print_lock);
	printf("%ld coder %d has compiled (compile # %d )\n",
		mytime() - coder->config->start, coder->name, coder->compiles);
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
    printf("%ld %d has taken a dongle\n",mytime()- coder->config->start, coder->name);
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
    printf("%ld %d has taken a dongle\n",mytime()- coder->config->start, coder->name);
	pthread_mutex_lock(&second->lock);
    printf("%ld %d has taken a dongle\n",mytime()- coder->config->start, coder->name);
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

