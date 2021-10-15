# include "philo.h"


int    check_philo(t_book *philo) {

    size_t id = philo->id;

    if ( no_time(id) >= all.data->time_to_die  && !(all.wise[id].is_eating) )
        return 1;
    return 0;
}


void    *routine( void *arg ) 
{

    t_book *philo = (t_book *)arg;

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

void    init_data( char **av) {

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
    all.wise = (t_book *)malloc(sizeof(t_book) * (all.data->n + 1));
    pthread_mutex_init(&all.print, NULL);
    pthread_mutex_init(&all.inc_meal, NULL);
    gettimeofday(&all.bdyatlkhal9, NULL);
    int i;

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
}

void    supervisor(void) {

    int i;

    i = 1;
    int f = 0; 

    while (!f) {

        i = 1;
        
        while (i < all.data->n + 1) {
    
            if (check_philo(&all.wise[i - 1]) == 1)
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
}

void    clean( void ) {

    int i;

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


void    philo_func( char **av )  
{
    int i;
    init_data(av);

    i = 1;
    while (i < all.data->n + 1)
    {
        if (pthread_create(&(all.wise[i - 1].philo), NULL, routine, &all.wise[i - 1]) != 0)
        {
            printf("Error philo\n");
            break ;
        }
        usleep(60);
        pthread_detach((all.wise[i - 1].philo));
        i++;
    }

    supervisor();
    clean();
}
