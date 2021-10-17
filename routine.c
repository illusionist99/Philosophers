# include "philo.h"


int    check_philo(t_book *philo) {

    size_t id = philo->id;

    if ( no_time(id) > all.data->time_to_die )
        return 1;

    return 0;
}


void    *routine( void *arg ) 
{

    t_book *philo = (t_book *)arg;
    if (all.ac == 5)
        all.wise[philo->id].n_meals = 1;
    while ( true )
    {    
        
        all.wise[philo->id].is_eating = false;
        all.wise[philo->id].start = current_timestamp();
        
        eat(philo);
    
        sleeper(philo);
    
        print_to_screen(philo->id, "is thinking");
    }
    return arg;
}

void    init_data( char **av) {

    int i;


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
    i = 1;
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

int     all_meals( void )
{
    int i;

    i = 0;
    while (i < all.data->n )
    {
        if (all.wise[i].n_meals > all.data->meals + 1)
            return (1);
        i++;
    }
    return (0);
}

void    supervisor( void )
{
    int i;
    int f;

    i = 1;
    f = 0; 
    while (!f)
    {
        i = 1;
        if ((all.ac == 6) && (all_meals() == 1))
        {
            f = 1;            
            print_to_screen(i, "is fat");
            break ;
        }
        while (i < all.data->n + 1)
        {
            if (check_philo(&all.wise[i - 1]) == 1  && !(all.wise[i - 1].is_eating))
            {
                f = 1;
                print_to_screen(i, "is dead");
                break ;
            }

            i++;
        }

    }
}

void    clean( void )
{
    int i;

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
        pthread_detach((all.wise[i - 1].philo));
        usleep(10);
        i++;
    }

    supervisor();
    clean();
}
