/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Punteros 04
    Fecha: 01/08/2024
*/

#include <stdlib.h>
#include <stdio.h>

int main(int agrc, char **argv){
  int rows, cols, i, j;
  int **matrix;
  
  rows = (int) atof(argv[1]);
  cols = (int) atof(argv[2]);
  
  matrix = (int **)malloc(rows*sizeof(int *));
  for(i = 0; i<rows; i++){
     matrix[i] = (int *)malloc(cols * sizeof(int));
   
  }
  
  for(i = 0; i < rows; i++){
    for(j = 0; j<cols; j++){
      matrix[i][j] = i * j;
     
    }
  }
  
  for(i = 0; i < rows; i++){
    for(j = 0; j < cols; j++){
      printf("%d ", matrix[i][j]);
    }
    printf("\n");

  }
  
  for(i=0;i<rows;i++){
    free(matrix[i]);
  }
  free(matrix);
  
  return 0;
}
