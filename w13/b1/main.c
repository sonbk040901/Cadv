#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lbr/dllist.h"
#include "lbr/jrb.h"
#include "lbr/jval.h"
char next[9][4] = {"1399", "0249", "1599", "0469", "1357", "2489", "3799", "4689", "5799"};
int find_i(char *str)
{
    for (int i = 0; i < 9; i++)
        if (str[i] == '0')
            return i;
}
Jval swap(int a, int b, char *str)
{
    char *s;
    s = strdup(str);
    char temp;
    temp = s[a];
    s[a] = s[b];
    s[b] = temp;
    return new_jval_s(s);
}
void add(Dllist open, char *str, JRB close)
{
    int index = find_i(str);
    Jval s;
    for (int i = 0; i < 4; i++)
    {
        if (next[index][i] != '9')
        {
            s = swap(index, next[index][i] - 48, str);
            if (!jrb_find_str(close, jval_s(s)))
            {
                dll_append(open, s);
            }
        }
        else
            break;
    }
}
int main(int argc, char const *argv[])
{
    char finish[] = "123456780", start[] = "087645321";
    Dllist open, root;
    open = new_dllist();
    JRB close;
    int dem = 0;
    close = make_jrb();
    dll_append(open, new_jval_s(start));
    jrb_insert_str(close, start, new_jval_i(0));
    while (!dll_empty(open))
    {
        root = dll_first(open);
        if (strcmp(jval_s(root->val), finish) == 0)
        {
            for (int i = 0; i < 9; i++)
            {
                printf("%c\t", jval_s(root->val)[i]);
                if ((i + 1) % 3 == 0)
                {
                    printf("\n");
                }
            }
            break;
        }
        add(open, jval_s(root->val), close);
        dll_delete_node(root);
        dem++;
    }
    printf("%d\n", dem);
    jrb_free_tree(close);
    return 0;
}