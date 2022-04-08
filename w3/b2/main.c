#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#define MAX 3
#define MIN 2
typedef struct Data
{
    long number;
    char *name;
} DT;
typedef struct phonebook
{
    DT *List;
    int size;
    int top;
} PB;
DT getPhoneNumber(const PB PhoneBook, char *key)
{
    int i = 0;
    while (i <= PhoneBook.top && strcmp(PhoneBook.List[i++].name, key) != 0)
        ;
    DT notFound;
    notFound.number = -1;
    notFound.name = NULL;
    if (i == PhoneBook.top + 1 && strcmp(PhoneBook.List[i - 1].name, key) != 0)
        return notFound;
    return PhoneBook.List[--i];
}
void addPhoneNumber(PB *PhoneBook, DT data)
{
    int i;
    if (PhoneBook->top >= PhoneBook->size)
    {
        PhoneBook->List = (DT *)realloc(PhoneBook->List, sizeof(DT) * (PhoneBook->size + 1 + MIN));
        PhoneBook->size += MIN;
        puts("Full bo nho,da cap phat them o nho");
    }
    for (i = 0; i <= PhoneBook->top; i++)
        if (strcmp(PhoneBook->List[i].name, data.name) == 0)
        {
            (PhoneBook->List + i)->number = data.number;
            break;
        }
    if (i == PhoneBook->top + 1)
    {
        PhoneBook->List[++(PhoneBook->top)] = data;
    }
}
void createPhoneBook(PB *PhoneBook, size_t size)
{
    PhoneBook->top = -1;
    PhoneBook->size = size - 1;
    PhoneBook->List = (DT *)malloc(sizeof(DT) * MAX);
}
void dropPhoneBook(PB *PhoneBook)
{
    for (int i = 0; i <= PhoneBook->top; i++)
    {
        free((PhoneBook->List + i)->name);
    }
    free(PhoneBook->List);
}
int main(int argc, char const *argv[])
{
    char name[30];
    long number;
    DT data;
    PB PhoneBook;
    createPhoneBook(&PhoneBook, MAX);
    for (int i = 0; i < 6; i++)
    {
        puts("Nhap ten:");
        __fpurge(stdin);
        data.name = (char *)realloc(NULL, sizeof(char) * 30);
        fgets(data.name, 30, stdin);
        puts("Nhap std:");
        scanf("%ld", &(data.number));
        addPhoneNumber(&PhoneBook, data);
    }
    __fpurge(stdin);
    fgets(name, 30, stdin);
    number = getPhoneNumber(PhoneBook, name).number;
    if (number == -1)
        printf("Not found\n");
    else
        printf("%ld\n", number);
    dropPhoneBook(&PhoneBook);
    return 0;
}