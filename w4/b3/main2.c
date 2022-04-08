#include "jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>
#define MAX_NAME 30
#define MAX_CHOSE 4
typedef struct
{
    char *name;
    Jval val;
} PhoneBook;
void menu()
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%25sMENU%25s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Add phone");
    printf("\t\t\t|  2  |   %-45s|\n", "Search phone");
    printf("\t\t\t|  3  |   %-45s|\n", "Print list");
    printf("\t\t\t|  4  |   %-45s|\n", "End program");
    printf("\t\t\t|======================================================|\n");
}
int main(int argc, char const *argv[])
{
    int chose;
    JRB tree, root;
    PhoneBook *pb;
    char *name;
    int number;
    JRB find;
    tree = make_jrb();
    name = (char *)malloc(sizeof(char) * MAX_NAME);
    do
    {
        menu();
        printf("Hay nhap lua chon cua ban: ");
        while (!scanf("%d", &chose))
        {
            printf("Nhap sai: Khong duoc nhap chu!\nChi duoc nhap so tu %d - %d\n", 1, MAX_CHOSE);
            menu();
            printf("Hay nhap lua chon cua ban: ");
            __fpurge(stdin);
        }
        __fpurge(stdin);
        switch (chose)
        {
        case 1:
            /* Lua chon thu nhat */

            /*Nhap thong tin lien he*/
            printf("Hay nhap ten cua lien he moi: ");
            __fpurge(stdin);
            fgets(name, MAX_NAME, stdin);
            name[strlen(name) - 1] = 0;
            printf("Hay nhap so dien thoai: ");
            scanf("%d", &number);
            putchar('\n');

            /*Tao lien he*/
            pb = (PhoneBook *)malloc(sizeof(PhoneBook));
            pb->name = strdup(name);
            pb->val = new_jval_i(number);

            /*Them lien he vao danh ba*/
            find = jrb_find_str(tree, name);
            if (find)
                find->val.v = pb;
            else
                jrb_insert_str(tree, pb->name, new_jval_v((void *)pb));
            break;
        case 2:
            /* Lua chon thu hai */

            /*Nhap thong tin lien he*/
            printf("Hay nhap ten cua lien he muon tim: ");
            __fpurge(stdin);
            fgets(name, MAX_NAME, stdin);
            name[strlen(name) - 1] = 0;

            /*Tim kiem*/
            find = jrb_find_str(tree, name);
            if (find)
            {
                pb = (PhoneBook *)jval_v(find->val);
                printf("Thong tin cua lien he:\nName: %s\tPhone: %d\n", pb->name, jval_i(pb->val));
            }
            else
            printf("Khong tim thay ten lien he trong danh ba\n");
            break;
        case 3:
            /* Lua chon thu ba */

            jrb_traverse(root, tree)
            {
                pb = (PhoneBook *)jval_v(root->val);
                printf("%-10s%d\n", pb->name, jval_i(pb->val));
            }
            break;
        case 4:
            printf("\tKet thuc chuong trinh\n");
            chose = 0;
            jrb_traverse(root, tree)
            {
                free(jval_s(root->key));
                free(jval_s(root->val));
            }
            jrb_free_tree(tree);
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}
