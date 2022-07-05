#include "philo.h"


unsigned int	time_in_ml(void)
{
	struct timeval tv;
	unsigned int ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ms);
}

void am_i_dead(t_person *self_person)
{
	unsigned int res;

	res = self_person->time_end - self_person->time_now;
	res = self_person->ttd - res;
	if (res <= 0)
	{
		pthread_mutex_lock((self_person->is_there_victim_p));
		*(self_person->there_is_victim) = 1;
		pthread_mutex_unlock((self_person->is_there_victim_p));
	}
}

void eat_food(t_person *self_person)
{
	//pthread_mutex_lock((self_person->table_mutex));
	if (self_person->owner_fork == 1 && *(self_person->left_fork) == 1)
	{
		//pthread_mutex_unlock((self_person->table_mutex));
		pthread_mutex_lock(self_person->owner_fork_mutex);
		printf("%u %d has taken a fork\n", time_in_ml(), self_person->owner_id);
		pthread_mutex_lock((self_person->master[self_person->fork_mate_id - 1 % self_person->fcount]->owner_fork_mutex));
		printf("%u %d has taken a fork\n", time_in_ml(), self_person->owner_id);
		self_person->time_now = time_in_ml();
		self_person->owner_fork = 0;
	 	*(self_person->left_fork) = 0;
	 	pthread_mutex_unlock(self_person->owner_fork_mutex);
	 	pthread_mutex_unlock((self_person->master[self_person->fork_mate_id - 1 % self_person->fcount]->owner_fork_mutex));
	 	printf("%u %d is eating\n", time_in_ml(), self_person->owner_id);
	 	usleep(self_person->tte);
	 	pthread_mutex_lock(self_person->owner_fork_mutex);
	 	pthread_mutex_lock((self_person->master[self_person->fork_mate_id - 1 % self_person->fcount]->owner_fork_mutex));
	 	self_person->owner_fork = 1;
	 	*(self_person->left_fork) = 1;
	 	pthread_mutex_unlock(self_person->owner_fork_mutex);
	 	pthread_mutex_unlock((self_person->master[self_person->fork_mate_id - 1 % self_person->fcount]->owner_fork_mutex));
		//wanna_sleep(self_person);
		printf("%u %d is thinking\n", time_in_ml(), self_person->owner_id);
	}
	else
		pthread_mutex_unlock((self_person->table_mutex));
		
}

void eat_food2(t_person *self)
{
	int permission;
	int owner;
	int neighbor;

	permission = 0;
	owner = 0;
	neighbor = 0;
	pthread_mutex_lock(self->table_mutex);
	//pthread_mutex_lock(self->owner_fork_mutex);
	if (self->owner_fork == 1)
	{
		self->owner_fork = 0;
		pthread_mutex_unlock(self->table_mutex);
		printf("%u %d has taken a fork\n", time_in_ml(), self->owner_id);
		//pthread_mutex_unlock(self->owner_fork_mutex);
		owner = 1;
		permission += 1;
	}
	else
		pthread_mutex_unlock(self->table_mutex);

	pthread_mutex_lock(self->table_mutex);
	//pthread_mutex_lock((self->master[self->fork_mate_id - 1 % self->fcount]->owner_fork_mutex));
	if (*(self->left_fork) == 1)
	{
		*(self->left_fork) = 0;
		pthread_mutex_unlock(self->table_mutex);
		printf("%u %d has taken a fork\n", time_in_ml(), self->owner_id);
		//pthread_mutex_unlock((self->master[self->fork_mate_id - 1 % self->fcount]->owner_fork_mutex));
		neighbor = 1;
		permission += 1;
	}
	else
		pthread_mutex_unlock(self->table_mutex);

	if (permission == 2)
	{
		printf("%u %d is eating\n", time_in_ml(), self->owner_id);
		self->time_now = time_in_ml();
		wanna_sleep(self->tte);
		pthread_mutex_lock(self->table_mutex);
		*(self->left_fork) = 1;
		self->owner_fork = 1;
		pthread_mutex_unlock(self->table_mutex);
		wanna_sleep(self->tts);
		permission = 0;
	}
	else
	{
		permission = 0;
		pthread_mutex_lock(self->table_mutex);
		*(self->left_fork) = 1;
		self->owner_fork = 1;
		pthread_mutex_unlock(self->table_mutex);
	}

}




void  wanna_sleep(unsigned int time_type)
{
	//printf("%u %d is sleeping\n", time_in_ml(), self_person->owner_id);

	unsigned int turn;
	unsigned int eks;
	int			i;

	i = 0;
	turn = time_type / 30;
	eks = time_type % 30;

	while(i < 30 + eks)
	{
		usleep(turn);
		i++;
	}
}

t_person *put_information(int i, char **av, t_person **master, int *victim_pointer)
{
	t_person *rtn;

	rtn = malloc(sizeof(t_person));
	rtn->owner_id = i + 1;
	rtn->ttd = ft_atoi(av[2]) * 1000;
	rtn->tte = ft_atoi(av[3]) * 1000;
	rtn->tts = ft_atoi(av[4]) * 1000;
	rtn->fcount = ft_atoi(av[1]);
	rtn->owner_fork = 1;
	rtn->left_fork = malloc(sizeof(int) * 1);
	rtn->there_is_victim = malloc(sizeof(int) * 1);
	rtn->there_is_victim = victim_pointer;
	//rtn->time_now = 0;
	rtn->master = master;
	return rtn;
}

void edit_neighbor_forks(t_person **master, int f_count)
{
	int i;

	i = 0;
	while (i < f_count)
	{
		if (i == 0)
		{
			(master[0])->left_fork = &((master[f_count - 1])->owner_fork);
			(master[0])->fork_mate_id = f_count;
			(master[0])->left_fork_mutex = ((master[f_count -1]->owner_fork_mutex));
		}
		else if (i == f_count - 1)
		{
			(master[i])->left_fork = &(master[0]->owner_fork);
			(master[i])->fork_mate_id = 1;
			(master[i])->left_fork_mutex = ((master[0]->owner_fork_mutex));
		}
		else
		{
			(master[i])->left_fork = &((master[i - 1])->owner_fork);
			(master[i])->fork_mate_id = i;
			(master[i])->left_fork_mutex = ((master[i - 1]->owner_fork_mutex));
		}
		i++;
	}
}

t_person **create_persons_class(char **av, int *victim_pointer, pthread_mutex_t *is_there, pthread_mutex_t *table_mutexx)
{
	t_person **master;
	int			i;

	master = malloc(sizeof(t_person *) * ft_atoi(av[1]));
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		master[i] = put_information(i, av, master, victim_pointer);
		(master[i])->is_there_victim_p = is_there;
		(master[i])->table_mutex = table_mutexx;
		i++;
	}
	edit_neighbor_forks(master, ft_atoi(av[1]));
	return (master);
}


void all_over(t_person **master, int philo_count)
{
	int i;

	i = 0;
	while (i < philo_count)
	{
		free((master[i])->left_fork);
		free((master[i])->there_is_victim);
		pthread_mutex_destroy((master[i])->owner_fork_mutex);
		free((master[i])->owner_fork_mutex);
		free(master[i]);
		i++;
	}
	free(master);
}
