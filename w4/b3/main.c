#include "jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>
#define MAX_NAME 30
typedef struct
{
    char *name;
    Jval val;
} PhoneBook;
int main(int argc, char const *argv[])
{
    JRB tree, tmp;
    tree = make_jrb();
    PhoneBook *pb;
    char *name;
    int number;
    int i = -3;
    do
    {
        /*Nhap thong tin lien he*/
        printf("Hay nhap ten cua lien he moi: ");
        name = (char *)malloc(sizeof(char) * MAX_NAME);
        __fpurge(stdin);
        fgets(name, MAX_NAME, stdin);
        name[strlen(name) - 1] = 0;
        printf("Hay nhap so dien thoai: ");
        scanf("%d", &number);
        putchar('\n');
        /*Tao lien he*/
        pb = (PhoneBook *)malloc(sizeof(PhoneBook));
        pb->name = name;
        pb->val = new_jval_i(number);
        /*Them lien he vao danh ba*/
        JRB find;
        find = jrb_find_str(tree, name);
        if (find)
            find->val.v = pb;
        else
            jrb_insert_str(tree, pb->name, new_jval_v((void *)pb));
    } while (i++);
    jrb_traverse(tmp, tree)
    {
        pb = (PhoneBook *)jval_v(tmp->val);
        printf("%-10s%d\n", pb->name, jval_i(pb->val));
        free(jval_s(tmp->key));
        free(jval_s(tmp->val));
        free(pb);
    }
    jrb_free_tree(tree);
    return 0;
}
