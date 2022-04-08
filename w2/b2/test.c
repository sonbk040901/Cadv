#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int size(void *x)
{
    return sizeof(*x);
}
int main(int argc, char const *argv[])
{
    char *a;
    a = (char *)malloc(sizeof(char));
    int *b;
    b = (int *)malloc(sizeof(int));
    long *c;
    c = (long *)malloc(sizeof(long));
    printf("%d %d %d\n", size(a), size(b), size(c));
    return 0;
}
