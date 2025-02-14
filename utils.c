/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:49:03 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 14:52:04 by ckonneck         ###   ########.fr       */
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

void	errorcheck(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("usage: ./philo \"number of philosophers\" \"time to die\", ");
		printf("\"time to eat\", \"time to sleep\", \"number of meals\" \n");
		exit(1);
	}
	error2(argc, argv, i);
}

void	error2(int argc, char **argv, int i)
{
	int	k;

	k = 0;
	while (i < argc)
	{
		k = 0;
		while (argv[i][k])
		{
			if (ft_isdigit(argv[i][k]) == 0)
			{
				printf("only digits are accepted here\n");
				exit(1);
			}
			else
				k++;
		}
		i++;
	}
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

long long	ft_timestamp(t_philo *philos)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philos->time_mutex);
	philos->current_time = (long long)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	philos->timestamp = (long long)(philos->current_time - philos->starttime);
	pthread_mutex_unlock(&philos->time_mutex);
	return (philos->timestamp);
}
