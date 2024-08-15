/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Challenge 01
    Fecha: 01/08/2024
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
  char *p;
  char *q = NULL;
  printf("Address of p=%s\n", p);
  strcpy(p, "Hello, I'm the best in Operating Systems!!!");
  printf("%s\n", p);
  printf("About to copy\"Goodbye\"to q\n");
  strcpy(q, "Goodbye");
  printf("String copied\n");
  printf("%s\n", q);
  
  return 0;

}
