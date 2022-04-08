#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    int lines[5];
    lines[0] = 0;
    lines[1] = 0;
    lines[2] = 0;
    lines[3] = 0;
    lines[4] = 0;
    int root;
    char name[256];
    srand((unsigned)clock());
    FILE *f;
    f = fopen("station.data", "w+");
    fputs("[STATIONS]\n", f);
    for (int i = 0; i < 15; i++)
    {
        fprintf(f, "S%d = Ben xe so %d\n", i + 1, i + 1);
    }
    fputs("\n[LINES]", f);
    for (int j = 0; j < 20; j++)
    {
        fprintf(f, "\nM%d = ", j + 1);
        for (int i = 0; i < 5; i++)
        {
            do
            {
                lines[i] = rand() % 15 + 1;
                for (int k = 0; k < i; k++)
                {
                    if (lines[i] == lines[k])
                    {
                        lines[i] = 0;
                        break;
                    }
                }
            } while (!lines[i]);
            fprintf(f, "S%d ", lines[i]);
        }
    }
    fclose(f);
    return 0;
}
