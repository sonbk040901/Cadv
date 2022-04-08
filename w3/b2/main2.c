#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#define MAX 3
#define MIN 2
typedef struct Data
{
    char *name;
    int number;
} DT;
typedef struct Phonebook
{
    DT *List;
    int size;
    int top;
} PB;
PB createPhoneBook(size_t size)
{
    PB PhoneBook;
    PhoneBook.List = (DT *)malloc(sizeof(DT) * size);
    PhoneBook.top = -1;
    PhoneBook.size = size - 1;
    return PhoneBook;
}
void dropPhoneBook(PB *PhoneBook)
{
    for (int i = 0; i <= PhoneBook->top; i++)
    {
        free((PhoneBook->List + i)->name);
    }
    free(PhoneBook->List);
}
int BinarySearch(PB *Phonebook, int left, int right, char *name, int *found);
void addPhoneNumber(char *name, long number, PB *book);
DT *getPhoneNumber(char *name, PB book);
int main(int argc, char const *argv[])
{

    return 0;
}
