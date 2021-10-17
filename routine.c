/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 21:01:15 by malaoui           #+#    #+#             */
/*   Updated: 2021/10/17 21:01:15 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"philo.h"

int	check_philo(t_book *philo)
{
	size_t	id;

	id = philo->id;
	if (no_time(id) > all.data->time_to_die)
		return (1);
	return (0);
}

void	*routine(void *arg)
{
	t_book	*philo;

	philo = (t_book *)arg;
	if (all.ac == 5)
		all.wise[philo->id].n_meals = 1;
	all.wise[philo->id].start = current_timestamp();
	while (true)
	{
		all.wise[philo->id].is_eating = false;
		eat(philo);
		sleeper(philo);
		print_to_screen(philo->id, "is thinking");
	}
	return (arg);
}

void	small_init(char **av)
{
	all.data = (t_philo *)malloc(sizeof(t_philo));
	all.data->n = atoi(av[0]);
	all.data->time_to_die = atoi(av[1]);
	all.data->time_to_eat = atoi(av[2]);
	all.data->time_to_sleep = atoi(av[3]);
	if (av[4])
		all.data->meals = atoi(av[4]);
	else
		all.data->meals = 1;
	all.wise = (t_book *)malloc(sizeof(t_book) * (all.data->n + 1));
	pthread_mutex_init(&all.print, NULL);
	pthread_mutex_init(&all.inc_meal, NULL);
	gettimeofday(&all.startp, NULL);
}

void	init_data(char **av)
{
	size_t	i;

	i = 1;
	small_init(av);
	while (i < all.data->n + 1)
	{
		all.wise[i - 1].n_meals = 1;
		all.wise[i - 1].id = i;
		all.wise[i - 1].flag = 0;
		all.wise[i - 1].is_eating = false;
		if (pthread_mutex_init(&(all.wise[i - 1].myfork), NULL) != 0)
			break ;
		i++;
	}
}

void	philo_func(char **av)
{
	size_t	i;

	i = 1;
	init_data(av);
	if ((all.data->n == 0) || (all.data->meals == 0))
	{
		printf("Error\n");
		return ;
	}
	while (i < all.data->n + 1)
	{
		if (pthread_create(&(all.wise[i - 1].philo),
				NULL, routine, &all.wise[i - 1]) != 0)
		{
			printf("Error philo\n");
			break ;
		}
		pthread_detach((all.wise[i - 1].philo));
		usleep(60);
		i++;
	}
	supervisor();
	clean();
}
