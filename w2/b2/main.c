#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 10000
typedef union main
{
    int i;
    long l;
    float f;
    double d;
    void *v;
    char *s;
    char c;
} Jval;
int compare_i(const void *x, const void *y)
{
    return (*((Jval *)x)).i - (*((Jval *)y)).i;
}
void swap_i(void *List, int i, int j)
{
    int temp = (*((Jval *)List + i)).i;
    (*((Jval *)List + i)).i = (*((Jval *)List + j)).i;
    (*((Jval *)List + j)).i = temp;
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
        swap_i(List, i, j);
        if (compare(List + i * size, List + right * size) == 0)
            swap_i(List, ++p, i);
        if (compare((List + j * size), List + right * size) == 0)
            swap_i(List, --q, j);
    }
    swap_i(List, i, right);
    j = i - 1;
    i++;
    for (int k = left; k <= p; k++)
        swap_i(List, k, j--);
    for (int k = right - 1; k >= q; k--)
        swap_i(List, k, i++);
    sort3Way(List, left, j, compare, size);
    sort3Way(List, i, right, compare, size);
    tfinish = clock();
    return (float)(tfinish - tstart) / CLOCKS_PER_SEC;
}

int search(void *List, int left, int right, const void *key, int (*compare)(const void *, const void *), size_t size)
{
    if (left >= right)
        if (compare(List + left * size, key) == 0)
            return left;
        else
            return -1;
    int middle = (left + right) / 2;
    if (compare(List + middle * size, key) > 0)
        return search(List, left, middle - 1, key, compare, size);
    if (compare(List + middle * size, key) < 0)
        return search(List, middle + 1, right, key, compare, size);
    return middle;
}
int search_i(Jval *List, int left, int right, int key)
{
    int size = sizeof(*List);
    return search(List, left, right, &key, compare_i, size);
}
Jval create_i(int Element)
{
    Jval data;
    data.i = Element;
    return data;
}
void create_array_i(Jval **List, size_t size)
{
    *List = (Jval *)malloc(sizeof(Jval) * size);
    srand((unsigned)clock());
    for (int i = 0; i < size; i++)
        *(*List + i) = create_i(rand() % 11);
}
int main(int argc, char const *argv[])
{
    int key;
    int index;
    int begin, end;
    Jval *List;
    create_array_i(&List, MAX);
    sort3Way(List, 0, MAX - 1, compare_i, sizeof(*List));
    // for (int i = 0; i < MAX; i++)
    // {
    //     printf("%d\t", (*(List + i)).i);
    // }
    // printf("\n");
    printf("Hay nhap vao gia tri ma ban muon tim kiem: ");
    scanf("%d", &key);
    index = search_i(List, 0, MAX - 1, key);
    if (index < 0)
    {
        printf("Khong tim thay vi tri cua gia tri can tim\n");
    }
    else
    {
        begin = index;
        end = index;
        while ((*(List + (--begin))).i == key && begin > -1)
            ;
        while ((*(List + (++end))).i == key && end < MAX)
            ;
        printf("Cac vi tri co gia tri thoa man la tu %d -> %d\n", begin + 1, end - 1);
    }
    free(List);
    return 0;
}
