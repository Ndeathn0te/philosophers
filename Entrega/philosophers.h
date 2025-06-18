#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "./libft/inc/libft.h"

typedef struct s_data t_data;

typedef struct s_philo
{
    int     id;
    int     left_fork;
    int     rigth_fork;
    long    last_meal;
    int     meals_eaten;
    pthread_t   thread;
    t_data      *data;
    t_philo;
}

struct s_data
{
    int     nb_philo;
    long    time_die;
    long    time_eat;
    long    time_sleep;
    int     must_eat;
    int     all_ate;
    int     someone_dead;
    long    start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print;
    pthread_mutex_t data_mutex;
    t_philo     *philos;
};

int init_data(t_data *data, int argcm char **argv);


long    timestamp(void);

void    precise_usleep(long time, t_data *data);

void    print_action(t_data *data, int id, char *str);


void    *routine(void *arg);

void    *monitor(void *arg);


int     cleanup(t_data *data, char *msg);

#endif // philosophers_H :D