/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:55:22 by ckonneck          #+#    #+#             */
/*   Updated: 2024/09/13 14:19:09 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	count;
	int	res;
	int	sign;

	count = 0;
	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		count++;
		str++;
	}
	if (count > 1)
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + *str - '0';
		str++;
	}
	return (res * sign);
}


void	*observer(void *arg)
{
	t_thread_data *data = (t_thread_data *)arg;
	t_philo *philos = data->philos;
	struct timeval tv;

	int num = data->philosopher_number;

	while (1)
	{
		gettimeofday(&tv, NULL);
		double current_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		pthread_mutex_lock(&philos->time_mutex);
		philos->timesinceeaten = current_time - philos->last_eaten_time;
		pthread_mutex_lock(philos->shared_dead_mutex);
    	if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target) 
		{
        	pthread_mutex_unlock(philos->shared_dead_mutex);
			pthread_mutex_unlock(&philos->time_mutex);
        	return NULL;
   		}
		pthread_mutex_unlock(philos->shared_dead_mutex);

		if(philos->timesinceeaten >= philos->time_to_die)
		{
			if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target)
			{
				pthread_mutex_unlock(&philos->time_mutex);
				return (NULL);
			}
			pthread_mutex_lock(philos->shared_dead_mutex);
            *philos->shared_dead = 1;
            pthread_mutex_unlock(philos->shared_dead_mutex);
			pthread_mutex_unlock(&philos->time_mutex);

			printf("%.3lld, Philo %d died\n",philos->timesinceeaten, num);
			return (NULL);
		}
		pthread_mutex_unlock(&philos->time_mutex);
		usleep(1000);
	}

	return(NULL);
}



void	*threadfunction(void *arg)
{
	
	t_thread_data *data = (t_thread_data *)arg;
	t_philo *philos = data->philos;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	philos->starttime = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	while (1)
	{
		pthread_mutex_lock(philos->shared_dead_mutex);
        if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target)
		{
        	pthread_mutex_unlock(philos->shared_dead_mutex);
        	return NULL;  // Stop this thread
        }
        pthread_mutex_unlock(philos->shared_dead_mutex);
		int num = data->philosopher_number;
		int left_fork = num - 1;
		int right_fork = num % philos->number_of_philosophers;
		if (num % 2 == 0)
				usleep(100);
		if (num == 1 && philos->number_of_philosophers % 2 == 1)
				usleep(200);
		// write_function("%.0f Philosopher %d has taken a fork\n", philos, num, data);
		// write_function("%.0f Philosopher %d has taken a fork\n", philos, num, data);
		select_fork(philos, &philos->forks[right_fork], &philos->forks[left_fork], num);
		take_chopsticks(philos, &philos->forks[right_fork], &philos->forks[left_fork], num, data);
		gettimeofday(&tv, NULL);
		pthread_mutex_lock(&philos->time_mutex);
		philos->last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		pthread_mutex_unlock(&philos->time_mutex);
		write_function("%.0f Philosopher %d is eating\n", philos, num, data);
		if (*philos->shared_dead != 1)
			usleep(philos->time_to_eat * 1000);
		write_function("%.0f Philosopher %d has finished eating\n",philos, num, data);
		philos->meals_eaten++;
		
		pthread_mutex_unlock(&philos->forks[right_fork]);
		pthread_mutex_unlock(&philos->forks[left_fork]);
		write_function("%.0f Philosopher %d is sleeping\n",philos, num, data);
		if (*philos->shared_dead != 1)
			usleep(philos->time_to_sleep * 1000);
		// philos->timestamp = ft_timestamp(philos);
		write_function("%.0f Philosopher %d is thinking\n",philos, num, data);
	}
	
	return (NULL);
}

int	main(int argc, char **argv)
{
	int i;
	if (argc != 5 && argc != 6)
	{
		printf("usage: ./philo \"number of philosophers\" \"time to die \"");
		printf("\"time to eat\" \"time to sleep\", \"number of meals or nothing\" \n"); 
		exit(1);
	}

	int number_of_philosophers = ft_atoi(argv[1]);
	t_philo *philos = malloc(sizeof(t_philo) * number_of_philosophers);
	philos->threads = malloc(sizeof(pthread_t) * number_of_philosophers);
    philos->observer_threads = malloc(sizeof(pthread_t) * number_of_philosophers);
 	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * number_of_philosophers);

    struct timeval tv;
	int *shared_dead = malloc(sizeof(int));  // Shared flag for death
    *shared_dead = 0;
	i = 0;
	
	pthread_mutex_t shared_dead_mutex;
    pthread_mutex_init(&shared_dead_mutex, NULL);
	while(i < number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		pthread_mutex_init(&philos[i].time_mutex, NULL);
		pthread_mutex_init(&philos[i].write_lock, NULL);
		philos[i].shared_dead = shared_dead;  // Shared dead flag
        philos[i].shared_dead_mutex = &shared_dead_mutex;
		philos[i].number_of_philosophers = ft_atoi(argv[1]);
		philos[i].number_of_forks = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]) ;
		philos[i].time_to_eat = ft_atoi(argv[3]) ;
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		philos[i].forks = forks;
		philos[i].meals_eaten = 0;
		if (argc == 6)
			philos[i].target = ft_atoi(argv[5]);
		else
			philos[i].target = -1;

			
		gettimeofday(&tv, NULL);
   		philos[i].last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		
		i++;
	}

	i = 0;
	t_thread_data **thread_data = malloc(sizeof(t_thread_data *) * number_of_philosophers);
	while (i < number_of_philosophers)
	{
		thread_data[i] = malloc(sizeof(t_thread_data));
		thread_data[i]->philosopher_number = i + 1;
		thread_data[i]->philos = &philos[i];
		pthread_create(&philos->observer_threads[i], NULL, observer, thread_data[i]);
		pthread_create(&philos->threads[i], NULL, threadfunction, thread_data[i]);
		i++;
	}
	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_join(philos->threads[i], NULL);
		pthread_join(philos->observer_threads[i], NULL);
		free(thread_data[i]);
		i++;
	}
	free(thread_data);
	printf("all threads have finished execution\n");

	for (int i = 0; i < number_of_philosophers; i++) {
        pthread_mutex_destroy(&philos[i].forks[i]);
		pthread_mutex_destroy(&philos[i].write_lock);
		pthread_mutex_destroy(&philos[i].time_mutex); 
		
    }
	pthread_mutex_destroy(&shared_dead_mutex);
	free(shared_dead);
	free(philos->threads);
   	free(philos->observer_threads);
    free(philos);
	free(forks);

	
	return (0);
}


long long ft_timestamp(t_philo *philos)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	philos->current_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	philos->timestamp = philos->current_time - philos->starttime;
	return(philos->timestamp);
}

void write_function(char *str, t_philo *philos, int num, t_thread_data *data)
{
	pthread_mutex_lock(philos->shared_dead_mutex);
	pthread_mutex_lock(&philos->write_lock);
	philos->timestamp = ft_timestamp(philos);
	if (*philos->shared_dead == 1 || philos->meals_eaten == philos->target)
	{
		pthread_mutex_unlock(philos->shared_dead_mutex);
		pthread_mutex_unlock(&philos->write_lock);
		return;
	}
	printf(str, philos->timestamp, num);
	pthread_mutex_unlock(&philos->write_lock);
	pthread_mutex_unlock(philos->shared_dead_mutex);
}


void	select_fork(t_philo *philos, \
					pthread_mutex_t *first, pthread_mutex_t *second, int num)
{

	int left_fork = num - 1;
	int right_fork = num % philos->number_of_philosophers;
	if (num == 1)
	{
		first = &philos->forks[left_fork];
		second = &philos->forks[right_fork];
	}
	else if (num % 2 == 0)
	{
		first = &philos->forks[left_fork];
		second = &philos->forks[right_fork];
	}
	else if (num % 2 == 1 && num % 2 == 0)
	{
		first = &philos->forks[right_fork];
		second = &philos->forks[left_fork];
	}
	else
	{
		first = &philos->forks[left_fork];
		second = &philos->forks[right_fork];
	}
}

void	take_chopsticks(t_philo *philo, \
					pthread_mutex_t *first, pthread_mutex_t *second, int num, t_thread_data *data)
{
	pthread_mutex_lock(first);
	write_function("%.0f Philosopher %d has taken a fork\n", philo, num, data);
	pthread_mutex_lock(second);
	write_function("%.0f Philosopher %d has taken a fork\n", philo, num, data);
}