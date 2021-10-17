/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaoui <malaoui@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 21:01:15 by malaoui           #+#    #+#             */
/*   Updated: 2021/10/17 21:15:16 by malaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

double  current_timestamp(void)
{
    struct timeval  watch;

	gettimeofday(&watch, NULL);
	return ((watch.tv_sec * 1000 ) + (watch.tv_usec / 1000));
}

void    usleeper2(double time)
{
    double  now;

    now = current_timestamp();
    usleep(time * 1000 * 0.85);
    while(current_timestamp() < now + time)
        continue;
}

void    sleeper( t_book *philo)
{
    print_to_screen(philo->id, "is sleeping");
    usleeper2(all.data->time_to_sleep);
}

double  no_time(int id)
{
    if (id == -1)
        return (current_timestamp() -
            ((all.startp.tv_sec  * 1000) + (all.startp.tv_usec / 1000)));
    return (current_timestamp() - all.wise[id].start);
}

void    print_to_screen(size_t id, char *msg)
{
    pthread_mutex_lock(&all.print);
    printf("%d %ld %s\n", (int)no_time(-1), id, msg);
    if (strncmp(msg, "is dead", (long unsigned int)7) != 0)
        pthread_mutex_unlock(&all.print);
}
