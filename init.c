// ===========================
//            init.c
// ===========================
#include "philosophers.h"

// Función para asignar memoria para los filósofos y los tenedores.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 0 si la asignación es exitosa, 1 en caso de error.
static int alloc_everything(t_data *data)
{
    // Asigna memoria para el array de filósofos.
    data->philos = malloc(sizeof(t_philo) * data->nb_philo);
    // Si falla la asignación de memoria para los filósofos, devuelve error.
    if (!data->philos)
        return (1);
    // Asigna memoria para el array de mutexes de los tenedores.
    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
    // Si falla la asignación de memoria para los tenedores, devuelve error.
    if (!data->forks)
        return (1);
    // Devuelve 0 si todas las asignaciones fueron exitosas.
    return (0);
}

// Función para inicializar todos los mutexes.
// Recibe un puntero a la estructura de datos principal.
// Devuelve 0 si la inicialización es exitosa, 1 en caso de error.
static int init_mutexes(t_data *data)
{
    int i;

    i = 0;
    // Itera sobre el número de filósofos para inicializar el mutex de cada tenedor.
    while (i < data->nb_philo)
    {
        // Inicializa el mutex para el tenedor actual.
        if (pthread_mutex_init(&data->forks[i], NULL))
            // Si falla la inicialización del mutex, devuelve error.
            return (1);
        i++;
    }
    // Inicializa el mutex para la impresión sincronizada.
    if (pthread_mutex_init(&data->print, NULL))
        // Si falla la inicialización del mutex, devuelve error.
        return (1);
    // Inicializa el mutex para proteger el acceso a los datos compartidos.
    if (pthread_mutex_init(&data->data_mutex, NULL))
        // Si falla la inicialización del mutex, devuelve error.
        return (1);
    // Devuelve 0 si todos los mutexes se inicializaron correctamente.
    return (0);
}

// Función para inicializar los datos de cada filósofo.
// Recibe un puntero a la estructura de datos principal.
static void init_philos(t_data *data)
{
    int i;

    i = 0;
    // Itera sobre el número de filósofos para inicializar sus datos.
    while (i < data->nb_philo)
    {
        // Asigna el ID al filósofo.
        data->philos[i].id = i;
        // Asigna el índice del tenedor izquierdo.
        data->philos[i].left_fork = i;
        // Asigna el índice del tenedor derecho (circular).
        data->philos[i].right_fork = (i + 1) % data->nb_philo;
        // Inicializa el contador de comidas del filósofo.
        data->philos[i].meals_eaten = 0;
        // Asigna un puntero a la estructura de datos principal.
        data->philos[i].data = data;
        i++;
    }
}

// Función para analizar los argumentos de la línea de comandos y rellenar la estructura de datos.
// Recibe un puntero a la estructura de datos principal, el número de argumentos y los argumentos.
// Devuelve 0 si el análisis es exitoso, 1 en caso de error.
static int parse_and_fill(t_data *data, int argc, char **argv)
{
    // Verifica el número correcto de argumentos.
    if (argc < 5 || argc > 6)
        return (1);
    // Convierte y asigna el número de filósofos.
    data->nb_philo = ft_atoi(argv[1]);
    // Convierte y asigna el tiempo para morir.
    data->time_die = ft_atoi(argv[2]);
    // Convierte y asigna el tiempo para comer.
    data->time_eat = ft_atoi(argv[3]);
    // Convierte y asigna el tiempo para dormir.
    data->time_sleep = ft_atoi(argv[4]);
    // Si se proporciona, convierte y asigna el número de veces que cada filósofo debe comer.
    data->must_eat = (argc == 6) ? ft_atoi(argv[5]) : -1;
    // Valida que los valores de los argumentos sean positivos.
    if (data->nb_philo < 1 || data->time_die < 1 || data->time_eat < 1
        || data->time_sleep < 1 || (argc == 6 && data->must_eat < 1))
        return (1);
    // Inicializa la bandera de muerte de un filósofo.
    data->someone_dead = 0;
    // Inicializa la bandera de que todos los filósofos han comido.
    data->all_ate = 0;
    // Devuelve 0 si el análisis y llenado fueron exitosos.
    return (0);
}

// Función principal para inicializar todos los datos del programa.
// Recibe un puntero a la estructura de datos principal, el número de argumentos y los argumentos.
// Devuelve 0 si la inicialización es exitosa, 1 en caso de error.
int init_data(t_data *data, int argc, char **argv)
{
    // Analiza los argumentos y rellena la estructura de datos.
    if (parse_and_fill(data, argc, argv))
        // Si hay un error en los argumentos, limpia y devuelve error.
        return (cleanup(NULL, "Invalid arguments"));
    // Asigna memoria para filósofos y tenedores.
    if (alloc_everything(data))
        // Si hay un error de asignación de memoria, limpia y devuelve error.
        return (cleanup(NULL, "Malloc error"));
    // Inicializa los mutexes.
    if (init_mutexes(data))
        // Si hay un error al inicializar los mutexes, limpia y devuelve error.
        return (cleanup(data, "Mutex init error"));
    // Inicializa los datos de los filósofos.
    init_philos(data);
    // Devuelve 0 si toda la inicialización fue exitosa.
    return (0);
}
