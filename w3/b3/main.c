#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#define INITIAL_SIZE 3
#define INCREMENTAL_SIZE 2
typedef struct
{
    void *key;
    void *value;
} Entry;
typedef struct
{
    Entry *entries;
    int size, total;
    Entry (*makeNode)(void *, void *);
    int (*compare)(void *, void *);
} SymbolTable;
//Hàm tạo 1 entry mới, mỗi entry tương ứng với cặp thông tin
// tên người – số điện thoại
Entry makePhone(void *name, void *phone)
{
    Entry res;
    res.key = strdup((char *)name);
    res.value = malloc(sizeof(long));
    memcpy(res.value, phone, sizeof(long));
    return res;
}

//Hàm so sánh 2 số điện thoại
int comparePhone(void *key1, void *key2)
{
    return strcmp((char *)key1, (char *)key2);
}
SymbolTable createSymbolTable(Entry (*makenode)(void *, void *), int (*compare)(void *, void *), size_t size)
{
    SymbolTable root;
    root.entries = (Entry *)malloc(sizeof(Entry) * size);
    root.total = -1;
    root.size = size - 1;
    root.compare = compare;
    root.makeNode = makenode;
    return root;
}
//Thêm một entry mới vào bảng dữ liệu
void addEntry(void *key, void *value, SymbolTable *book)
{
    Entry NewNode = book->makeNode(key, value);
    // printf("%s\t%d\n", (char *)(NewNode.key), *(long *)(NewNode.value));
    if (book->total >= book->size)
    {
        book->entries = (Entry *)realloc(book->entries, sizeof(Entry) * (book->size + INCREMENTAL_SIZE + 1));
        book->size += INCREMENTAL_SIZE;
        printf("Full bo nho, da cap phat them%d\n", book->total);
    }
    int i = 0;
    while (i <= book->total && book->compare(book->entries + i, &NewNode) != 0 && ++i)
        ;
    if (i == book->total + 1)
    {
        *(book->entries + i) = NewNode;
        book->total++;
    }
    else
    {
        memcpy((book->entries + i)->value, NewNode.value, sizeof(long));
    }
}

//Tìm entry dựa trên key cho trước
Entry *getEntry(void *key, SymbolTable book)
{
    for (int i = 0; i <= book.total; i++)
    {
        if (book.compare((book.entries + i)->key, key) == 0)
        {
            return book.entries + i;
        }
    }
    return NULL;
}

//Xóa bảng dữ liệu
void dropSymbolTable(SymbolTable *tab)
{
    for (int i = 0; i <= tab->total; i++)
    {
        free((tab->entries + i)->key);
        free((tab->entries + i)->value);
    }
    free(tab->entries);
}
int main(int argc, char const *argv[])
{
    SymbolTable phoneBook = createSymbolTable(makePhone, comparePhone, INITIAL_SIZE);
    long number = 9839847751;
    char name1[] = "Ta Tuan Anh1";
    addEntry(name1, &number, &phoneBook);
    number = 9839847752;
    char name2[] = "Ta Tuan Anh2";
    addEntry(name2, &number, &phoneBook);
    number = 9839847753;
    char name3[] = "Ta Tuan Anh3";
    addEntry(name3, &number, &phoneBook);
    number = 9839847754;
    char name4[] = "Ta Tuan Anh4";
    addEntry(name4, &number, &phoneBook);
    number = 9839847755;
    char name5[] = "Ta Tuan Anh5";
    addEntry(name5, &number, &phoneBook);
    for (int i = 0; i <= phoneBook.total; i++)
    {
        printf("%s\t%ld\n", (char *)((phoneBook.entries + i)->key), *(long *)((phoneBook.entries + i)->value));
    }
    dropSymbolTable(&phoneBook);
    return 0;
}
