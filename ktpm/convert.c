#include "data_struct.h"
int main(int argc, char const *argv[])
{
    PS s;
    BTA *nk;
    btinit();
    btcrt("nhan_khau.data", 0, FALSE);
    btdups(nk, TRUE);

    return 0;
}