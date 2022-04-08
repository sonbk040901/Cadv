#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 1000000
void createNewArray(int **List, int n)
{
    *List = (int *)malloc(sizeof(int) * n);
    srand((unsigned)clock());
    for (int i = 0; i < n; i++)
        *(*List + i) = rand() % 10;
}
int compare(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}
int main(int argc, char const *argv[])
{
    int *List;
    createNewArray(&List, MAX);
    clock_t tSart = clock();
    qsort(List, MAX, sizeof(int), compare);
    clock_t tFinish = clock();
    printf("Time to run: %.3g\n", (float)(tFinish - tSart) / CLOCKS_PER_SEC);
    free(List);
    return 0;
}