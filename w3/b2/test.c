#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5
int search(char a[][10], char key[], int *index)
{
    int i;
    for (i = 0; i <= *index; i++)
    {
        if (strcmp(a[i], key) == 0)
        {
            break;
        }
    }
    if (i == *index + 1)
    {
        strcpy(a[i], key);
        *index++;
    }
    return i;
}
int main(int argc, char const *argv[])
{
    char arr[MAX][10];
    char arr2[MAX][10];
    int dem[MAX];
    for (int i = 0; i < MAX; i++)
    {
        dem[i] = 0;
    }
    int index = 0;
    strcpy(arr[0], "nguyet");
    strcpy(arr[1], "tam");
    strcpy(arr[2], "nguyet");
    strcpy(arr[3], "huong");
    strcpy(arr[4], "nguyet");
    for (int i = 0; i < MAX; i++)
    {
        dem[search(arr2, arr[i], &index)]++;
    }
    for (int i = 0; i <= index; i++)
    {
        puts(arr2[i]);
    }
    return 0;
}