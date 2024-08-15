/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Punteros 03
    Fecha: 01/08/2024
*/

#include <stdlib.h>
#include <stdio.h>

#define size 10

int main(int argc, char *argv[]){
  int valor1, valor2;
  int *ptrV1, *ptrV2;
  ptrV1 = &valor1;
  ptrV2 = &valor2;
  
  printf("\nIngrese dos valores a ser sumados \n");
  scanf("%d %d", &valor1, &valor2);
  printf("\n La suma es = %d\n", *ptrV1+*ptrV2);
  return 0;
}
