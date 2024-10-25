#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Declaración de la variable compartida contador y el valor máximo de hilos
int counter = 0;
int max = 4;

// Inicialización de los mutex y variables condicionales
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER;

// Prototipos de las funciones para el productor y el consumidor
void *productor();
void *consumidor();

/* Función principal que crea y une hilos productores y consumidores.
   Crea 5 hilos de productores y 5 hilos de consumidores, luego espera
   a que todos finalicen (con pthread_join). */
int main() {
    pthread_t proHilo[max], conHilo[max];
    int i;
    for(i = 0; i < 5; i++) {
        // Creación de hilos productores y consumidores
        pthread_create(&proHilo[i], NULL, &productor, NULL);
        pthread_create(&conHilo[i], NULL, &consumidor, NULL);
    }
    for(i = 0; i < 5; i++) {
        // Espera a que los hilos finalicen
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }
    return 0;
}

/* Función ejecutada por cada hilo productor.
   Cada productor aumenta el valor de counter hasta un límite y espera
   si el valor del contador alcanza 10. */
void * productor() {
    while(1) {
        // Se bloquea el mutex condp_mutex para controlar el acceso a la condición de productores
        pthread_mutex_lock(&condp_mutex);
        while(counter >= 10) {
            // Espera hasta que haya espacio para producir (contador < 10)
            pthread_cond_wait(&condVarProd, &condp_mutex);
        }
        // Libera el mutex condp_mutex
        pthread_mutex_unlock(&condp_mutex);

        // Se bloquea el mutex counter_mutex para actualizar el contador
        pthread_mutex_lock(&counter_mutex);
        // Incrementa el contador
        counter++;
        // Señala a los consumidores que hay un nuevo producto disponible
        pthread_cond_signal(&condVarCons);
        printf("Soy productor %d valor contador = %d\n", (int)pthread_self(), counter);
        // Libera los mutexes
        pthread_mutex_unlock(&condp_mutex);
        pthread_mutex_unlock(&counter_mutex);

        // Simula un retardo si el contador llega a 5
        if(counter == 5) {
            sleep(1);
        }
    }
}

/* Función ejecutada por cada hilo consumidor.
   Cada consumidor disminuye el valor de `counter` y espera si el valor del contador es 0. */
void * consumidor() {
    while(1) {
        // El consumidor espera 1 segundo antes de intentar consumir
        sleep(1);
        // Se bloquea el mutex condc_mutex para controlar el acceso a la condición de consumidores
        pthread_mutex_lock(&condc_mutex);
        while(counter <= 0) {
            // Señala a los productores que pueden producir
            pthread_cond_signal(&condVarProd);
            // Espera hasta que haya productos disponibles (contador > 0)
            pthread_cond_wait(&condVarCons, &condc_mutex);
        }
        // Libera el mutex condc_mutex
        pthread_mutex_unlock(&condc_mutex);

        // Se bloquea el mutex counter_mutex para actualizar el contador
        pthread_mutex_lock(&counter_mutex);
        if(counter > 0) {
            // El consumidor decrementa el contador
            printf("Soy consumidor %d valor contador= %d\n", (int)pthread_self(), counter);
            counter--;
            // Señala a los productores que hay espacio disponible para producir
            pthread_cond_signal(&condVarProd);
        }
        // Libera los mutexes
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_unlock(&condc_mutex);
    }
}
