// ===========================
//          monitor.c
// ===========================
#include "philosophers.h"

// Función para verificar si algún filósofo ha muerto.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 1 si un filósofo ha muerto, 0 en caso contrario.
static int check_death(t_data *data)
{
    int i;

    i = 0;
    // Itera sobre todos los filósofos mientras no se haya detectado la muerte de ninguno.
    while (i < data->nb_philo && !data->someone_dead)
    {
        // Bloquea el mutex para acceder de forma segura a los datos compartidos.
        pthread_mutex_lock(&data->data_mutex);
        // Comprueba si el tiempo transcurrido desde la última comida del filósofo actual supera el tiempo para morir.
        if (timestamp() - data->philos[i].last_meal > data->time_die)
        {
            // Establece la bandera de que alguien ha muerto.
            data->someone_dead = 1;
            // Bloquea el mutex de impresión para mostrar el mensaje de muerte de forma sincronizada.
            pthread_mutex_lock(&data->print);
            printf("%ld %d died\n", timestamp() - data->start_time, i + 1);
            // Desbloquea el mutex de impresión.
            pthread_mutex_unlock(&data->print);
            // Desbloquea el mutex de datos.
            pthread_mutex_unlock(&data->data_mutex);
            // Devuelve 1 indicando que un filósofo ha muerto.
            return (1);
        }
        // Desbloquea el mutex de datos si el filósofo actual no ha muerto.
        pthread_mutex_unlock(&data->data_mutex);
        i++;
    }
    // Devuelve 0 si ningún filósofo ha muerto.
    return (0);
}

// Función para verificar si todos los filósofos han comido el número requerido de veces.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 1 si todos los filósofos han comido lo suficiente, 0 en caso contrario.
static int check_all_ate(t_data *data)
{
    int i;
    int count;

    // Si no hay un número mínimo de comidas establecido (must_eat == -1), esta condición no se aplica.
    if (data->must_eat == -1)
        return (0);
    i = 0;
    count = 0;
    // Itera sobre todos los filósofos.
    while (i < data->nb_philo)
    {
        // Bloquea el mutex para acceder de forma segura a los datos compartidos.
        pthread_mutex_lock(&data->data_mutex);
        // Comprueba si el filósofo actual ha comido el número requerido de veces.
        if (data->philos[i].meals_eaten >= data->must_eat)
            count++;
        // Desbloquea el mutex de datos.
        pthread_mutex_unlock(&data->data_mutex);
        i++;
    }
    // Si el contador de filósofos que han comido lo suficiente es igual al número total de filósofos.
    if (count == data->nb_philo)
    {
        // Bloquea el mutex para acceder de forma segura a los datos compartidos.
        pthread_mutex_lock(&data->data_mutex);
        // Establece la bandera de que todos han comido.
        data->all_ate = 1;
        // Desbloquea el mutex de datos.
        pthread_mutex_unlock(&data->data_mutex);
        // Devuelve 1 indicando que todos los filósofos han comido lo suficiente.
        return (1);
    }
    // Devuelve 0 si no todos los filósofos han comido lo suficiente.
    return (0);
}

// Función ejecutada por el hilo monitor.
// Supervisa el estado de los filósofos (muerte o si todos han comido).
// Recibe un puntero a la estructura de datos principal como argumento.
void *monitor(void *arg)
{
    t_data *data = (t_data *)arg;

    // Bucle principal del monitor: continúa mientras no todos hayan comido y nadie haya muerto.
    while (!data->all_ate && !data->someone_dead)
    {
        // Verifica si algún filósofo ha muerto.
        if (check_death(data))
            break ; // Si alguien muere, sale del bucle.
        // Verifica si todos los filósofos han comido lo suficiente.
        if (check_all_ate(data))
            break ; // Si todos han comido, sale del bucle.
        // Pequeña pausa para no consumir CPU innecesariamente.
        usleep(500);
    }
    // Devuelve NULL al finalizar la ejecución del hilo.
    return (NULL);
}