#include "alloc.h"
#include <stdio.h>

int main() {
    printf("== calloc test ==\n");
    int *arr = tucalloc(5, sizeof(int));
    for (int i = 0; i < 5; i++) arr[i] = i * 10;
    for (int i = 0; i < 5; i++) printf("%d\n", arr[i]);

    printf("== realloc test ==\n");
    arr = turealloc(arr, 10 * sizeof(int));
    for (int i = 5; i < 10; i++) arr[i] = i * 10;
    for (int i = 0; i < 10; i++) printf("%d\n", arr[i]);

    printf("== free test ==\n");
    tufree(arr);

    return 0;
}
