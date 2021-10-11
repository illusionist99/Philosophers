# include "philo.h"

// start at 1 



// eat
// sleep 
// think

void    print_to_screen(size_t id, char *msg);



long        current_timestamp(void)
{
	struct timeval	watch;

	gettimeofday(&watch, NULL);
	return ((watch.tv_sec * 1000) + (watch.tv_usec / 1000));
}

void    sleeper( t_book *philo)
{
    usleep(data->time_to_sleep);
    print_to_screen(philo->id, "is sleeping");
}

long    no_time(size_t id) {

    return (wise[id].start - current_timestamp());
}


void    print_to_screen(size_t id, char *msg) {

    pthread_mutex_lock(&print);
    printf("%ld %ld %s\n", no_time(id), id, msg);
    pthread_mutex_unlock(&print);
}


void    eat(t_book *philo) {

    size_t id = philo->id;

    pthread_mutex_lock(&wise[id].myfork);
    print_to_screen(id, "picked up a fork");
    pthread_mutex_lock(&wise[(id + 1) % data->n].myfork);
    print_to_screen(id, "picked up a fork");

    usleep(data->time_to_eat);
    print_to_screen(id, "is eating");
    pthread_mutex_lock(&inc_meal);
    wise[id].n_meals++;
    pthread_mutex_unlock(&inc_meal);
    wise[id].start = current_timestamp();

    pthread_mutex_unlock(&wise[id].myfork);
    print_to_screen(id, "released a fork");
    pthread_mutex_unlock(&wise[(id + 1) % data->n].myfork);
    print_to_screen(id, "released a fork");
}



void    *routine( void *arg ) 
{
    t_book *philo = (t_book *)arg;

    usleep(100);
    while ((philo->n_meals < data->meals) && (philo->flag == 0))
    {
        if (no_time(philo->id) > data->time_to_die) {
            philo->flag = 1;
            break ;
        }
        eat(philo);
        sleeper(philo);
        print_to_screen(philo->id, "is thinking");
    }
    if (philo->flag == 1) {
        print_to_screen(philo->id, "is dead");
        philo->flag++;
    }
    return arg;
}

void    philo_func( char **av )  
{

    data = (t_philo *)malloc(sizeof(t_philo));

    data->n = atoi(av[0]);
    printf("Number of phiilosophers: %zu\n", data->n);

    data->time_to_die = atoi(av[1]);
    printf("time to die : %zu\n", data->time_to_die);

    data->time_to_eat = atoi(av[2]);
    printf("time to eat: %zu\n", data->time_to_eat);

    data->time_to_sleep = atoi(av[3]);
    printf("time to sleep : %zu\n", data->time_to_sleep);

    if (av[4]) {
        data->meals = atoi(av[4]);
        printf("Number of meals: %zu\n", data->meals);
    }

    int i;

    i = 1;

    wise = (t_book *)malloc(sizeof(t_book) * (data->n + 1));
    pthread_mutex_init(&print, NULL);
    pthread_mutex_init(&inc_meal, NULL);
    while (i <= data->n)
    {
        wise[i].n_meals = 0;
        wise[i].id = i;
        wise[i].flag = 0;
        if (pthread_mutex_init(&(wise[i].myfork), NULL) != 0)
            return ;
        wise[i].start = current_timestamp();
        if (pthread_create(&(wise[i].philo), NULL, routine, &wise[i]) != 0)
        {
            printf("Error philo\n");
            return ;
        }
        i++;
    }

    i = 1;
    while (i <= data->n) {
        pthread_join(wise[i].philo, NULL);
        if (pthread_mutex_destroy(&(wise[i].myfork)) != 0)
            return ;
        i++;
    }
    pthread_mutex_destroy(&inc_meal);
    pthread_mutex_destroy(&print);
    free(wise);
}

int     main( int ac, char **av ) {

    if ( ac  == 6 || ac == 5)
        philo_func(++av);
    else
        printf("Error\n");
    return (0);
}
