// ===========================
//           utils.c
// ===========================
#include "philosophers.h"

long timestamp(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void precise_usleep(long time, t_data *data)
{
    long start;

    start = timestamp();
    while (!data->someone_dead && (timestamp() - start) < time)
        usleep(200);
}

void print_action(t_data *data, int id, char *str)
{
    pthread_mutex_lock(&data->print);
    if (!data->someone_dead && !data->all_ate)
        printf("%ld %d %s\n", timestamp() - data->start_time, id + 1, str);
    pthread_mutex_unlock(&data->print);
}
