/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:55:53 by ckonneck          #+#    #+#             */
/*   Updated: 2024/09/10 14:43:54 by ckonneck         ###   ########.fr       */
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
	pthread_mutex_t *forks;
	pthread_mutex_t time_mutex;
	int			result;
	long long timesinceeaten;
	long long last_eaten_time;
	long long starttime;
	long long current_time;
	float timestamp;
	int		dead;
	pthread_mutex_t	dead_mutex;
}	t_philo;


typedef struct t_thread_data
{
 	t_philo *philos;
    int philosopher_number;
    // long long last_eaten_time;
    // long long timesinceeaten;
    // long long starttime;
    // float timestamp;
} t_thread_data;


pthread_t *threads;
pthread_t *observer_threads;


long long ft_timestamp(t_philo *philos);