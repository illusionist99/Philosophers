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
	while (i < all.data->n)
	{
		if (all.wise[i].n_meals > all.data->meals + 1)
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
		if ((all.ac == 6) && (all_meals() == 1))
		{
			f = 1;
			break ;
		}
		while (++i < all.data->n + 1)
		{
			if ((check_philo(&all.wise[i - 1]) == 1)
				&& !(all.wise[i - 1].is_eating))
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
	while (i < all.data->n + 1)
	{
		if (pthread_mutex_destroy(&(all.wise[i - 1].myfork)) != 0)
			return ;
		i++;
	}
	pthread_mutex_destroy(&all.print);
	pthread_mutex_destroy(&all.inc_meal);
	free(all.wise);
}

void	eat(t_book *philo)
{
	size_t	id;

	id = philo->id;
	pthread_mutex_lock(&all.wise[(id) % all.data->n].myfork);
	print_to_screen(id, "picked up a fork");
	pthread_mutex_lock(&all.wise[(id + 1) % all.data->n].myfork);
	print_to_screen(id, "picked up a fork");
	all.wise[id].is_eating = true;
	print_to_screen(id, "is eating");
	all.wise[id].start = current_timestamp();
	usleeper2(all.data->time_to_eat);
	if (all.ac == 6)
	{
		pthread_mutex_lock(&all.inc_meal);
		(all.wise[id].n_meals)++;
		pthread_mutex_unlock(&all.inc_meal);
	}
	pthread_mutex_unlock(&all.wise[(id) % all.data->n].myfork);
	pthread_mutex_unlock(&all.wise[(id + 1) % all.data->n].myfork);
	all.wise[id].is_eating = false;
}
