#include <stdio.h> 
#include <stdlib.h>   
#include <pthread.h> 
#include <string.h>   

pthread_mutex_t BLOQUEO;

/**
 * Función que ejecuta el trabajo de cada hilo.
 * Recibe un puntero a un entero que representa el ID del hilo.
 * Bloquea el acceso compartido usando el mutex, realiza un cálculo y luego desbloquea.
 * 
 * @param job Puntero al ID del trabajo (hilo) actual
 * @return NULL
 */
void *funcion(void *job) {
    pthread_mutex_lock(&BLOQUEO);  // Bloquea el mutex para acceso exclusivo
    size_t IDh = *(size_t *)job;   // Convierte y obtiene el ID del hilo
    int cont = 5 * IDh;            // Inicializa una variable cont con un valor único para el hilo
    printf("\nHilo %zu ha iniciado\n", IDh);

    // Realiza una operación de cálculo intensivo
    for (int i = 0; i < 230; i++) {
        cont *= (cont + 1);
    }
    printf("\nHilo %zu ha finalizado\n", IDh);
    pthread_mutex_unlock(&BLOQUEO);  // Libera el mutex para que otros hilos puedan acceder
    return NULL;
}

/**
 * Función principal del programa.
 * Inicializa el mutex y crea tres hilos, cada uno ejecutando la función `funcion`.
 * Espera a que cada hilo termine antes de destruir el mutex y salir.
 * 
 * @return 0 si el programa finaliza correctamente, 1 en caso de error.
 */
int main() {
    size_t jobs[3];             // Array para almacenar el ID de cada trabajo
    pthread_t hilos[3];         // Array para almacenar los identificadores de los hilos
    int i = 0;
    int error;

    // Inicializa el mutex y verifica si la inicialización fue exitosa
    if (pthread_mutex_init(&BLOQUEO, NULL) != 0) {
        printf("\nFallo de inicialización de MUTEX\n");
        return 1;
    }

    // Bucle para crear tres hilos
    while (i < 3) {
        jobs[i] = i;   // Asigna un ID único para cada hilo
        // Crea un nuevo hilo que ejecutará la función `funcion`
        error = pthread_create(&hilos[i], NULL, &funcion, &jobs[i]);
        if (error != 0) {   // Verifica si hubo error en la creación del hilo
            printf("\nError en la creación de hilos [%s]", strerror(error));
        }
        i++;
    }

    // Espera a que cada hilo termine su ejecución
    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    pthread_join(hilos[2], NULL);

    // Destruye el mutex después de que todos los hilos hayan terminado
    pthread_mutex_destroy(&BLOQUEO);
    return 0;
}
