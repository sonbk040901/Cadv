#include <stdio.h>
#include <stdarg.h>
void viet()
{
    printf("Viet\n");
}
void FindMax(int _count, ...)
{
    va_list ap;
    va_start(ap, _count);
    void (*func)();
    for (int i = 0; i < _count; i++)
    {
        func = va_arg(ap, void *);
        func();
    }

    va_end(ap);
}

int main()
{
    // int m = FindMax(8, 489, 2258, 5622, 7411, 892, 1045, 6081, 9000);
    // printf("The greatest one is : %d\n", m);
    FindMax(2, viet, viet);
    return 0;
}