/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Punteros 01: * y &
    Fecha: 01/08/2024
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  int variable = 123;
  int *ptr = &variable;
  printf("Valor de la Variable %d \n", variable);
  printf("Dirección de memoria de la Variable %p\n", &variable);
  printf("Valor de la Variable %i \n", *ptr);
  
  return 0;
}
