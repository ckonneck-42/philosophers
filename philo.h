/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:55:53 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 15:44:17 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct t_philo
{
	int				number_of_philosophers;
	int				number_of_forks;
	int				philonum;
	int				left_fork;
	int				right_fork;
	int				first_fork;
	int				second_fork;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	pthread_t		*threads;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	meal_mutex;
	long long		timesinceeaten;
	long long		last_eaten_time;
	long long		starttime;
	long long		current_time;
	long long		timestamp;
	int				meals_eaten;
	int				target;
	int				*shared_dead;
	int				philosopher_number;
	pthread_mutex_t	*shared_dead_mutex;

}					t_philo;

typedef struct t_thread_data
{
	t_philo			*philos;
	int				philosopher_number;
}					t_thread_data;

long long			ft_timestamp(t_philo *philos);
int					write_function(char *str, t_philo *philos, int num);
void				time_update(t_philo *philos);
int					reaperwatch(t_philo *philos);
void				sort_forks(t_philo *philos);
int					take_and_eat(t_philo *philos);
void				obs_1(t_philo *philos, int i);
void				obs_2(t_philo *philos, int i);
void				free_call(t_philo *philos, int number_of_philosophers,
						pthread_mutex_t *forks,
						pthread_mutex_t shared_dead_mutex);
int					ft_isdigit(int c);
void				errorcheck(int argc, char **argv);
void				error2(int argc, char **argv, int i);
t_philo				*allocate_memory(int number_of_philosophers,
						pthread_mutex_t **forks);
void				philoinit1(t_philo *philos, char **argv,
						int number_of_philosophers,
						pthread_mutex_t *shared_dead_mutex);
void				philoinit2(t_philo *philos, int number_of_philosophers,
						pthread_mutex_t *write_lock, int *shared_dead);
void				philoinit3(t_philo *philos, int argc, char **argv,
						pthread_mutex_t *forks);
void				*threadfunction(void *arg);
int					ft_atoi(const char *str);
void				startandend(t_philo *philos, int number_of_philosophers,
						pthread_mutex_t *write_lock);