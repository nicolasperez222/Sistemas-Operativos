/*********************************************************
  Autor: Nicolás S. Pérez Trujillo
  Materia: Sistemas Operativos
  Fecha: 15 de Agosto de 2024
  Tema: Matrices
  Tópico: El uso o no uso de punteros
**********************************************************/

#include <stdio.h>

void imprimirMatriz(int *matriz[], int tam){
  for(int i=0;i<tam*tam;i++){
    if(i%tam==0){
      printf("\n");
    }
    printf(" %d ", *(matriz+i));
  }
   printf("\n\n");
}

int main(int argc, char *argv[]){
  int col = atoi(argv[1]); //Columnas
  int tam = col*col; //Tamaño
 
  int mA[tam];
  int mB[tam];
  int mC[tam];

  /*Inicialización*/
  for(int i=0;i<tam;i++){
    mA[i] = 1;
    mB[i] = 2;
    mC[i] = 0;
  }

  /*Impresión*/
  
 
  imprimirMatriz(mA, col);
  imprimirMatriz(mB, col);
  imprimirMatriz(mC, col);
  printf("\n\n\n-----FIN DEL PROGRAMA------\n");
  
  return 0;
}
