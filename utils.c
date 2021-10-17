/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 20:55:59 by malaoui           #+#    #+#             */
/*   Updated: 2021/10/17 20:55:59 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_meals(void)
{
	size_t	i;

	i = 0;
	while (i < g_all.data->n)
	{
		if (g_all.wise[i].n_meals > g_all.data->meals + 1)
			return (1);
		i++;
	}
	return (0);
}

void	supervisor(void)
{
	size_t	i;
	int		f;

	i = 1;
	f = 0;
	while (!f)
	{
		i = 0;
		if ((g_all.ac == 6) && (all_meals() == 1))
		{
			f = 1;
			break ;
		}
		while (++i < g_all.data->n + 1)
		{
			if ((check_philo(&g_all.wise[i - 1]) == 1)
				&& !(g_all.wise[i - 1].is_eating))
			{
				f = 1;
				print_to_screen(i, "is dead");
				break ;
			}
		}
		usleep(5);
	}
}

void	clean(void)
{
	size_t	i;

	i = 1;
	while (i < g_all.data->n + 1)
	{
		if (pthread_mutex_destroy(&(g_all.wise[i - 1].myfork)) != 0)
			return ;
		i++;
	}
	pthread_mutex_destroy(&g_all.print);
	pthread_mutex_destroy(&g_all.inc_meal);
	free(g_all.wise);
}

void	eat(t_book *philo)
{
	size_t	id;

	id = philo->id;
	pthread_mutex_lock(&g_all.wise[(id) % g_all.data->n].myfork);
	print_to_screen(id, "picked up a fork");
	pthread_mutex_lock(&g_all.wise[(id + 1) % g_all.data->n].myfork);
	print_to_screen(id, "picked up a fork");
	g_all.wise[id].is_eating = true;
	print_to_screen(id, "is eating");
	g_all.wise[id].start = current_timestamp();
	usleeper2(g_all.data->time_to_eat);
	if (g_all.ac == 6)
	{
		pthread_mutex_lock(&g_all.inc_meal);
		(g_all.wise[id].n_meals)++;
		pthread_mutex_unlock(&g_all.inc_meal);
	}
	pthread_mutex_unlock(&g_all.wise[(id) % g_all.data->n].myfork);
	pthread_mutex_unlock(&g_all.wise[(id + 1) % g_all.data->n].myfork);
	g_all.wise[id].is_eating = false;
}
