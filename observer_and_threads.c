/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer_and_threads.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:51:03 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 15:28:15 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*observer(t_philo *philos)
{
	int				i;

	while (1)
	{
		i = 0;
		while (i < philos->number_of_philosophers)
		{
			obs_1(philos, i);
			pthread_mutex_lock(&philos[i].time_mutex);
			if (philos[i].timesinceeaten >= philos[i].time_to_die)
			{
				pthread_mutex_unlock(&philos[i].time_mutex);
				pthread_mutex_lock(philos->shared_dead_mutex);
				pthread_mutex_lock(philos->write_lock);
				obs_2(philos, i);
				pthread_mutex_unlock(philos->shared_dead_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i].time_mutex);
			usleep(20);
			i++;
		}
	}
	return (NULL);
}

void	obs_1(t_philo *philos, int i)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philos[i].time_mutex);
	philos[i].current_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	philos[i].timesinceeaten = philos[i].current_time
		- philos[i].last_eaten_time;
	pthread_mutex_unlock(&philos[i].time_mutex);
}

void	obs_2(t_philo *philos, int i)
{
	pthread_mutex_lock(&philos[i].meal_mutex);
	if (*(philos[i].shared_dead) == 1
		|| philos[i].meals_eaten == philos[i].target)
	{
		pthread_mutex_unlock(&philos[i].meal_mutex);
		return ;
	}
	*(philos[i].shared_dead) = 1;
	pthread_mutex_unlock(&philos[i].meal_mutex);
	printf("%.lld Philo %d died\n", philos[i].current_time
		- philos[i].last_eaten_time, i + 1);
	return ;
}

void	startandend(t_philo *philos, int number_of_philosophers,
		pthread_mutex_t *write_lock)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		philos[i].philosopher_number = i + 1;
		pthread_create(&philos->threads[i], NULL, threadfunction, &philos[i]);
		i++;
	}
	observer(philos);
	pthread_mutex_unlock(write_lock);
	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_join(philos->threads[i], NULL);
		i++;
	}
}

void	free_call(t_philo *philos, int number_of_philosophers,
		pthread_mutex_t *forks, pthread_mutex_t shared_dead_mutex)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&philos[i].time_mutex);
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(philos->write_lock);
	pthread_mutex_destroy(&shared_dead_mutex);
	free(philos->threads);
	free(philos);
	free(forks);
}
