#include <stdio.h>

int main(void){ // to be compile as gcc -S teste-var64.c
    float x =3.14159, y = 2.71828;
    x = 2 * x + y;
    y = x * y;
    printf("%g, %g", x, y);
    return 0;
}