#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <time.h>
#define MAX 1000000
void SWAP(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
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
void SupperCpy(int *key, int x)
{
    *key = x;
}
int partiton(int list[], int left, int right, int chose)
{
    int i, j;
    int pivot;
    i = left;
    j = right + 1;
    SupperCpy(&pivot, list[left]);
    do
    {
        do
            i++;
        while (SupperCmp(list[i], pivot, chose) < 0);
        do
            j--;
        while (SupperCmp(list[j], pivot, chose) > 0);
        if (i < j)
            SWAP(&list[i], &list[j]);
    } while (i < j);
    SWAP(&list[left], &list[j]);
    return j;
}
float quick_sort2(int list[], int left, int right, int chose)
{
    int j;
    clock_t tstart, tfinish;
    tstart = clock();
    if (left < right)
    {
        j = partiton(list, left, right, chose);
        quick_sort2(list, left, j - 1, chose);
        quick_sort2(list, j + 1, right, chose);
    }
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}
void swap(int List[], int i, int j)
{
    int temp = List[i];
    List[i] = List[j];
    List[j] = temp;
}
float quick_sort3(int List[], int left, int right)
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
    quick_sort3(List, left, j);
    quick_sort3(List, i, right);
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}
void createNewArray(int **List, int n)
{
    *List = (int *)malloc(sizeof(int) * n);
    srand((unsigned)clock());
    for (int i = 0; i < n; i++)
        *(*List + i) = rand() % 10;
}
int *copyNewArray(int n, int *root)
{
    int *List;
    List = (int *)malloc(sizeof(int) * n);
    return memcpy(List, root, sizeof(int) * n);
}
int main()
{
    int *List1, *List2;
    float sum = 0;
    createNewArray(&List1, MAX);
    List2 = copyNewArray(MAX, List1);
    printf("Quick Sort 2: %g\n", quick_sort2(List1, 0, MAX - 1, 1));
    printf("Quick Sort 3: %g\n", quick_sort3(List2, 0, MAX - 1));
    // for (int i = 0; i < MAX; i++)
    // {
    //     printf("%d\t", *(List1 + i));
    // }
    // putchar('\n');
    // for (int i = 0; i < MAX; i++)
    //     printf("%d\t", *(List2 + i));
    // putchar('\n');
    free(List1);
    free(List2);
    return 0;
}