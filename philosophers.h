// ===========================
//        philosophers.h
// ===========================
#ifndef PHILOSOPHERS_H // Guardia de inclusión para evitar la inclusión múltiple del archivo de cabecera.
#define PHILOSOPHERS_H

// Inclusión de bibliotecas estándar necesarias.
#include <pthread.h>    // Para la gestión de hilos (threads) y mutexes.
#include <sys/time.h>   // Para la función gettimeofday (obtener el tiempo actual).
#include <unistd.h>     // Para la función usleep (pausar la ejecución).
#include <stdlib.h>     // Para funciones como malloc, free, exit.
#include <stdio.h>      // Para funciones de entrada/salida estándar como printf.
#include "./libft/inc/libft.h" // Inclusión de la biblioteca personalizada libft.

// Declaración anticipada (forward declaration) de la estructura s_data.
// Permite que t_philo pueda tener un puntero a t_data sin que t_data esté completamente definida aún.
typedef struct s_data t_data;

// Estructura para representar a un filósofo.
typedef struct s_philo
{
    int             id;             // Identificador único del filósofo.
    int             left_fork;      // Índice del tenedor izquierdo en el array de tenedores.
    int             right_fork;     // Índice del tenedor derecho en el array de tenedores.
    long            last_meal;      // Marca de tiempo de la última vez que el filósofo comió.
    int             meals_eaten;    // Contador de cuántas veces ha comido el filósofo.
    pthread_t       thread;         // Identificador del hilo asociado a este filósofo.
    t_data          *data;          // Puntero a la estructura de datos principal compartida.
}   t_philo;

// Estructura principal de datos que contiene toda la información de la simulación.
struct s_data
{
    int             nb_philo;       // Número total de filósofos (y tenedores).
    long            time_die;       // Tiempo (en ms) que un filósofo puede pasar sin comer antes de morir.
    long            time_eat;       // Tiempo (en ms) que un filósofo tarda en comer.
    long            time_sleep;     // Tiempo (en ms) que un filósofo tarda en dormir.
    int             must_eat;       // Número opcional de veces que cada filósofo debe comer antes de que termine la simulación. (-1 si no está definido).
    int             all_ate;        // Bandera (flag) que indica si todos los filósofos han comido 'must_eat' veces.
    int             someone_dead;   // Bandera (flag) que indica si algún filósofo ha muerto.
    long            start_time;     // Marca de tiempo del inicio de la simulación.
    pthread_mutex_t *forks;         // Array de mutexes, uno para cada tenedor.
    pthread_mutex_t print;          // Mutex para sincronizar la salida por consola (printf).
    pthread_mutex_t data_mutex;     // Mutex para proteger el acceso a datos compartidos en esta estructura (ej: someone_dead, all_ate).
    t_philo         *philos;        // Array de estructuras t_philo, una para cada filósofo.
};

// Declaraciones de funciones (prototipos) definidas en otros archivos .c.

//  init.c
// Inicializa la estructura de datos principal (t_data) con los argumentos de la línea de comandos.
int     init_data(t_data *data, int argc, char **argv);

//  utils.c
// Devuelve la marca de tiempo actual en milisegundos.
long    timestamp(void);
// Realiza una pausa precisa (similar a usleep) teniendo en cuenta el estado de la simulación.
void    precise_usleep(long time, t_data *data);
// Imprime una acción de un filósofo de forma sincronizada.
void    print_action(t_data *data, int id, char *str);

//  routine.c
// Función principal que ejecuta cada hilo de filósofo (comer, dormir, pensar).
void    *routine(void *arg);

//  monitor.c
// Función ejecutada por un hilo separado para monitorizar el estado de los filósofos (muerte, comidas completadas).
void    *monitor(void *arg);

//  error.c
// Libera todos los recursos asignados y muestra un mensaje de error si se proporciona.
int     cleanup(t_data *data, char *msg);

#endif // PHILOSOPHERS_H