/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:42:36 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 16:14:57 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philoinit1(t_philo *philos, char **argv, int number_of_philosophers,
		pthread_mutex_t *shared_dead_mutex)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		pthread_mutex_init(&philos[i].time_mutex, NULL);
		philos[i].shared_dead_mutex = shared_dead_mutex;
		philos[i].number_of_philosophers = ft_atoi(argv[1]);
		philos[i].number_of_forks = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]);
		philos[i].time_to_eat = ft_atoi(argv[3]);
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		i++;
	}
}

void	philoinit2(t_philo *philos, int number_of_philosophers,
		pthread_mutex_t *write_lock, int *shared_dead)
{
	int	i;

	i = 0;
	while (i < number_of_philosophers)
	{
		philos[i].write_lock = write_lock;
		philos[i].shared_dead = shared_dead;
		philos[i].meals_eaten = 0;
		philos[i].timesinceeaten = 0;
		i++;
	}
}

void	philoinit3(t_philo *philos, int argc, char **argv,
		pthread_mutex_t *forks)
{
	int				i;
	int				number_of_philosophers;
	struct timeval	tv;

	i = 0;
	number_of_philosophers = ft_atoi(argv[1]);
	while (i < number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos[i].forks = forks;
		if (argc == 6)
			philos[i].target = ft_atoi(argv[5]);
		else
			philos[i].target = -1;
		gettimeofday(&tv, NULL);
		philos[i].last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		i++;
	}
}

t_philo	*allocate_memory(int number_of_philosophers, pthread_mutex_t **forks)
{
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * number_of_philosophers);
	if (!philos)
		return (NULL);
	philos->threads = malloc(sizeof(pthread_t) * number_of_philosophers);
	*forks = malloc(sizeof(pthread_mutex_t) * number_of_philosophers);
	if (!philos->threads || !(*forks))
	{
		free(philos->threads);
		free(philos);
		return (NULL);
	}
	return (philos);
}

void	time_update(t_philo *philos)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philos->time_mutex);
	philos->last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	pthread_mutex_unlock(&philos->time_mutex);
}
