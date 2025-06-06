// ===========================
//          routine.c
// ===========================
#include "philosophers.h"

static void eat(t_philo *philo)
{
    t_data *data = philo->data;

    pthread_mutex_lock(&data->forks[philo->left_fork]);
    print_action(data, philo->id, "has taken a fork");
    if (philo->left_fork == philo->right_fork)
    {
        //  One philosopher edge case: wait until death
        while (!data->someone_dead)
            usleep(250);
        return ;
    }
    pthread_mutex_lock(&data->forks[philo->right_fork]);
    print_action(data, philo->id, "has taken a fork");

    pthread_mutex_lock(&data->data_mutex);
    print_action(data, philo->id, "is eating");
    philo->last_meal = timestamp();
    philo->meals_eaten++;
    pthread_mutex_unlock(&data->data_mutex);

    precise_usleep(data->time_eat, data);
    pthread_mutex_unlock(&data->forks[philo->right_fork]);
    pthread_mutex_unlock(&data->forks[philo->left_fork]);
}

static void thinking(t_philo *philo)
{
    t_data *data = philo->data;

    print_action(data, philo->id, "is thinking");
}

static void sleeping_philo(t_philo *philo)
{
    t_data *data = philo->data;

    print_action(data, philo->id, "is sleeping");
    precise_usleep(data->time_sleep, data);
}

void    *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data  *data = philo->data;

    if (philo->id % 2)
        usleep(200);
    while (!data->someone_dead && !data->all_ate)
    {
        eat(philo);
        if (data->all_ate)
            break ;
        sleeping_philo(philo);
        thinking(philo);
    }
    return (NULL);
}
