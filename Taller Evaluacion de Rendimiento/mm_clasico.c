/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Pérez N
	Fecha: Noviembre 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3) 

pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;

/**
 * Estructura que almacena los parámetros para cada hilo:
 * - nH: Número total de hilos.
 * - idH: Identificador del hilo actual.
 * - N: Tamaño de las matrices (dimensión NxN).
 */
struct parametros{
	int nH;
	int idH;
	int N;
};

struct timeval start, stop;

/**
 * Función para llenar las matrices mA y mB con valores específicos.
 * La matriz mA se llena con valores incrementales multiplicados por 1.1,
 * y mB con valores incrementales multiplicados por 2.2. 
 * La matriz mC se inicializa en 0.
 * 
 * @param SZ Tamaño de la matriz (NxN).
 */
void llenar_matriz(int SZ){ 
	srand48(time(NULL));
	for(int i = 0; i < SZ*SZ; i++){
			mA[i] = 1.1*i; //drand48(); 
			mB[i] = 2.2*i; //drand48();
			mC[i] = 0; 
		}	
}

/**
 * Función para imprimir una matriz en la consola. Si el tamaño de la matriz
 * es menor que 12x12, imprime el contenido en formato matricial.
 * 
 * @param sz Tamaño de la matriz.
 * @param matriz Puntero a la matriz que se quiere imprimir.
 */
void print_matrix(int sz, double *matriz){
	if(sz < 12){
    		for(int i = 0; i < sz*sz; i++){
     				if(i%sz==0) printf("\n");
            		printf(" %.3f ", matriz[i]);
			}	
    	printf("\n>-------------------->\n");
	}
}

/**
 * Función para iniciar la medición de tiempo.
 * Utiliza `gettimeofday` para obtener el tiempo actual y almacenarlo en la variable global `start`.
 */
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

/**
 * Función para finalizar la medición de tiempo.
 * Calcula el tiempo transcurrido en microsegundos desde la ejecución de `inicial_tiempo` 
 * y lo imprime en la consola.
 */
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}

/**
 * Función ejecutada por cada hilo para realizar la multiplicación de matrices.
 * Cada hilo calcula una parte de la matriz resultado mC, utilizando los índices 
 * adecuados basados en el identificador del hilo.
 * 
 * @param variables Puntero a una estructura que contiene los parámetros necesarios 
 * para la multiplicación (nH, idH, N).
 */
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables;
	
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N/nH)*idH;
	int fin = (N/nH)*(idH+1);

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i*N); 
			pB = mB + j;
            for (int k = 0; k < N; k++, pA++, pB+=N){
				sumaTemp += (*pA * *pB);
			}
			mC[i*N+j] = sumaTemp;
		}
	}

	pthread_mutex_lock (&MM_mutex);
	pthread_mutex_unlock (&MM_mutex);
	pthread_exit(NULL);
}

/**
 * Función principal que gestiona la ejecución del programa. 
 * Realiza las siguientes acciones:
 * - Procesa los argumentos de entrada.
 * - Inicializa las matrices mA, mB y mC.
 * - Inicia la medición de tiempo.
 * - Crea los hilos para realizar la multiplicación de matrices.
 * - Espera que todos los hilos finalicen su ejecución.
 * - Mide el tiempo total de ejecución y muestra el resultado.
 * - Imprime la matriz resultado mC.
 * 
 * @param argc Número de argumentos.
 * @param argv Arreglo con los argumentos de entrada.
 * @return Código de salida del programa.
 */
int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    int SZ = atoi(argv[1]); 
    int n_threads = atoi(argv[2]); 

    pthread_t p[n_threads];
    pthread_attr_t atrMM;

	mA = MEM_CHUNK;
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;

	llenar_matriz(SZ);
	print_matrix(SZ, mA);
	print_matrix(SZ, mB);

	inicial_tiempo();
	pthread_mutex_init(&MM_mutex, NULL);
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    for (int j=0; j<n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
	}

    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
	final_tiempo();
	
	print_matrix(SZ, mC);

	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);
}
