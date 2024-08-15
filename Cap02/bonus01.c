/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Bonus 01
    Fecha: 01/08/2024
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *function(const char *p) {
    char *q = (char *)malloc(strlen(p) + 1);
    strcpy(q, p);
    printf("From q: the string is %s\n", q);
    return q;
}

int main() {
    char *a = NULL;
    char *b = NULL;
    a = function("Hi, it's fun to learn");
    b = function("Systems Engineer");
    printf("From main: %s %s\n", a, b);

    free(a);
    free(b);

    return 0;
}
