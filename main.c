# include "philo.h"

void    print_to_screen(size_t id, char *msg);


double        current_timestamp(void)
{
	struct timeval	watch;

	gettimeofday(&watch, NULL);
	return ((watch.tv_sec * 1000 ) + (watch.tv_usec / 1000));
}

void    sleeper( t_book *philo)
{

    usleep( all.data->time_to_sleep );
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
    int x;


    pthread_mutex_lock(&all.wise[(id + 1) % all.data->n].myfork);
    print_to_screen(id, "picked up a fork");

    pthread_mutex_lock(&all.wise[(id) % all.data->n].myfork);
    print_to_screen(id, "picked up a fork");

    all.wise[id].is_eating = true;
    
    print_to_screen(id, "is eating");
    all.wise[id].start = current_timestamp();

    usleep( all.data->time_to_eat );

    pthread_mutex_lock(&all.inc_meal);
    all.wise[id].n_meals++;
    pthread_mutex_unlock(&all.inc_meal);

    pthread_mutex_unlock(&all.wise[(id) % all.data->n].myfork);
    print_to_screen(id, "released a fork");
    
    pthread_mutex_unlock(&all.wise[(id + 1) % all.data->n].myfork);
    print_to_screen(id, "released a fork");
    all.wise[id].is_eating = false;
}

int    check_philo(t_book *philo) {

    size_t id = philo->id;

    if ( no_time(id) >= all.data->time_to_die )
        return 1;
    return 0;
}


void    *routine( void *arg ) 
{

    t_book *philo = (t_book *)arg;
    usleep(60);
    all.wise[philo->id].start = current_timestamp();
    all.wise[philo->id].is_eating = false;

    while (philo->n_meals < all.data->meals)
    {

        eat(philo);
    
        sleeper(philo);
    
        print_to_screen(philo->id, "is thinking");

    }
    return arg;
}



void    philo_func( char **av )  
{

    all.data = (t_philo *)malloc(sizeof(t_philo));

    all.data->n = atoi(av[0]);
    printf("Number of phiilosophers: %zu\n", all.data->n);

    all.data->time_to_die = atoi(av[1]);
    printf("time to die : %zu\n", all.data->time_to_die);

    all.data->time_to_eat = atoi(av[2]);
    printf("time to eat: %zu\n", all.data->time_to_eat);

    all.data->time_to_sleep = atoi(av[3]);
    printf("time to sleep : %zu\n", all.data->time_to_sleep);

    if (av[4]) {
        all.data->meals = atoi(av[4]);
        printf("Number of meals: %zu\n", all.data->meals);
    }
    else {
        all.data->meals = 1;
    }
    int i;

    i = 1;

    all.wise = (t_book *)malloc(sizeof(t_book) * (all.data->n + 1));
    pthread_mutex_init(&all.print, NULL);
    pthread_mutex_init(&all.inc_meal, NULL);
    gettimeofday(&all.bdyatlkhal9, NULL);
    i = 1;
    while (i < all.data->n + 1)
    {

        all.wise[i - 1].n_meals = 0;
        all.wise[i - 1].id = i;
        all.wise[i - 1].flag = 0;
        all.wise[i - 1].start = current_timestamp();
        all.wise[i - 1].is_eating = false;

        if (pthread_mutex_init(&(all.wise[i - 1].myfork), NULL) != 0)
            break ;
        i++;
    }
    i = 1;
    while (i < all.data->n + 1)
    {
        if (pthread_create(&(all.wise[i - 1].philo), NULL, routine, &all.wise[i - 1]) != 0)
        {
            printf("Error philo\n");
            break ;
        }
        usleep(60);
        i++;
    }

    i = 1;
    int f = 0; 

    while (!f) {

        i = 1;
        
        while (i < all.data->n + 1) {
    
            if ((check_philo(&all.wise[i - 1]) == 1) && !(all.wise[i - 1].is_eating))
            {
                f = 1;
                print_to_screen(i, "is dead");
                break ;
            }
            if ((all.wise[i - 1].n_meals == all.data->meals)  && (all.data->meals != 1))
                break ;
            i++;
        }
    }
    i = 1;
    while (i < all.data->n + 1) {
        if (pthread_mutex_destroy(&(all.wise[i - 1].myfork)) != 0)
            return ;
        i++;
    }
    pthread_mutex_destroy(&all.print);
    pthread_mutex_destroy(&all.inc_meal);
    free(all.wise);
}

int     main( int ac, char **av )   {

    if ( ac  == 6 || ac == 5 )
        philo_func(++av);
    else
        printf("Error\n");
    return (0);
}
