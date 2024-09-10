/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 10:55:22 by ckonneck          #+#    #+#             */
/*   Updated: 2024/09/10 14:47:23 by ckonneck         ###   ########.fr       */
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

// int main() {
//     struct timeval tv;
//     float starttime = gettimeofday(&tv, NULL);
//     printf("Seconds: %ld\nMicroseconds: %ld\n", tv.tv_sec, tv.tv_usec);
//     return (0);
// }



void	*observer(void *arg)
{
	t_thread_data *data = (t_thread_data *)arg;
	t_philo *philos = data->philos;
	struct timeval tv;

	int num = data->philosopher_number;
	// printf("started observing Philo %d\n", num);
	while (1)
	{
		gettimeofday(&tv, NULL);
		double current_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		pthread_mutex_lock(&philos->time_mutex);
		philos->timesinceeaten = current_time - philos->last_eaten_time;
		
		// printf("time to die %f\n", philos->time_to_die);
		if(philos->timesinceeaten >= philos->time_to_die)
		{
			// printf("time since eaten %.3f\n", philos->timesinceeaten);
			philos->dead = 1;
			printf("%.3lld, Philo %d died\n",philos->timesinceeaten, num);
			//callfree
			exit(1);
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

	while (philos->dead != 1)
	{
		gettimeofday(&tv, NULL);
		philos->starttime = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		int num = data->philosopher_number;
		// printf("Philosopher %d is thinking\n", num);

		int left_fork = num - 1;
		int right_fork = num % philos->number_of_philosophers;

		// Pick up forks
		if (num % 2 == 0)
				usleep(100);
		pthread_mutex_lock(&philos->forks[left_fork]);
		philos->timestamp = ft_timestamp(philos);
		printf("%.0f, Philosopher %d has taken fork %d\n", philos->timestamp, num, left_fork);
		pthread_mutex_lock(&philos->forks[right_fork]);
		printf("Philosopher %d has taken fork %d\n", num, right_fork);

		// Eating
		gettimeofday(&tv, NULL);
		pthread_mutex_lock(&philos->time_mutex);
		philos->last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		printf("time since eaten %lld\n", philos->timesinceeaten);
		pthread_mutex_unlock(&philos->time_mutex);
		
		philos->timestamp = ft_timestamp(philos);
		printf("%.0f, Philosopher %d is eating\n", philos->timestamp, num);
		usleep(philos->time_to_eat * 1000);
		philos->timestamp = ft_timestamp(philos);
		printf("%.0f, Philosopher %d has finished eating\n",philos->timestamp, num);
		
		
		// Put down forks
		
		
		pthread_mutex_unlock(&philos->forks[right_fork]);
		printf("Philosopher %d put down fork %d\n", num, right_fork);
		pthread_mutex_unlock(&philos->forks[left_fork]);
		printf("Philosopher %d put down fork %d\n", num, left_fork);
		philos->timestamp = ft_timestamp(philos);
		printf("%.0f, Philosopher %d is sleeping\n",philos->timestamp, num);
		usleep(philos->time_to_sleep * 1000);
		printf("Philosopher %d woke up\n", num);
		philos->timestamp = ft_timestamp(philos);
		printf("%.0f, Philosopher %d is thinking\n",philos->timestamp, num);
	}
	
	return (NULL);
}

int	main(int argc, char **argv)
{
	int i;
	if (argc != 5)
	{
		printf("usage: ./philo \"number of philosophers\" \"time to die \"");
		printf("\"time to eat\" \"time to sleep\" \n"); 
		exit(1);
	}

	int number_of_philosophers = ft_atoi(argv[1]);
	t_philo *philos = malloc(sizeof(t_philo) * number_of_philosophers);
	threads = malloc(sizeof(pthread_t) * number_of_philosophers);
    observer_threads = malloc(sizeof(pthread_t) * number_of_philosophers);
 	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * number_of_philosophers);
	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);  // Initialize each fork
		i++;
	}
    struct timeval tv;
	
	i = 0;
	pthread_mutex_init(&philos->time_mutex, NULL);
	while(i < number_of_philosophers)
	{
		
		philos[i].number_of_philosophers = ft_atoi(argv[1]);
		philos[i].number_of_forks = ft_atoi(argv[1]);
		philos[i].time_to_die = ft_atoi(argv[2]) ;
		philos[i].time_to_eat = ft_atoi(argv[3]) ;
		philos[i].time_to_sleep = ft_atoi(argv[4]);
		philos[i].forks = forks;
		philos[i].dead = 0;
		gettimeofday(&tv, NULL);
   		philos[i].last_eaten_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
		
		i++;
	}

	i = 0;
	while (i < number_of_philosophers)
	{
		t_thread_data *data = malloc(sizeof(t_thread_data));
		data->philosopher_number = i + 1;
		data->philos = &philos[i];
		pthread_create(&observer_threads[i], NULL, observer, data);
		pthread_create(&threads[i], NULL, threadfunction, data);
		i++;
	}
	i = 0;
	while (i < number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		pthread_join(observer_threads[i], NULL);
		i++;
	}
		
	printf("Thread has finished execution\n");

	for (int i = 0; i < number_of_philosophers; i++) {
        pthread_mutex_destroy(&philos->forks[i]);
    }

	free(threads);
    free(observer_threads);
    free(philos);
	return (0);
}

// ◦ timestamp_in_ms X has taken a fork
// ◦ timestamp_in_ms X is eating
// ◦ timestamp_in_ms X is sleeping
// ◦ timestamp_in_ms X is thinking
// ◦ timestamp_in_ms X died

long long ft_timestamp(t_philo *philos)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	philos->current_time = tv.tv_sec * 1000.0 + (tv.tv_usec / 1000.0);
	philos->timestamp = philos->current_time - philos->starttime;
	return(philos->timestamp);
}