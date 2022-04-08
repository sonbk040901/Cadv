#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "fields.h"
#include "jval.h"
typedef struct
{
    char *name;
    int score;
    char *key;
} Person;

int main()
{
    JRB t, tmp;
    IS is;
    Person *p;
    int nsize, i;
    is = new_inputstruct("text.txt");
    t = make_jrb();

    while (get_line(is) >= 0)
    {
        if (is->NF > 1)
        {

            /* Each line is name followed by score.  The score is easy to get. */

            p = malloc(sizeof(Person));
            p->score = atoi(is->fields[is->NF - 1]);

            /* The name is a different matter, because names may be composed of any 
         number of words with any amount of whitespace.  We want to create a 
         name string that has each word of the name separated by one space. 
  
         Our first task is to calculate the ssize of our name. */

            nsize = strlen(is->fields[0]);

            for (i = 1; i < is->NF - 1; i++)
                nsize += (strlen(is->fields[i]) + 1);

            /* We then allocate the string and copy the first word into the string. */

            p->name = (char *)malloc(sizeof(char) * (nsize + 1));
            strcpy(p->name, is->fields[0]);

            /* We copy in the remaining words, but note how we do so by calling strcpy
         into the exact location of where the name goes, rather than, say, repeatedly
         calling strcat() as we would do in a C++-like solution.  This is much more 
         efficient (not to mention inconvenient) than using strcat(). */

            nsize = strlen(is->fields[0]);
            for (i = 1; i < is->NF - 1; i++)
            {
                p->name[nsize] = ' ';
                strcpy(p->name + nsize + 1, is->fields[i]);
                nsize += strlen(p->name + nsize);
            }

            /* We create a key for inserting into the red-black tree.  That is going
         to be the score, padded to 10 characters, followed by the name.  We 
         allocate (nsize+12) characters: nsize for the name, 10 for the score,
         one for the space, and one for the null character. */

            p->key = (char *)malloc(sizeof(char) * (nsize + 12));
            sprintf(p->key, "%10d %s", p->score, p->name);

            jrb_insert_str(t, p->key, new_jval_v((void *)p));
        }
    }

    /* Traverse the tree and print the people. */

    jrb_traverse(tmp, t)
    {
        p = (Person *)(tmp->val.v);
        printf("%-40s %2d\n", p->name, p->score);
    }
    jrb_free_tree(t);
    return 0;
}