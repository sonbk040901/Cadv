
#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"
int main(int argc, char const *argv[])
{
    int i;
    Jval jval;
    JRB tree, tmp;
    tree = make_jrb();
    do
    {
        printf("Hay nhap so nguyen ban muon them vao cay: ");
        scanf("%d", &i);
        jrb_insert_int(tree, i, new_jval_i(i));
    } while (i != 0);
    jrb_traverse(tmp, tree)
    {
        printf("%d\n", jval_i(tmp->val));
    }
    jrb_free_tree(tree);
    return 0;
}
