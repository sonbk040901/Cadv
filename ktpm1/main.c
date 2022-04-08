#include "data_struct.h"
BTA *hk; //Ho khau
BTA *nk; //Nhan khau
int file_not_found(char *name)
{
    FILE *f;
    f = fopen(name, "r");
    if (!f)
        return 1;
    else
    {
        fclose(f);
        return 0;
    }
}
void open_file_bt(char *name1, char *name2)
{
    if (file_not_found(name1))
    {
        hk = btcrt(name1, 0, FALSE);
        btdups(hk, FALSE);
    }
    else
        hk = btopn(name1, 0, FALSE);
    if (file_not_found(name2))
    {
        nk = btcrt(name2, 0, FALSE);
        btdups(nk, TRUE);
    }
    else
        nk = btopn(name2, 0, 1);
}
void close_file_bt()
{
    btcls(nk);
    btcls(hk);
}
DAY get_time_now()
{
    time_t raw_time;
    time(&raw_time);
    struct tm now;
    now = *localtime(&raw_time);
    char *buffer[80];
    // strftime(buffer, 80, "%d/%m/%Y - %I:%M%p", &now);
    // printf("Date & time đã định dạng theo hàm strftime là: \n|%s|\n", buffer);
    return now;
}
COVID covid()
{
    int s, v;
    COVID khai_bao;
    khai_bao.thoi_gian = get_time_now();
    khai_bao.so_ngay = 21;
    printf("Nhap muc do cach ly cua nguoi nay(0: F0, 1: F1, 2: F2...): ");
    scanf("%d", &s);
    khai_bao.muc_do = s;
    printf("Nguoi nay da test covid chua(1: roi, 0: chua): ");
    scanf("%d", &s);
    khai_bao.test = s;
    __fpurge(stdin);
    if (s)
    {
        printf("Da test bao nhieu lan: ");
        scanf("%d", &s);
        khai_bao.so_lan_test = s;
        for (int i = 0; i < s; i++)
        {
            printf("Ket qua lan thu %d: ", i + 1);
            scanf("%d", &v);
            khai_bao.ket_qua[i] = v;
        }
        __fpurge(stdin);
        if (s)
        {
            printf("Ngay test gan nhat(dd/mm/yyyy): ");
            scanf("%d/%d/%d", &(khai_bao.ngay_test.tm_mday), &(khai_bao.ngay_test.tm_mon), &(khai_bao.ngay_test.tm_year));
            khai_bao.ngay_test.tm_mon--;
            khai_bao.ngay_test.tm_year -= 1900;
        }
        strcpy(khai_bao.tinh_trang_sk, "On dinh");
    }
    __fpurge(stdin);
    return khai_bao;
}
void print_list()
{
    PS *ps;
    ps = (PS *)malloc(sizeof(PS));
    char str[100];
    int dsize;
    btpos(nk, 1);
    printf("  %-5s|%9s%-15s|  %-9s|%20s%-35s|\n", "ID", "", "TEN", "HO KHAU", "", "DIA CHI");
    while (!btseln(nk, str, (char *)ps, sizeof(PS), &dsize))
    {
        // printf("%d %d1\n", ps->chet, ps->vang);
        if (!ps->chet && !ps->vang)
            printf("%05u%5s%-25s%-14s%s\n", ps->id, "", ps->ten, ps->so_ho_khau, ps->nguyen_quan);
    }
    free(ps);
    
}
void khai_bao_covid()
{
    char s[10];
    PS ps, *c;
    int dsize;
    print_list();
    printf("Hay nhap id cua nguoi can khai bao: ");
    scanf("%d", &dsize);
    sprintf(s, "%02d", dsize);
    __fpurge(stdin);
    c = (PS *)malloc(sizeof(PS));
    if (btsel(nk, s, (char *)c, sizeof(PS), &dsize))
    {
        printf("Khong tim thay nhan khau nay!!\n");
    }
    else
    {
        ps = *c;
        ps.lien_quan_covid = 1;
        ps.thong_tin_khai_bao = covid();
        *c = ps;
        btupd(nk, s, (char *)c, sizeof(PS));
    }
    free(c);
}
void thong_ke_tt_covid()
{
    printf("\t\t\t\t\t\tTHONG KE THONG TIN COVID\n");
    PS nhan_khau;
    char *c, buffer[20];
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    printf("  %-5s|%5s%-15s| %-7s| %-5s|%-12s|  %-8s |%1s%-14s|%20s%-30s|\n", "ID", "", "TEN", "MUC DO", "TEST", "LAN TEST CUOI", "KET QUA", "", "TINH TRANG SK", "", "DIA CHI");
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (!nhan_khau.chet && !nhan_khau.vang)
            if (nhan_khau.lien_quan_covid)
            {
                if (!nhan_khau.thong_tin_khai_bao.test || !nhan_khau.thong_tin_khai_bao.so_lan_test)
                {
                    strcpy(buffer, "Chua test");
                    nhan_khau.thong_tin_khai_bao.so_lan_test = 0;
                }
                else
                    strftime(buffer, 80, "%d/%m/%Y", &nhan_khau.thong_tin_khai_bao.ngay_test);
                printf("%05u    %-22s F%-7c%-7c%-13s%-14s%-15s%s\n", nhan_khau.id, nhan_khau.ten, nhan_khau.thong_tin_khai_bao.muc_do + 48, nhan_khau.thong_tin_khai_bao.so_lan_test + 48, buffer, nhan_khau.thong_tin_khai_bao.ket_qua[nhan_khau.thong_tin_khai_bao.so_lan_test - 1] ? "Duong tinh" : "Am tinh", nhan_khau.thong_tin_khai_bao.tinh_trang_sk, nhan_khau.nguyen_quan);
            }
    }
    free(c);
}
void resest()
{
    PS ps, *c;
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (PS *)malloc(sizeof(PS));
    while (!btseln(nk, str, (char *)c, sizeof(PS), &dsize))
    {
        ps = *c;
        ps.lien_quan_covid = 0;
        ps.thong_tin_khai_bao.muc_do = 0;
        strcpy(ps.thong_tin_khai_bao.tinh_trang_sk, "On dinh");
        ps.thong_tin_khai_bao.test = 0;
        ps.thong_tin_khai_bao.so_lan_test = 0;
        ps.chet = 0;
        ps.tru = 0;
        ps.vang = 0;
        // ps.chu_ho = !strcmp("Chủ hộ", ps.quan_he_chu_ho);
        // !strcmp(ps.nguyen_quan, "Số 3, ngõ 568 Đường Láng, Hà Nội") ? strcpy(ps.nguyen_quan, "So 3, ngo 568 Duong Lang, Ha Noi") : 0;
        *c = ps;
        btupd(nk, str, (char *)c, sizeof(PS));
    }
}
void set(PS *ps)
{
    ps->lien_quan_covid = 0;
    ps->thong_tin_khai_bao.muc_do = 0;
    strcpy(ps->thong_tin_khai_bao.tinh_trang_sk, "On dinh");
    ps->thong_tin_khai_bao.test = 0;
    ps->thong_tin_khai_bao.so_lan_test = 0;
    ps->chet = 0;
    ps->tru = 0;
    ps->vang = 0;
}
void khai_bao_tam_tru()
{
    PS *tt;
    char str[100];
    char id[4];
    int max = 0, dsize;
    tt = (PS *)malloc(sizeof(PS));
    __fpurge(stdin);
    printf("Hay nhap ten nguoi muon tam tru: ");
    fgets(tt->ten, 50, stdin);
    tt->ten[strlen(tt->ten) - 1] = 0;
    btpos(nk, 1);
    set(tt);
    while (!bnxtky(nk, str, &dsize))
        max++;
    id[2] = 0;
    id[1] = max % 10 + 48;
    id[0] = max / 10 + 48;
    tt->id = max + 1;
    tt->tru = 1;
    strcpy(tt->tam_tru.ma_giay_tam_tru, "TT001");
    tt->tam_tru.so_dien_thoai_nguoi_dk = 123456789;
    tt->tam_tru.tu_ngay = get_time_now();
    printf("Hay nhap ly do tam tru: ");
    fgets(tt->tam_tru.li_do, 100, stdin);
    tt->tam_tru.li_do[strlen(tt->tam_tru.li_do) - 1] = 0;
    printf("Dang ky tam tru den ngay(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &(tt->tam_tru.den_ngay.tm_mday), &(tt->tam_tru.den_ngay.tm_mon), &(tt->tam_tru.den_ngay.tm_year));
    tt->tam_tru.den_ngay.tm_mon--;
    tt->tam_tru.den_ngay.tm_year -= 1900;
    btins(nk, id, (char *)tt, sizeof(PS));
    free(tt);
}
int main(int argc, char const *argv[])
{
    if (btinit())
    {
        printf("Err\n");
        return 0;
    }
    open_file_bt("ho_khau.data", "nhan_khau.data");
    // resest();
    khai_bao_covid();
    // thong_ke_tt_covid();
    // khai_bao_tam_tru();
    close_file_bt();
    return 0;
}