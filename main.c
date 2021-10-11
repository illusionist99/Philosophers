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

    if (id == -1)
        return (current_timestamp() - ((bdyatlkhal9.tv_sec * 1000) + (bdyatlkhal9.tv_usec / 1000)));

    return (current_timestamp() - wise[id].start);
}


void    print_to_screen(size_t id, char *msg) {

    pthread_mutex_lock(&print);
    printf("%ld %ld %s\n", no_time(-1), id, msg);
    if (strncmp(msg, "is dead", (long unsigned int)7) != 0)
        pthread_mutex_unlock(&print);
}


void    eat(t_book *philo) {

    size_t id = philo->id;


    pthread_mutex_lock(&wise[(id - 1) % data->n].myfork);
    print_to_screen(id, "picked up a fork");
    if (data->n == 1)
        return ;
    pthread_mutex_lock(&wise[(id) % data->n].myfork);
    print_to_screen(id, "picked up a fork");

    print_to_screen(id, "is eating");
    wise[id].start = current_timestamp();
    usleep(data->time_to_eat);


    pthread_mutex_lock(&inc_meal);
    if (philo->n_meals < data->meals)
        wise[id].n_meals++;
    pthread_mutex_unlock(&inc_meal);

    pthread_mutex_unlock(&wise[(id) % data->n].myfork);
    print_to_screen(id, "released a fork");
    
    pthread_mutex_unlock(&wise[(id - 1) % data->n].myfork);
    print_to_screen(id, "released a fork");

}



void    *routine( void *arg ) 
{
    t_book *philo = (t_book *)arg;

    wise[philo->id].start = current_timestamp();

    while ((philo->n_meals <= data->meals) && (flag == 0))
    {
        eat(philo);
        sleeper(philo);
        print_to_screen(philo->id, "is thinking");
    }
    return arg;
}


int    check_philo(t_book *philo) {

    size_t id = philo->id;

    if (no_time(id) >= data->time_to_die)
        return 1;
    return 0;
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
    gettimeofday(&bdyatlkhal9, NULL);
    while (i < data->n + 1)
    {
        wise[i - 1].n_meals = 0;
        wise[i - 1].id = i;
        wise[i - 1].flag = 0;
        wise[i - 1].start = current_timestamp();
        if (pthread_mutex_init(&(wise[i - 1].myfork), NULL) != 0)
            return ;
        if (pthread_create(&(wise[i - 1].philo), NULL, routine, &wise[i - 1]) != 0)
        {
            printf("Error philo\n");
            return ;
        }
        // delay
        usleep(100);
        i++;
    }
    int f = 0;
    int g = 0;
    while (!f) {
        
        i = 1;
        g = 0;
        while (i < data->n + 1) {
            if (wise[i - 1].n_meals == data->meals)
                g++;
            if (check_philo(&wise[i - 1]) == 1)
            {
                f = 1;
                break ;
            }
            i++;
        }
        if (g == data->meals)
            break ;
    }
    i = 1;
    while (i < data->n + 1) {
        if (pthread_mutex_destroy(&(wise[i - 1].myfork)) != 0)
            return ;
        i++;
    }
    pthread_mutex_destroy(&print);
    pthread_mutex_destroy(&inc_meal);
    free(wise);
}

int     main( int ac, char **av ) {

    if ( ac  == 6 || ac == 5)
        philo_func(++av);
    else
        printf("Error\n");
    return (0);
}
