/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Challenge 04
    Fecha: 01/08/2024
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
  int n, i, *ptr, sum = 0;
  printf("Enter number of elements: ");
  scanf("%d", &n);
  ptr = (int*) calloc(n, sizeof(int));
  if(ptr == NULL){
    printf("Error! memory not allocated");
    exit(0);
  }
  printf("Enter elements: ");
  for(i = 0; i<n;++i){
    scanf("%d", ptr +i);
    sum += *(ptr + i);
  }
  
  printf("Sum = %d", sum);
  free(ptr);
  return 0;
  return 0;
}
