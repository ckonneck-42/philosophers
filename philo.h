/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:55:53 by ckonneck          #+#    #+#             */
/*   Updated: 2024/09/13 14:09:59 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


typedef struct t_philo
{
	int			number_of_philosophers;
	int			number_of_forks;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	pthread_t	*threads;
	pthread_t	*observer_threads;
	pthread_mutex_t	write_lock;
	pthread_mutex_t *forks;
	pthread_mutex_t time_mutex;
	int			result;
	long long timesinceeaten;
	long long last_eaten_time;
	long long starttime;
	long long current_time;
	float timestamp;
	int		meals_eaten;
	int		target;
	int *shared_dead;
	pthread_mutex_t	*shared_dead_mutex;
}	t_philo;


typedef struct t_thread_data
{
 	t_philo *philos;
    int philosopher_number;
} t_thread_data;

long long ft_timestamp(t_philo *philos);
void write_function(char *str, t_philo *philos, int num, t_thread_data *data);

void	select_fork(t_philo *philos, \
					pthread_mutex_t *first, pthread_mutex_t *second, int num);
void	take_chopsticks(t_philo *philo, \
					pthread_mutex_t *first, pthread_mutex_t *second, int num, t_thread_data *data);