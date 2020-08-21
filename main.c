#include <stdio.h>

int main() {
    printf("Hello, World!\n");

    int b = 2;
    int **a = NULL;
    int *c = &b;

    a = &c;

    printf("ok");

    return 0;
}