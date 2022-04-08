#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 10
int *createArray(int *size)
{
    size = (int *)malloc(MAX * sizeof(int));
    srand((unsigned)clock());
    for (int i = 0; i < MAX; i++)
    {
        size[i] = rand() % 11;
    }
    return size;
}
void SWAP(int *num1, int *num2)
{
    int tmp;
    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}
// int partition(int a[], int left, int right)
// {
//     int pivot,i,j;
//     i=left;
//     j=right;

// }

void sort2way(int list[], int left, int right)
{
    int pivot, i, j;
    if (left < right)
    {
        i = left;
        j = right + 1;
        pivot = list[left];
        do
        {
            do
                i++;
            while (list[i] < pivot);
            do
                j--;
            while (list[j] > pivot);
            if (i < j)
                SWAP(&list[i], &list[j]);
        } while (i < j);
        SWAP(&list[left], &list[j]);
        sort2way(list, left, j - 1);
        sort2way(list, j + 1, right);
    }
}
void exch(int a[], int i, int j)
{
    int tmp;
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}
void sort3way(int a[], int l, int r)
{
    if (r <= l)
        return;
    int i = l - 1, j = r;
    int p = l - 1, q = r;
    while (1)
    {
        while (a[++i] < a[r])
            ;

        while (a[r] < a[--j])
            if (j == l)
                break;
        if (i >= j)
            break;
        exch(a, i, j);
        if (a[i] == a[r])
            exch(a, ++p, i);
        if (a[j] == a[r])
            exch(a, --q, j);
    }
    exch(a, i, r);
    j = i - 1;
    i = i + 1;
    for (int k = 1; k <= p; k++)
        exch(a, k, j--);
    for (int k = r - 1; k >= q; k--)
        exch(a, k, i++);
    sort3way(a, l, j);
    sort3way(a, i, r);
}
int main()
{
    int *a, *b, *c;
    double time_cpu1, time_cpu2, time_cpu3, time_cpu4;
    clock_t t1, t2, t3, t4, t5, t6, t7, t8;
    a = createArray(a);
    b = createArray(b);
    c = createArray(c);
    for (int i = 0; i < MAX; i++)
    {
        printf("%5d", a[i]);
    }
    printf("\n\n\n");
    // t1 = clock();
    // sort2way(a, 0, MAX - 1);
    // t2 = clock();
    // time_cpu1 = ((double)t2 - t1) / CLOCKS_PER_SEC;
    // printf("Thoi gian  khi dung sort2 la:%f s\n\n", time_cpu1);
    // t3 = clock();
    // sort3way(b, 0, MAX - 1);
    // t4 = clock();
    // time_cpu2 = ((double)t4 - t3) / CLOCKS_PER_SEC;
    // printf("Thoi gian chay khi dung sort3 la:%f s\n\n", time_cpu2);
    for (int i = 0; i < MAX; i++)
        printf("%5d", b[i]);
    printf("\n");
    for (int i = 0; i < MAX; i++)
    {
        printf("%5d", c[i]);
    }
    printf("\n");
    free(a);
    free(b);
    free(c);
    return 0;
}