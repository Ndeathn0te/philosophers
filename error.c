// ===========================
//           error.c
// ===========================
#include "philosophers.h"

// Función para destruir todos los mutexes inicializados.
// Recibe un puntero a la estructura de datos principal.
static void destroy_mutexes(t_data *data)
{
    int i;

    // Si no hay tenedores (forks), no hay nada que destruir.
    if (!data->forks)
        return ;
    i = 0;
    // Itera sobre el número de filósofos para destruir el mutex de cada tenedor.
    while (i < data->nb_philo)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    // Destruye el mutex utilizado para la impresión sincronizada.
    pthread_mutex_destroy(&data->print);
    // Destruye el mutex utilizado para proteger el acceso a los datos compartidos.
    pthread_mutex_destroy(&data->data_mutex);
}

// Función para limpiar todos los recursos asignados y mostrar un mensaje de error si es necesario.
// Recibe un puntero a la estructura de datos principal y un mensaje de error opcional.
// Devuelve 1 para indicar un error.
int cleanup(t_data *data, char *msg)
{
    // Si se proporciona un mensaje, lo imprime en la salida de error estándar.
    if (msg)
        fprintf(stderr, "Error: %s\n", msg);
    // Si la estructura de datos existe, procede a liberar los recursos.
    if (data)
    {
        // Llama a la función para destruir todos los mutexes.
        destroy_mutexes(data);
        // Si la memoria para los filósofos fue asignada, la libera.
        if (data->philos)
            free(data->philos);
        // Si la memoria para los tenedores fue asignada, la libera.
        if (data->forks)
            free(data->forks);
    }
    // Devuelve 1 para indicar que la limpieza se realizó (generalmente en caso de error).
    return (1);
}
