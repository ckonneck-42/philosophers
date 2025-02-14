/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:46:25 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 14:52:02 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*threadfunction(void *arg)
{
	t_philo			*philos;
	struct timeval	tv;

	philos = (t_philo *)arg;
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philos->time_mutex);
	philos->starttime = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	pthread_mutex_unlock(&philos->time_mutex);
	while (1)
	{
		if (reaperwatch(philos) == 1)
			return (NULL);
		sort_forks(philos);
		if (take_and_eat(philos) == 1)
			return (NULL);
		if (write_function("%.lld %d is sleeping\n", philos,
				philos->philonum) == 1)
			return (NULL);
		usleep(philos->time_to_sleep * 1000);
		if (write_function("%.lld %d is thinking\n", philos,
				philos->philonum) == 1)
			return (NULL);
		if (philos->number_of_philosophers % 2 != 0)
			usleep(1000);
	}
}

void	sort_forks(t_philo *philos)
{
	philos->philonum = philos->philosopher_number;
	philos->left_fork = philos->philonum - 1;
	philos->right_fork = philos->philonum % philos->number_of_philosophers;
	if (philos->left_fork < philos->right_fork)
	{
		philos->first_fork = philos->left_fork;
		philos->second_fork = philos->right_fork;
	}
	else
	{
		philos->first_fork = philos->right_fork;
		philos->second_fork = philos->left_fork;
	}
	if (philos->philonum % 2 == 0 && philos->philonum != 1)
		usleep(100);
}

int	take_and_eat(t_philo *philos)
{
	pthread_mutex_lock(&philos->forks[philos->first_fork]);
	if (philos->number_of_philosophers == 1)
	{
		pthread_mutex_unlock(&philos->forks[philos->first_fork]);
		return (1);
	}
	pthread_mutex_lock(&philos->forks[philos->second_fork]);
	if (write_function("%.lld %d is eating\n", philos, philos->philonum) == 1)
	{
		pthread_mutex_unlock(&philos->forks[philos->second_fork]);
		pthread_mutex_unlock(&philos->forks[philos->first_fork]);
		return (1);
	}
	time_update(philos);
	usleep(philos->time_to_eat * 1000);
	pthread_mutex_lock(&philos->meal_mutex);
	philos->meals_eaten++;
	pthread_mutex_unlock(&philos->meal_mutex);
	pthread_mutex_unlock(&philos->forks[philos->second_fork]);
	pthread_mutex_unlock(&philos->forks[philos->first_fork]);
	return (0);
}

int	reaperwatch(t_philo *philos)
{
	pthread_mutex_lock(philos->shared_dead_mutex);
	if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target)
	{
		pthread_mutex_unlock(philos->shared_dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philos->shared_dead_mutex);
	return (0);
}

int	write_function(char *str, t_philo *philos, int num)
{
	pthread_mutex_lock(philos->shared_dead_mutex);
	pthread_mutex_lock(philos->write_lock);
	if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target)
	{
		pthread_mutex_unlock(philos->write_lock);
		pthread_mutex_unlock(philos->shared_dead_mutex);
		return (1);
	}
	philos->timestamp = ft_timestamp(philos);
	printf(str, philos->timestamp, num);
	pthread_mutex_unlock(philos->write_lock);
	pthread_mutex_unlock(philos->shared_dead_mutex);
	return (0);
}
