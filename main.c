# include "philo.h"
# include <strings.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

// start at 1 
typedef struct s_philo {

    size_t n;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    size_t meals;

}   t_philo;

t_philo     *data;
pthread_t *philo;
pthread_mutex_t *forkk;

void    eat( int id ) {

    // pthread_mutex_t *lock;

    // if (pthread_mutex_init(&lock, NULL) != 0)
    //     return ;
    if (pthread_mutex_lock(&forkk[(id - 1) % data->n]) == 0)
        printf(" %d has taken a fork\n", id);
    if (pthread_mutex_lock(&forkk[(id + 1) % data->n]) == 0)
        printf(" %d has taken a fork\n", id);

        printf("%d is eating\n", id);
        usleep(1000 * data->time_to_eat);

    pthread_mutex_unlock(&forkk[(id - 1) % data->n]);
    pthread_mutex_unlock(&forkk[(id + 1) % data->n]);

}



void    sleepp( int id ) {

    printf("%d is sleeping\n", id);
    usleep(1000 * data->time_to_sleep);
}
void    *routine( void *arg ) 
{
//     int *myid = arg;
//    printf("Thread ID: %d\n", *myid);

        // eat
    int *myid = (int *)arg;


    forkk = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (data->n + 1));
    int i = 0;
    while (i < data->n) {
        if (pthread_mutex_init(&forkk[i], NULL) != 0)
            return arg;
        i++;
    }
    //eat();
        // 2 forkss
    eat(*myid);
    // printf("my id is %d\n", *myid);
    sleepp(*myid);
   // think 
    printf(" philo in routine\n");
    i = 0;
    while (i < data->n) {
        if (pthread_mutex_destroy(&forkk[i]) != 0)
            return arg;
        i++;
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

    data->meals = atoi(av[4]);
    printf("Number of meals: %zu\n", data->meals);


    int i;

    int *id;

    i = 1;
    id = &i;
    philo = (pthread_t *)malloc(sizeof(pthread_t) * (data->n + 1));
    while (i <= data->n)
    {
        //printf("id : %d\n", i);
        id = &i;
        if (pthread_create(&philo[i - 1], NULL, routine, (void * )id) != 0){
            printf("Error philo\n");
            exit(0);
        }
        i++;
    }


    i = 1;
    while (i <= data->n) {
        pthread_join(philo[i - 1], NULL);
        i++;
    }
}

int     main( int ac, char **av ) {

    if ( ac  == 6 || ac == 5)
        philo_func(++av);
    else
        printf("Error\n");
    return (0);
}