/**************************************

  Tema: Taller Fork()
  Tópico: Fork y pipes
  Autor: Nicolás S. Pérez
  Materia: Sistemas Operativos
  
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h> 

/* Función para leer un archivo y cargar los enteros en un arreglo dinámico
 Recibe el nombre del archivo, el arreglo donde se almacenarán los enteros
y el número de elementos a leer. Si no se puede abrir el archivo, termina el programa.*/

void leer_fichero(char *nombre_archivo, int *arreglo, int n) {
    FILE *f = fopen(nombre_archivo, "r");
    if (f == NULL) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    // Leer los números del archivo y almacenarlos en el arreglo
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d", &arreglo[i]);
    }

    fclose(f);
}

/* Función que calcula la suma de un arreglo de enteros
 Recibe el arreglo, el número de elementos, el pipe a través del cual enviará el resultado,
 y el nombre del proceso para imprimir información adicional. La suma se envía a través del pipe.*/

void calcular_suma(int *arreglo, int n, int pipefd[2], const char *proceso_nombre) {
    close(pipefd[0]); 
    int suma = 0;

    //Se calcula la suma de todos los elementos del arreglo
    for (int i = 0; i < n; i++) {
        suma += arreglo[i];
    }

    //Se imprime el resultado de la suma con el nombre del proceso
    printf("%s, suma: %d\n", proceso_nombre, suma);

    //Se envía la suma por el pipe
    write(pipefd[1], &suma, sizeof(suma));
    close(pipefd[1]);
}

int main(int argc, char *argv[]) {
    //Se verifica que los argumentos sean correctos
    if (argc != 5) {
        return 1;
    }

    // Paso 1: Convertir los argumentos a enteros y nombres de archivos
    int N1 = atoi(argv[1]); 
    int N2 = atoi(argv[3]); 
    char *archivo00 = argv[2]; 
    char *archivo01 = argv[4]; 

    // Paso 2: Reservar memoria dinámica para los arreglos
    int *arreglo00 = malloc(N1 * sizeof(int));
    int *arreglo01 = malloc(N2 * sizeof(int));

    // Paso 3: Leer los arreglos desde los archivos correspondientes
    leer_fichero(archivo00, arreglo00, N1);
    leer_fichero(archivo01, arreglo01, N2);

    // Crear pipes para la comunicación entre procesos
    int pipeA[2], pipeB[2], pipeC[2];
    if (pipe(pipeA) == -1 || pipe(pipeB) == -1 || pipe(pipeC) == -1) {
        perror("Error al crear pipes");
        exit(1);
    }

    // Crear el proceso grand hijo que calcula la suma del archivo00
    pid_t pid_nieto = fork();
    if (pid_nieto == -1) {
        perror("Error al hacer fork");
        exit(1);
    } else if (pid_nieto == 0) {
      
        // El proceso nieto (grand hijo) calcula la suma del arreglo00 y la envía por pipeA
        calcular_suma(arreglo00, N1, pipeA, "Grand hijo");
      
        exit(0); 
    }

    // Crear el proceso "Segundo hijo" que calcula la suma del archivo01
    pid_t pid_segundo_hijo = fork();
    if (pid_segundo_hijo == -1) {
        perror("Error al hacer fork");
        exit(1);
    } else if (pid_segundo_hijo == 0) {
        // El proceso segundo hijo calcula la suma del arreglo01 y la envía por pipeB
        calcular_suma(arreglo01, N2, pipeB, "Segundo hijo");
        exit(0); 
    }

    /* Crear el proceso "Primer hijo" que recibe las sumas de los otros dos procesos
    y calcula la suma total de ambos arreglos */
    pid_t pid_primer_hijo = fork();
    if (pid_primer_hijo == -1) {
        perror("Error al hacer fork");
        exit(1);
    } else if (pid_primer_hijo == 0) {
        close(pipeC[0]); 
        int suma_total = 0;
        int sumaA, sumaB;

        // Leer la suma del "Grand hijo" (arreglo00) desde el pipeA
        read(pipeA[0], &sumaA, sizeof(sumaA));

        // Leer la suma del "Segundo hijo" (arreglo01) desde el pipeB
        read(pipeB[0], &sumaB, sizeof(sumaB));

        // Sumar ambas sumas recibidas
        suma_total = sumaA + sumaB;

        // Imprimir el resultado de la suma total (para depuración)
        printf("Primer hijo, suma total: %d\n", suma_total);

        // Enviar la suma total al proceso padre usando el pipeC
        write(pipeC[1], &suma_total, sizeof(suma_total));
        close(pipeC[1]);
        exit(0); 
    }

    // Proceso padre
    // Se cierran los extremos de escritura de todos los pipes ya que el padre solo recibe datos
    close(pipeA[1]); close(pipeB[1]); close(pipeC[1]);  
    // Se cierran los extremos de lectura de pipeA y pipeB, ya no los necesitamos
    close(pipeA[0]); close(pipeB[0]);

    int suma_total;

    // Leer la suma total enviada por el primer hijo desde el pipeC
    read(pipeC[0], &suma_total, sizeof(suma_total));

    // Imprimir la suma total que el proceso padre ha recibido
    printf("Padre, suma total: %d\n", suma_total);

    // Esperar a que terminen los procesos hijos
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Liberar memoria dinámica
    free(arreglo00);
    free(arreglo01);

    return 0;
}
