#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    printf("%d\n", 224 & ~(1 << (7 - 2)));
    return 0;
}