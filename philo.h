#ifndef __philo__h__
# define __philo__h__


# include <strings.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_philo {

    size_t n;
    size_t time_to_die;
    size_t time_to_eat;
    size_t time_to_sleep;
    size_t meals;
}               t_philo;

typedef struct s_book {

   pthread_t        philo;

    size_t               id;
    double             start;
    double             last_m;
    size_t           n_meals;
    int             flag;
    pthread_mutex_t  myfork;
    bool             is_eating;
    bool             is_sleeping;

}               t_book;

typedef struct s_global {


    t_book *wise;
    struct timeval bdyatlkhal9;
    pthread_mutex_t  print;
    pthread_mutex_t  inc_meal;
    t_philo     *data;
}               t_global;

t_global all;

#endif
