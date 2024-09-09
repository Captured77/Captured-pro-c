#include <stdio.h>
int main() {

    int a;
    char b;
    float c;
    double d;

    a = 520;
    b = 'F';
    c = 3.14;
    d = 3.141592653;

    printf("%d\n", a);
    printf("%c\n", b);
    printf("%.2f\n", c);
    printf("%11.9f\n", d);

    return 0;
}