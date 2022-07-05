#include "philo.h"

void *create_person(void *person_from_th_func)
{
	t_person *self_person;

	self_person = (t_person *)person_from_th_func;
	self_person->time_now = the_counter(self_person->old_time);
	while (1)
	{
		
		while (!eat_food2(self_person))
			{
				put_that_fork_back(self_person);
				am_i_dead(self_person, 1);
			}
		//am_i_dead(self_person);
		printf("%u %d is eating\n", the_counter(self_person->old_time), self_person->owner_id);
		self_person->time_now = the_counter(self_person->old_time);
		lets_eat(self_person);
		put_that_fork_back(self_person);
		printf("%u %d is sleeping\n", the_counter(self_person->old_time), self_person->owner_id);
		wanna_sleep(self_person);
		printf("%u %d is thinking\n", the_counter(self_person->old_time), self_person->owner_id);
		
	}
	return (NULL);
}

int	check_argument(int ac, char **av)
{
	int	i;
	int	y;

	i = 1;
	while (i <= ac - 1)
	{
		y = 0;
		while (av[i][y])
		{
			if (!(av[i][y] <= '9' && av[i][y] >= '0'))
				return (0);
			y++;
		}
		i++;
	}
	return (1);
}

void init_mutex(t_person **master, int fc)
{
	int i;

	i = 0;
	while (i < fc)
	{
		(master[i])->owner_fork_mutex = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((master[i])->owner_fork_mutex, NULL);
		i++;
	}
}

void start_threads(t_person **master, int philo_count)
{
	int i;
	unsigned int oldtime;
	
	oldtime = time_in_ml();
	i = 0;
	while (i < philo_count)
	{
		master[i]->old_time = oldtime;
		master[i]->time_now = time_in_ml();
		pthread_create(&((master[i])->owner_thread_id), NULL, &create_person, (void *)master[i]);
		i = i + 2;
	}
	usleep(1000);
	i = 1;
	while (i < philo_count)
	{
		master[i]->old_time = oldtime;
		master[i]->time_now = time_in_ml();
		pthread_create(&((master[i])->owner_thread_id), NULL, &create_person, (void *)master[i]);
		i = i + 2;
	}
	i = 0;
	while (i < philo_count)
	{
	 	pthread_join((master[i])->owner_thread_id, NULL);
	 	i++;
	}
}




int main(int argc, char *argv[])
{
	t_person		**all_persons;
	int				there_is_victim;
	pthread_mutex_t	*is_there_victim_m;
	pthread_mutex_t *table_mutex;
	is_there_victim_m = malloc(sizeof(pthread_mutex_t));
	table_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(is_there_victim_m, NULL);
	pthread_mutex_init(table_mutex, NULL);
	there_is_victim = 0;
	int i = 0;
	if (argc == 5 || argc == 6)
	{
		if (check_argument(argc, argv))
		{
			all_persons = create_persons_class(argv, &there_is_victim, is_there_victim_m, table_mutex);
			init_mutex(all_persons, ft_atoi(argv[1]));
			start_threads(all_persons, ft_atoi(argv[1]));
			// while (1)
			// {
			// 	usleep(5000);
			// 	current_time = time_in_ml();
			// 	if (current_time - all_persons[i]->time_now >= all_persons[i]->ttd / 1000)
			// 	{
			// 		printf("%u %d is dead\n", current_time - all_persons[i]->time_now, i + 1);
			// 		//all_over(all_persons, ft_atoi(argv[1]));
			// 		pthread_mutex_destroy(is_there_victim_m);
			// 		//free(is_there_victim_m);
			// 		return (0);
			// 	}
			// 	i++;
			// 	if (i == ft_atoi(argv[1]))
			// 		i = 0;
			// }
		}
	}
	return (0);
}
