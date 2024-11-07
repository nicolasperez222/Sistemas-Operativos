#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t BLOQUEO;

void *funcion(void *job) {
    pthread_mutex_lock(&BLOQUEO);
    size_t IDh = *(size_t *)job;
    int cont = 5 * IDh;
    printf("\nHilo %zu ha iniciado\n", IDh);

    for (int i = 0; i < 230; i++) {
        cont *= (cont + 1);
    }
    printf("\nHilo %zu ha finalizado\n", IDh);
    pthread_mutex_unlock(&BLOQUEO);
    return NULL;
}

int main() {
    size_t jobs[3];
    pthread_t hilos[3];
    int i = 0;
    int error;

    if (pthread_mutex_init(&BLOQUEO, NULL) != 0) {
        printf("\nFallo de inicialización de MUTEX\n");
        return 1;
    }

    while (i < 3) {
        jobs[i] = i;
        error = pthread_create(&hilos[i], NULL, &funcion, &jobs[i]);
        if (error != 0) {
            printf("\nError en la creación de hilos [%s]", strerror(error));
        }
        i++;
    }

    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    pthread_join(hilos[2], NULL);

    pthread_mutex_destroy(&BLOQUEO);
    return 0;
}
