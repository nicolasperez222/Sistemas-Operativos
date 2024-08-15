/* 
    Pontificia Universidad Javeriana
    Nicolas Sebastian Perez Trujillo
    Sistemas Operativos
    Challenge 05
    Fecha: 01/08/2024
*/

#include <stdio.h>
#include <stdlib.h>

int main(){
    int *ptr = NULL, i, n1, n2;

  printf("Enter size: ");
    scanf("%d", &n1);
    ptr = (int*) malloc(n1 * sizeof(int));
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Addresses of previously allocated memory: ");
    for(i = 0; i < n1; ++i){
        printf("\n\np = %p\n", (void*)(ptr + i));
    }
    printf("\nEnter the new size: ");
    scanf("%d", &n2);

    int *new_ptr = (int*) realloc(ptr, n2 * sizeof(int));
    if (new_ptr == NULL) {
        printf("Memory reallocation failed\n");
        free(ptr); 
        return 1;
    }

    ptr = new_ptr;
    printf("Addresses of newly allocated memory: ");
    for(i = 0; i < n2; ++i){
        printf("\n\nnp = %p\n", (void*)(ptr + i));
    }
    free(ptr);

    return 0;
}
