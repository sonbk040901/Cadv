#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#define MAX 100
typedef struct Data
{
    long number;
    char name[30];
} DT;
typedef struct phonebook
{
    DT List[MAX];
    int top;
} PB;
int getPhoneNumber(const PB PhoneBook, const char *key)
{
    int i = 0;
    while (i <= PhoneBook.top && strcmp(PhoneBook.List[i++].name, key) != 0)
        ;
    if (i == PhoneBook.top + 1 && strcmp(PhoneBook.List[i - 1].name, key) != 0)
        return -1;
    return PhoneBook.List[--i].number;
}
void addPhoneNumber(PB *PhoneBook, DT data)
{
    int i;
    for (i = 0; i <= PhoneBook->top; i++)
        if (strcmp(PhoneBook->List[i].name, data.name) == 0)
        {
            PhoneBook->List[i].number = data.number;
            break;
        }
    if (i == PhoneBook->top + 1)
    {
        PhoneBook->List[++(PhoneBook->top)] = data;
    }
}
int main(int argc, char const *argv[])
{
    char name[30];
    long number;
    DT data;
    PB PhoneBook;
    PhoneBook.top = -1;
    for (int i = 0; i < 2; i++)
    {
        puts("Nhap ten:");
        __fpurge(stdin);
        fgets(data.name, 30, stdin);
        puts("Nhap std:");
        scanf("%ld", &(data.number));
        addPhoneNumber(&PhoneBook, data);
    }
    __fpurge(stdin);
    fgets(name, 30, stdin);
    number = getPhoneNumber(PhoneBook, name);
    if (number == -1)
        return 0;
    else
        printf("%ld\n", number);
    return 0;
}