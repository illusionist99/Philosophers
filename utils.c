# include "philo.h"


double        current_timestamp(void)
{
	struct timeval	watch;

	gettimeofday(&watch, NULL);
	return ((watch.tv_sec * 1000 ) + (watch.tv_usec / 1000));
}


void    sleeper( t_book *philo)
{

    usleep( all.data->time_to_sleep * 1000 );
    print_to_screen(philo->id, "is sleeping");

}

double    no_time(size_t id) {

    if (id == -1)
        return (current_timestamp() - ((all.bdyatlkhal9.tv_sec  * 1000) + (all.bdyatlkhal9.tv_usec / 1000)));

    return (current_timestamp() - all.wise[id].start);
}

void    print_to_screen(size_t id, char *msg) {

    pthread_mutex_lock(&all.print);
    printf("%d %ld %s\n", (int)no_time(-1), id, msg);
    if (strncmp(msg, "is dead", (long unsigned int)7) != 0)
        pthread_mutex_unlock(&all.print);
}

void    eat(t_book *philo) {

    size_t id = philo->id;

    pthread_mutex_lock(&all.wise[(id) % all.data->n].myfork);
    print_to_screen(id, "picked up a fork");

    pthread_mutex_lock(&all.wise[(id + 1) % all.data->n].myfork);
    print_to_screen(id, "picked up a fork");



    all.wise[id].is_eating = true;
    
    print_to_screen(id, "is eating");

    usleep( all.data->time_to_eat  * 1000);

    pthread_mutex_lock(&all.inc_meal);
    all.wise[id].n_meals++;
    pthread_mutex_unlock(&all.inc_meal);
    all.wise[id].start = current_timestamp();

    pthread_mutex_unlock(&all.wise[(id + 1) % all.data->n].myfork);
    print_to_screen(id, "released a fork");
    
    pthread_mutex_unlock(&all.wise[(id) % all.data->n].myfork);
    print_to_screen(id, "released a fork");
    all.wise[id].is_eating = false;
}

