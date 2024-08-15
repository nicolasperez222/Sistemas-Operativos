/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Codigo 01
    Fecha: 01/08/2024
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  int x;
  int suma = 0;
  do{
    scanf("%d", &x);
    suma += x;
    
  }while(x!=0);
  printf("La suma es %d \n", suma);
  return 0;
}
