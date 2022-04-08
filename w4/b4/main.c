#include "jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>
#define MAX_NAME 30
#define MAX_CHOSE 4
#define MAX_LINE_SIZE 1000
typedef struct
{
    char *name;
    Jval val;
} PhoneBook;
typedef struct FDT
{
    char *name;
    FILE *f;
    int LN;
    char **line;
} * FDATA;
void menu()
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%25sMENU%25s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Updata phonebook");
    printf("\t\t\t|  2  |   %-45s|\n", "Search phone");
    printf("\t\t\t|  3  |   %-45s|\n", "Print list");
    printf("\t\t\t|  4  |   %-45s|\n", "End program");
    printf("\t\t\t|======================================================|\n");
}
int popData(FDATA fd, char *filename)
{
    fd->name = filename;
    fd->f = fopen(fd->name, "r");
    if (!fd->f)
    {
        return 0;
    }
    int LN = 0;
    char line[MAX_LINE_SIZE];
    while (!feof(fd->f))
    {
        if (fgets(line, MAX_LINE_SIZE, fd->f))
        {
            fd->line = (char **)realloc(fd->line, sizeof(char *) * (++LN));
            *(fd->line + LN - 1) = strdup(line);
        }
        else
        {
            break;
        }
    }
    fd->LN = LN;
    return 1;
}
int main()
{
    int chose;
    JRB tree, root;
    PhoneBook *pb;
    char *name;
    int number;
    JRB find;
    FDATA fData;
    char filename[100];
    int real = 0;
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
            real = 1;
            /*Nhap thong tin lien he*/
            printf("Hay nhap ten file data ban muon cap nhat danh ba: ");
            fgets(filename, 100, stdin);
            filename[strlen(filename) - 1] = 0;
            fData = (FDATA)realloc(NULL, sizeof(struct FDT));
            fData->LN = 0;
            int index, j;
            if (popData(fData, filename))
            {
                char str[10][MAX_LINE_SIZE];
                for (int i = 0; i < fData->LN; i++)
                {
                    index = 0, j = 0;
                    while (sscanf(*(fData->line + i) + index, "%s", str[j]) > 0)
                    {
                        index = ((int)(strstr(*(fData->line + i) + index, str[j]) - *(fData->line + i)) + strlen(str[j++]));
                    }

                    /*Tao lien he*/
                    pb = (PhoneBook *)realloc(NULL, sizeof(PhoneBook));
                    for (int k = 1; k < j - 1; k++)
                    {
                        strcat(str[0], " ");
                        strcat(str[0], str[k]);
                    }
                    // puts(str[j - 1]);
                    pb->name = strdup(str[0]);
                    pb->val = new_jval_l(atol(str[j - 1]));

                    /*Them lien he vao danh ba*/
                    find = jrb_find_str(tree, str[0]);
                    if (find)
                        find->val.v = pb;
                    else
                        jrb_insert_str(tree, pb->name, new_jval_v((void *)pb));
                }
            }
            else
            {
                printf("Khong the mo file data!\n");
                break;
            }
            break;
        case 2:
            /* Lua chon thu hai */

            if (!real)
            {
                puts("Chua co lien he nao duoc them vao danh ba!!!");
                break;
            }
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
                printf("Thong tin cua lien he:\nName: %s\tPhone: %ld\n", pb->name, jval_l(pb->val));
            }
            else
                printf("Khong tim thay ten lien he trong danh ba\n");
            break;
        case 3:
            /* Lua chon thu ba */
            if (!real)
            {
                puts("Chua co lien he nao duoc them vao danh ba!!!");
                break;
            }

            jrb_traverse(root, tree)
            {
                pb = (PhoneBook *)jval_v(root->val);
                printf("%-30s%ld\n", pb->name, jval_l(pb->val));
            }
            break;
        case 4:
            if (!real)
            {
                chose = 0;
                break;
            }

            chose = 0;
            jrb_traverse(root, tree)
            {
                free(jval_s(root->key));
                free(jval_s(root->val));
            }
            for (int i = 0; i < fData->LN; i++)
            {
                free(*(fData->line + i));
            }
            free(name);
            free(fData->line);
            free(fData);
            jrb_free_tree(tree);
            printf("\tKet thuc chuong trinh\n");
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}
