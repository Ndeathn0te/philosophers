// ===========================
//            main.c
// ===========================
#include "philosophers.h"

// Función para crear los hilos de los filósofos.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 0 si la creación de hilos es exitosa, o llama a cleanup y devuelve 1 en caso de error.
static int  create_threads(t_data *data)
{
    int i;

    // Registra el tiempo de inicio de la simulación.
    data->start_time = timestamp();
    i = 0;
    // Itera sobre el número de filósofos para crear un hilo para cada uno.
    while (i < data->nb_philo)
    {
        // Establece el tiempo de la última comida del filósofo al tiempo de inicio.
        data->philos[i].last_meal = data->start_time;
        // Crea un hilo para el filósofo actual, ejecutando la función 'routine'.
        if (pthread_create(&data->philos[i].thread, NULL, &routine, &data->philos[i]))
            // Si falla la creación del hilo, limpia los recursos y devuelve error.
            return (cleanup(data, "pthread_create error"));
        i++;
    }
    // Devuelve 0 si todos los hilos se crearon correctamente.
    return (0);
}

// Función para esperar a que terminen los hilos de los filósofos.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 0 si la unión de hilos es exitosa, o llama a cleanup y devuelve 1 en caso de error.
static int  join_threads(t_data *data)
{
    int i;

    i = 0;
    // Itera sobre el número de filósofos para esperar a que cada hilo termine.
    while (i < data->nb_philo)
    {
        // Espera a que el hilo del filósofo actual termine.
        if (pthread_join(data->philos[i].thread, NULL))
            // Si falla la unión del hilo, limpia los recursos y devuelve error.
            return (cleanup(data, "pthread_join error"));
        i++;
    }
    // Devuelve 0 si todos los hilos se unieron correctamente.
    return (0);
}

// Función principal del programa.
// Recibe el número de argumentos de la línea de comandos y los argumentos.
// Devuelve 0 si el programa se ejecuta correctamente, 1 en caso de error.
int main(int argc, char **argv)
{
    t_data      data;       // Estructura de datos principal.
    pthread_t   monitor_th; // Hilo para el monitor de la simulación.

    // Inicializa los datos del programa a partir de los argumentos de la línea de comandos.
    if (init_data(&data, argc, argv))
        // Si falla la inicialización, devuelve 1.
        return (1);
    // Crea los hilos para cada filósofo.
    if (create_threads(&data))
        // Si falla la creación de hilos, devuelve 1.
        return (1);
    // Crea el hilo monitor que supervisa el estado de los filósofos.
    if (pthread_create(&monitor_th, NULL, &monitor, &data))
        // Si falla la creación del hilo monitor, limpia los recursos y devuelve error.
        return (cleanup(&data, "monitor thread error"));
    // Espera a que el hilo monitor termine.
    if (pthread_join(monitor_th, NULL))
        // Si falla la unión del hilo monitor, limpia los recursos y devuelve error.
        return (cleanup(&data, "monitor join error"));
    // Espera a que todos los hilos de los filósofos terminen.
    join_threads(&data);
    // Limpia todos los recursos asignados antes de salir.
    cleanup(&data, NULL);
    // Devuelve 0 indicando una ejecución exitosa.
    return (0);
}
