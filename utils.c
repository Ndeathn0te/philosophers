// ===========================
//           utils.c
// ===========================
#include "philosophers.h"

// Función para obtener la marca de tiempo actual en milisegundos.
// No recibe parámetros.
// Devuelve un valor long que representa el tiempo actual en milisegundos.
long timestamp(void)
{
    struct timeval tv; // Estructura para almacenar el tiempo.

    // Obtiene el tiempo actual y lo almacena en 'tv'.
    gettimeofday(&tv, NULL);
    // Convierte los segundos y microsegundos a milisegundos y los devuelve.
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

// Función para realizar una pausa precisa (similar a usleep) durante un tiempo determinado.
// La pausa se interrumpe si algún filósofo muere.
// Recibe 'time' (long): la duración de la pausa en milisegundos.
// Recibe 'data' (t_data*): puntero a la estructura de datos principal para verificar el estado de la simulación.
void precise_usleep(long time, t_data *data)
{
    long start; // Variable para almacenar el tiempo de inicio de la pausa.

    start = timestamp(); // Registra el tiempo de inicio.
    // Bucle que continúa mientras no haya muerto ningún filósofo y
    // el tiempo transcurrido sea menor que el tiempo de pausa deseado.
    while (!data->someone_dead && (timestamp() - start) < time)
        usleep(200); // Realiza pausas cortas (200 microsegundos) para no consumir CPU excesivamente
                     // y permitir una verificación frecuente de la condición de salida.
}

// Función para imprimir una acción de un filósofo de forma sincronizada.
// Utiliza un mutex para evitar que múltiples hilos escriban en la consola al mismo tiempo.
// Recibe 'data' (t_data*): puntero a la estructura de datos principal.
// Recibe 'id' (int): el identificador del filósofo que realiza la acción.
// Recibe 'str' (char*): la cadena que describe la acción.
void print_action(t_data *data, int id, char *str)
{
    // Bloquea el mutex de impresión para asegurar que solo un hilo imprima a la vez.
    pthread_mutex_lock(&data->print);
    // Comprueba si la simulación debe continuar (nadie ha muerto y no todos han comido).
    if (!data->someone_dead && !data->all_ate)
        // Imprime la marca de tiempo relativa al inicio, el ID del filósofo (sumando 1 para que sea basado en 1) y la acción.
        printf("%ld %d %s\n", timestamp() - data->start_time, id + 1, str);
    // Desbloquea el mutex de impresión.
    pthread_mutex_unlock(&data->print);
}
