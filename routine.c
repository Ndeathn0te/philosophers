// ===========================
//          routine.c
// ===========================
#include "philosophers.h"

// Función que simula la acción de comer de un filósofo.
// Recibe un puntero a la estructura del filósofo.
static void eat(t_philo *philo)
{
    t_data *data = philo->data; // Puntero a la estructura de datos principal.

    // El filósofo intenta tomar el tenedor izquierdo.
    pthread_mutex_lock(&data->forks[philo->left_fork]);
    print_action(data, philo->id, "has taken a fork"); // Imprime la acción.

    // Caso especial: si solo hay un filósofo, tomará un tenedor y esperará hasta morir.
    if (philo->left_fork == philo->right_fork)
    {
        // Espera en bucle hasta que la simulación termine por muerte.
        // Este es un caso límite para un solo filósofo.
        while (!data->someone_dead)
            usleep(250); // Pequeña pausa para no consumir CPU innecesariamente.
        // Libera el tenedor (aunque en este caso la simulación ya habría terminado).
        pthread_mutex_unlock(&data->forks[philo->left_fork]);
        return ;
    }
    // El filósofo intenta tomar el tenedor derecho.
    pthread_mutex_lock(&data->forks[philo->right_fork]);
    print_action(data, philo->id, "has taken a fork"); // Imprime la acción.

    // Bloquea el mutex de datos para actualizar de forma segura la información del filósofo.
    pthread_mutex_lock(&data->data_mutex);
    print_action(data, philo->id, "is eating"); // Imprime que el filósofo está comiendo.
    philo->last_meal = timestamp(); // Actualiza la marca de tiempo de la última comida.
    philo->meals_eaten++;           // Incrementa el contador de comidas.
    pthread_mutex_unlock(&data->data_mutex); // Desbloquea el mutex de datos.

    // Simula el tiempo que el filósofo pasa comiendo.
    precise_usleep(data->time_eat, data);
    // El filósofo suelta el tenedor derecho.
    pthread_mutex_unlock(&data->forks[philo->right_fork]);
    // El filósofo suelta el tenedor izquierdo.
    pthread_mutex_unlock(&data->forks[philo->left_fork]);
}

// Función que simula la acción de pensar de un filósofo.
// Recibe un puntero a la estructura del filósofo.
static void thinking(t_philo *philo)
{
    t_data *data = philo->data; // Puntero a la estructura de datos principal.

    print_action(data, philo->id, "is thinking"); // Imprime que el filósofo está pensando.
}

// Función que simula la acción de dormir de un filósofo.
// Recibe un puntero a la estructura del filósofo.
static void sleeping_philo(t_philo *philo)
{
    t_data *data = philo->data; // Puntero a la estructura de datos principal.

    print_action(data, philo->id, "is sleeping"); // Imprime que el filósofo está durmiendo.
    // Simula el tiempo que el filósofo pasa durmiendo.
    precise_usleep(data->time_sleep, data);
}

// Función principal que ejecuta cada hilo de filósofo.
// Define el ciclo de vida de un filósofo: comer, dormir, pensar.
// Recibe un puntero genérico que se convierte a un puntero a t_philo.
void    *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg; // Convierte el argumento al tipo t_philo.
    t_data  *data = philo->data;    // Puntero a la estructura de datos principal.

    // Si el ID del filósofo es impar, espera un corto período.
    // Esto ayuda a desincronizar a los filósofos y evitar un posible interbloqueo inicial
    // donde todos intentan tomar los tenedores al mismo tiempo en el mismo orden.
    if (philo->id % 2)
        usleep(200); // Pequeña pausa.

    // Bucle principal del filósofo: continúa mientras no haya muerto ningún filósofo
    // y no todos los filósofos hayan comido el número requerido de veces (si está definido).
    while (!data->someone_dead && !data->all_ate)
    {
        eat(philo); // El filósofo intenta comer.
        // Si después de comer se cumple la condición de que todos han comido (y must_eat está activo), sale del bucle.
        if (data->all_ate)
            break ;
        sleeping_philo(philo); // El filósofo duerme.
        thinking(philo);       // El filósofo piensa.
    }
    return (NULL); // Termina la ejecución del hilo.
}
