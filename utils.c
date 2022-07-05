#include "philo.h"


unsigned int	time_in_ml(void)
{
	struct timeval tv;
	unsigned int ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ms);
}

void am_i_dead(t_person *self_person, int id)
{
	unsigned int current_time;

	current_time = time_in_ml();
	if ((current_time - self_person->time_now) > (self_person->ttd / 1000))
	{
		printf("%u %d is dead from %d\n", current_time - self_person->time_now, self_person->owner_id, id);
		exit(0);
	}
}

/*void eat_food(t_person *self_person)
{
	pthread_mutex_lock((self_person->table_mutex));
	if (self_person->owner_fork == 1 && *(self_person->left_fork) == 1)
	{
		pthread_mutex_unlock((self_person->table_mutex));
		pthread_mutex_lock(self_person->owner_fork_mutex);
		printf("%u %d has taken a fork\n", time_in_ml(), self_person->owner_id);
		pthread_mutex_lock((self_person->master[self_person->fork_mate_i]->owner_fork_mutex));
		printf("%u %d has taken a fork\n", time_in_ml(), self_person->owner_id);
		self_person->time_now = time_in_ml();
		self_person->owner_fork = 0;
	 	*(self_person->left_fork) = 0;
	 	pthread_mutex_unlock(self_person->owner_fork_mutex);
	 	pthread_mutex_unlock((self_person->master[self_person->fork_mate_i]->owner_fork_mutex));
	 	printf("%u %d is eating\n", time_in_ml(), self_person->owner_id);
	 	usleep(self_person->tte);
	 	pthread_mutex_lock(self_person->owner_fork_mutex);
	 	pthread_mutex_lock((self_person->master[self_person->fork_mate_i]->owner_fork_mutex));
	 	self_person->owner_fork = 1;
	 	*(self_person->left_fork) = 1;
	 	pthread_mutex_unlock(self_person->owner_fork_mutex);
	 	pthread_mutex_unlock((self_person->master[self_person->fork_mate_i]->owner_fork_mutex));
		wanna_sleep(self_person->tts);
		printf("%u %d is thinking\n", time_in_ml(), self_person->owner_id);
	}
	else
		pthread_mutex_unlock((self_person->table_mutex));
		
}*/

int eat_food2(t_person *self)
{
	int rtn;

	rtn = 0;
	am_i_dead(self, 2);
	pthread_mutex_lock(self->owner_fork_mutex);
	if (self->owner_fork == 1)
	{
		self->owner_fork = 0;
		printf("%u %d has taken a fork\n", time_in_ml(), self->owner_id);
		rtn++;
	}
	pthread_mutex_unlock(self->owner_fork_mutex);
	pthread_mutex_lock(self->master[self->fork_mate_i]->owner_fork_mutex);
	if (self->left_fork[0] == 1)
	{
		self->left_fork[0] = 0;
		printf("%u %d has taken a fork\n", time_in_ml(), self->owner_id);
		rtn++;
	}
	pthread_mutex_unlock(self->master[self->fork_mate_i]->owner_fork_mutex);
	return (rtn == 2);
}

void put_that_fork_back(t_person *self_person)
{
	pthread_mutex_lock(self_person->owner_fork_mutex);
	self_person->owner_fork = 0;
	pthread_mutex_unlock(self_person->owner_fork_mutex);
	pthread_mutex_lock(self_person->master[self_person->fork_mate_i]->owner_fork_mutex);
	self_person->left_fork[0] = 0;
	pthread_mutex_unlock(self_person->master[self_person->fork_mate_i]->owner_fork_mutex);
}



void  lets_eat(t_person *self)
{
	unsigned int turn_count;

	turn_count = time_in_ml();
	while (time_in_ml() - turn_count < self->tte)
	{
		am_i_dead(self, 3);
		usleep(100);
	}
}

void  wanna_sleep(t_person *self)
{
	unsigned int turn_count;

	turn_count = time_in_ml();
	while (time_in_ml() - turn_count < self->tts)
	{
		am_i_dead(self, 4);
		usleep(100);
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
			(master[0])->fork_mate_i = f_count - 1;
			(master[0])->left_fork_mutex = ((master[f_count -1]->owner_fork_mutex));
		}
		else if (i == f_count - 1)
		{
			(master[i])->left_fork = &(master[0]->owner_fork);
			(master[i])->fork_mate_i = 0;
			(master[i])->left_fork_mutex = ((master[0]->owner_fork_mutex));
		}
		else
		{
			(master[i])->left_fork = &((master[i - 1])->owner_fork);
			(master[i])->fork_mate_i = i - 1;
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
