#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>


typedef struct	s_person
{
	struct s_person	**master;
	int 			fork_mate_i;
	unsigned int	owner_id;
	unsigned int	old_time;
	unsigned int	time_now;
	unsigned int	time_end;
	unsigned int	ttd;
	unsigned int	tte;
	unsigned int	tts;
	unsigned int	fcount;
	unsigned int	eat_count;
	pthread_mutex_t *table_mutex;
	int				*there_is_victim;
	pthread_mutex_t *is_there_victim_p;
	int				owner_fork;
	pthread_mutex_t	*owner_fork_mutex;
	int				*left_fork;
	pthread_mutex_t *left_fork_mutex;
	pthread_t		owner_thread_id;
}				t_person;

unsigned int	the_counter(unsigned int old);
unsigned int	time_in_ml(void);
void am_i_dead(t_person *self_person, int id);
void eat_food(t_person *self_person);
void  wanna_sleep(t_person *self);
void  lets_eat(t_person *self);
int eat_food2(t_person *self);
t_person *put_information(int i, char **av, t_person **master, int *victim_pointer);
int	ft_atoi(const char *nptr);
t_person **create_persons_class(char **av, int *victim_pointer, pthread_mutex_t *is_there, pthread_mutex_t *table_mutex);
void all_over(t_person **master, int philo_count);
void put_that_fork_back(t_person *self_person);

#endif
