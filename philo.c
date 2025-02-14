/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckonneck <ckonneck@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:23:32 by ckonneck          #+#    #+#             */
/*   Updated: 2024/11/07 15:33:15 by ckonneck         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	shared_dead_mutex;
	pthread_mutex_t	write_lock;
	int				*shared_dead;

	shared_dead = malloc(sizeof(int));
	*shared_dead = 0;
	errorcheck(argc, argv);
	philos = allocate_memory(ft_atoi(argv[1]), &forks);
	pthread_mutex_init(&write_lock, NULL);
	pthread_mutex_init(&shared_dead_mutex, NULL);
	philoinit1(philos, argv, ft_atoi(argv[1]), &shared_dead_mutex);
	philoinit2(philos, ft_atoi(argv[1]), &write_lock, shared_dead);
	philoinit3(philos, argc, argv, forks);
	startandend(philos, ft_atoi(argv[1]), &write_lock);
	free_call(philos, ft_atoi(argv[1]), forks, shared_dead_mutex);
	free(shared_dead);
}
