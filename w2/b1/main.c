#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 100
void createNewArray(int **List, int n)
{
    *List = (int *)malloc(sizeof(int) * n);
    srand((unsigned)clock());
    for (int i = 0; i < n; i++)
        *(*List + i) = rand() % 11;
}
int compare(const void *x, const void *y)
{
    return *((int *)x) - *((int *)y);
}
void swap(void *List, int i, int j)
{
    int temp = *((int *)List + i);
    *((int *)List + i) = *((int *)List + j);
    *((int *)List + j) = temp;
}
float sort3Way(void *List, int left, int right, int (*compare)(const void *, const void *), size_t size)
{
    if (right <= left)
        return 0;
    clock_t tstart, tfinish;
    tstart = clock();
    int i = left - 1, j = right;
    int p = left - 1, q = right;
    while (1)
    {
        while (compare(List + (++i) * size, List + right * size) < 0)
            ;
        while (compare(List + right * size, List + (--j) * size) < 0)
            if (j == left)
                break;
        if (i >= j)
            break;
        swap(List, i, j);
        if (compare(List + i * size, List + right * size) == 0)
            swap(List, ++p, i);
        if (compare((List + j * size), List + right * size) == 0)
            swap(List, --q, j);
    }
    swap(List, i, right);
    j = i - 1;
    i++;
    for (int k = left; k <= p; k++)
        swap(List, k, j--);
    for (int k = right - 1; k >= q; k--)
        swap(List, k, i++);
    sort3Way(List, left, j, compare, size);
    sort3Way(List, i, right, compare, size);
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}
int *copyNewArray(int n, int *root)
{
    int *List;
    List = (int *)malloc(sizeof(int) * n);
    return memcpy(List, root, sizeof(int) * n);
}
int main(int argc, char const *argv[])
{
    int *List;
    createNewArray(&List, MAX);
    for (int i = 0; i < MAX; i++)
    {
        printf("%d\t", *(List + i));
    }
    printf("\n");
    printf("3 way partition: %g\n", sort3Way(List, 0, MAX - 1, compare, sizeof(int)));
    for (int i = 0; i < MAX; i++)
    {
        printf("%d\t", *(List + i));
    }
    printf("\n");
    return 0;
}
