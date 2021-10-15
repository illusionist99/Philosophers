# include "philo.h"








int     main( int ac, char **av )   {

    if ( ac  == 6 || ac == 5 )
        philo_func(++av);
    else
        printf("Error\n");
    return (0);
}
