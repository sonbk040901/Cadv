#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void swap(void *List, int i, int j)
{
    int temp = *((int *)List + i);
    *((int *)List + i) = *((int *)List + j);
    *((int *)List + j) = temp;
}

int compare(const void *x, const void *y)
{
    printf("%p %p\n", (char *)x, (char *)(y));
    printf("%p %p\n", (int *)x, (int *)(y));
    return *((int *)x) - *((int *)y);
}
void test(void *x, int (*compare)(void *, void *), int size)
{
    printf("%p %p\n", (char *)x, (char *)(x + size));
    printf("%d\n", compare((char *)x, (char *)(x + size)));
}
int main(int argc, char const *argv[])
{
    void (*sw)(void *, int, int) = &swap;
    int *x;
    x = (int *)malloc(sizeof(int) * 2);
    scanf("%d %d", x, x + 1);
    (*sw)(x, 0, 1);
    printf("%d %d\n", *x, *(x + 1));
    // printf("%d %d\n", x[0], x[1]);
    // swap(x, 0, 1);
    // printf("%d %d\n", x[0], x[1]);
    printf("%p %p\n", x, x + 1);
    test(x, compare, sizeof(int));
    return 0;
}
