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
        *(*List + i) = rand() % 11;
}
int compare(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}
void swap(int List[], int i, int j)
{
    int temp = List[i];
    List[i] = List[j];
    List[j] = temp;
}
int SupperCmp(int x, int y, int chose)
{
    switch (chose)
    {
    case 1:
        return x - y;
        break;
    default:
        puts("Fail!! Khong the sap xep!");
        exit(0);
        break;
    }
}
float sort3Way(int List[], int left, int right)
{
    if (right <= left)
        return 0;
    clock_t tstart, tfinish;
    tstart = clock();
    int i = left - 1, j = right;
    int p = left - 1, q = right;
    while (1)
    {
        while (List[++i] < List[right])
            ;
        while (List[right] < List[--j])
            if (j == left)
                break;
        if (i >= j)
            break;
        swap(List, i, j);
        if (List[i] == List[right])
            swap(List, ++p, i);
        if (List[j] == List[right])
            swap(List, --q, j);
    }
    swap(List, i, right);
    j = i - 1;
    i++;
    for (int k = left; k <= p; k++)
        swap(List, k, j--);
    for (int k = right - 1; k >= q; k--)
        swap(List, k, i++);
    sort3Way(List, left, j);
    sort3Way(List, i, right);
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}
int *copyNewArray(int n, int *root)
{
    int *List;
    List = (int *)malloc(sizeof(int) * n);
    return memcpy(List, root, sizeof(int) * n);
}
float insertion_sort(int list[], int n, int chose)
{
    int i, j;
    int next;
    clock_t tstart, tfinish;
    tstart = clock();
    for (i = 1; i < n; i++)
    {
        next = list[i];
        for (j = i - 1; j >= 0 && SupperCmp(next, list[j], chose) < 0; j--)
            list[j + 1] = list[j];
        list[j + 1] = next;
    }
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}
int main(int argc, char const *argv[])
{
    int *List, *List1, *List2;
    createNewArray(&List, MAX);
    List1 = copyNewArray(MAX, List);
    List2 = copyNewArray(MAX, List);
    clock_t tSart = clock();
    qsort(List, MAX, sizeof(int), compare);
    clock_t tFinish = clock();
    printf("3 way partition: %g\n", sort3Way(List1, 0, MAX - 1));
    printf("qsort: %.3g\n", (float)(tFinish - tSart) / CLOCKS_PER_SEC);
    // printf("insertion sort: %g\n", insertion_sort(List2, MAX, 1));
    free(List);
    return 0;
}