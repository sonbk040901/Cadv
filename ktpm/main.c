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
    }
    strcpy(khai_bao.tinh_trang_sk, "On dinh");
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
            printf("%5u%5s%-25s%-14s%s\n", ps->id, "", ps->ten, ps->so_ho_khau, ps->nguyen_quan);
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
void print_tam_tru(PS *ps)
{
    char buf1[100], buf2[100];
    printf("\t\t\t\t\t\t\t\tGIAY TAM TRU\n");
    printf("%40sHo va ten: %-30sMa giay tam tru: %-30s\n", "", ps->ten, ps->tam_tru.ma_giay_tam_tru);
    strftime(buf1, 80, "%d/%m/%Y", &(ps->tam_tru.tu_ngay));
    strftime(buf2, 80, "%d/%m/%Y", &(ps->tam_tru.den_ngay));
    printf("%40sTu ngay: %-32sDen ngay: %s\n", "", buf1, buf2);
    printf("%40sDia chi thuong tru truoc khi chuyen den: %s\n", "", ps->dia_chi_thuong_tru_tkcd);
    printf("%40sLy do tam tru: %s\n", "", ps->tam_tru.li_do);
    printf("%40sSo dien thoai nguoi dang ky: %010d\n", "", ps->tam_tru.so_dien_thoai_nguoi_dk);
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
    strcpy(tt->so_ho_khau, "NULL");
    while (!bnxtky(nk, str, &dsize))
        max++;
    max++;
    id[2] = 0;
    id[1] = max % 10 + 48;
    id[0] = max / 10 + 48;
    tt->id = max;
    tt->tru = 1;
    strcpy(tt->tam_tru.ma_giay_tam_tru, "TT001");
    tt->tam_tru.so_dien_thoai_nguoi_dk = 353764901;
    tt->tam_tru.tu_ngay = get_time_now();
    printf("Hay nhap dia chi thuong tru: ");
    fgets(tt->dia_chi_thuong_tru_tkcd, 100, stdin);
    tt->dia_chi_thuong_tru_tkcd[strlen(tt->dia_chi_thuong_tru_tkcd) - 1] = 0;
    printf("Hay nhap ly do tam tru: ");
    fgets(tt->tam_tru.li_do, 100, stdin);
    tt->tam_tru.li_do[strlen(tt->tam_tru.li_do) - 1] = 0;
    printf("Dang ky tam tru den ngay(dd/mm/yyyy): ");
    scanf("%d/%d/%d", &(tt->tam_tru.den_ngay.tm_mday), &(tt->tam_tru.den_ngay.tm_mon), &(tt->tam_tru.den_ngay.tm_year));
    tt->tam_tru.den_ngay.tm_mon--;
    tt->tam_tru.den_ngay.tm_year -= 1900;
    print_tam_tru(tt);
    btins(nk, id, (char *)tt, sizeof(PS));
    free(tt);
}
void print_tam_vang(PS *ps)
{
    char buf1[100], buf2[100];
    printf("\t\t\t\t\t\t\t\tGIAY TAM VANG\n");
    printf("%40sHo va ten: %-30sMa giay tam vang: %-30s\n", "", ps->ten, ps->tam_vang.ma_giay_tam_vang);
    strftime(buf1, 80, "%d/%m/%Y", &(ps->tam_vang.tu_ngay));
    strftime(buf2, 80, "%d/%m/%Y", &(ps->tam_vang.den_ngay));
    printf("%40sTu ngay: %-32sDen ngay: %s\n", "", buf1, buf2);
    printf("%40sDia chi dang ky tam tru: %s\n", "", ps->tam_vang.noi_tam_tru);
    printf("%40sLy do tam vang: %s\n", "", ps->tam_vang.li_do);
}
void khai_bao_tam_vang()
{
    PS *tv;
    char str[100];
    char id[4], s[100];
    int max = 0, dsize;
    tv = (PS *)malloc(sizeof(PS));
    __fpurge(stdin);
    print_list();
    printf("Hay nhap id nguoi muon tam vang: ");
    scanf("%d", &dsize);
    sprintf(s, "%02d", dsize);
    __fpurge(stdin);
    if (btsel(nk, s, (char *)tv, sizeof(PS), &dsize))
    {
        printf("Khong tim thay nhan khau nay!!\n");
    }
    else
    {
        strcpy(tv->tam_vang.ma_giay_tam_vang, "TV001");
        tv->vang = 1;
        tv->tam_vang.tu_ngay = get_time_now();
        printf("Hay nhap dia chi noi tam tru: ");
        fgets(tv->tam_vang.noi_tam_tru, 100, stdin);
        tv->tam_vang.noi_tam_tru[strlen(tv->tam_vang.noi_tam_tru) - 1] = 0;
        printf("Hay nhap ly do tam vang: ");
        fgets(tv->tam_vang.li_do, 100, stdin);
        tv->tam_vang.li_do[strlen(tv->tam_vang.li_do) - 1] = 0;
        printf("Dang ky tam vang den ngay(dd/mm/yyyy): ");
        scanf("%d/%d/%d", &(tv->tam_vang.den_ngay.tm_mday), &(tv->tam_vang.den_ngay.tm_mon), &(tv->tam_vang.den_ngay.tm_year));
        tv->tam_vang.den_ngay.tm_mon--;
        tv->tam_vang.den_ngay.tm_year -= 1900;
        print_tam_vang(tv);
        btupd(nk, s, (char *)tv, sizeof(PS));
    }
    free(tv);
}
void print_hk()
{
    HHB *hhb;
    PS *ps;
    char s[100], v[3];
    int dsize;
    hhb = (HHB *)malloc(sizeof(HHB));
    ps = (PS *)malloc(sizeof(PS));
    btpos(hk, 1);
    printf("%5s%-8s|%5s%-15s|%1s%-14s|%22s%-33s|\n", "", "ID", "", "TEN CHU HO", "", "SO THANH VIEN", "", "DIA CHI");
    while (!btseln(hk, s, (char *)hhb, sizeof(HHB), &dsize))
    {
        sprintf(v, "%02u", hhb->chu_ho);
        btsel(nk, v, (char *)ps, sizeof(PS), &dsize);
        printf("%3s%-5s%7s%-16s%10s%-5d%8s%-10s\n", "", hhb->so_ho_khau, "", ps->ten, "", hhb->so_nguoi, "", hhb->dia_chi);
    }
    free(ps);
    free(hhb);
}
int co_chu_ho(HHB *hhb, int *hk2, int so_tv)
{
    for (int i = 0; i < so_tv; i++)
    {
        if (hhb->chu_ho == hk2[i])
            return 1;
    }
    return 0;
}
int find(int *hk2, int so_nguoi, unsigned int id)
{
    for (int i = 0; i < so_nguoi; i++)
    {
        if (id == hk2[i])
        {
            return 1;
        }
    }
    return 0;
}
void tach_ra(HHB *hhb, int *hk2, int so_tv)
{
    printf("So nguoi ban dau: %d\n", hhb->so_nguoi);
    HHB *a, *b;
    PS *ps;
    char s[100];
    int tv_con_lai = 0;
    int dsize;
    unsigned int thanh_vien[8];
    for (int i = 0; i < hhb->so_nguoi; i++)
    {
        if (!find(hk2, so_tv, hhb->id_thanh_vien[i]))
        {
            thanh_vien[tv_con_lai++] = hhb->id_thanh_vien[i];
            printf("%u\n", hhb->id_thanh_vien[i]);
        }
    }
    a = (HHB *)malloc(sizeof(HHB));
    ps = (PS *)malloc(sizeof(PS));
    hhb->so_thay_doi++;
    hhb->thay_doi[hhb->so_thay_doi - 1].ngay = get_time_now();
    strcpy(hhb->thay_doi[hhb->so_thay_doi - 1].noi_dung, "Tach ho khau");
    if (co_chu_ho(hhb, hk2, so_tv))
    {
        *a = *hhb;
        hhb->so_nguoi = so_tv;
        memcpy(hhb->id_thanh_vien, hk2, sizeof(unsigned int) * 8);
        a->so_nguoi = tv_con_lai;
        a->chu_ho = 0;
        memcpy(a->id_thanh_vien, thanh_vien, sizeof(unsigned int) * 8);
        printf("Nhap quan he chu ho cho cac thanh vien: \n");
        printf("%5s%-8s|%8s%-11s|%5s%-10s|\n", "", "ID", "", "TEN", "", "QUAN HE");
        for (int i = 0; i < tv_con_lai; i++)
        {
            sprintf(s, "%02u", thanh_vien[i]);
            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
            printf("%3s%05u%8s%-23s", "", thanh_vien[i], "", ps->ten);
            fgets(ps->quan_he_chu_ho, 100, stdin);
            ps->quan_he_chu_ho[strlen(ps->quan_he_chu_ho) - 1] = 0;
            if (!strcmp(ps->quan_he_chu_ho, "Chu ho") || !strcmp(ps->quan_he_chu_ho, "chu ho") || !strcmp(ps->quan_he_chu_ho, "CHU HO"))
            {
                a->chu_ho = ps->id;
            }
            btupd(nk, s, (char *)ps, sizeof(PS));
        }
        if (a->chu_ho == 0)
        {
            printf("Ho khau moi khong co chu ho!!!\n");
            goto here;
        }
    }
    else
    {
        *a = *hhb;
        hhb->so_nguoi = tv_con_lai;
        memcpy(hhb->id_thanh_vien, thanh_vien, sizeof(unsigned int) * 8);
        a->so_nguoi = so_tv;
        a->chu_ho = 0;
        memcpy(a->id_thanh_vien, hk2, sizeof(unsigned int) * 8);
        printf("Nhap quan he chu ho cho cac thanh vien: \n");
        printf("%5s%-8s|%8s%-11s|%5s%-10s|\n", "", "ID", "", "TEN", "", "QUAN HE");
        for (int i = 0; i < so_tv; i++)
        {
            sprintf(s, "%02u", hk2[i]);
            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
            printf("%3s%05u%8s%-23s", "", hk2[i], "", ps->ten);
            fgets(ps->quan_he_chu_ho, 100, stdin);
            ps->quan_he_chu_ho[strlen(ps->quan_he_chu_ho) - 1] = 0;
            if (!strcmp(ps->quan_he_chu_ho, "Chu ho") || !strcmp(ps->quan_he_chu_ho, "chu ho") || !strcmp(ps->quan_he_chu_ho, "CHU HO"))
            {
                a->chu_ho = ps->id;
            }
            btupd(nk, s, (char *)ps, sizeof(PS));
        }
        if (a->chu_ho == 0)
        {
            printf("Ho khau moi khong co chu ho!!!\n");
            goto here;
        }
    }
    btpos(hk, 2);
    b = (HHB *)malloc(sizeof(HHB));
    btselp(hk, s, (char *)b, sizeof(HHB), &dsize);
    s[5]++;
    strcpy(a->so_ho_khau, s);
    btupd(hk, hhb->so_ho_khau, (char *)hhb, sizeof(HHB));
    btins(hk, a->so_ho_khau, (char *)a, sizeof(HHB));
    free(b);
here:
    free(ps);
    free(a);
}
void tach_ho_khau()
{
    char id[9], s[100];
    print_hk();
    printf("Hay nhap ma ho khau cua ho khau ma ban muon tach: ");
    fgets(id, 9, stdin);
    HHB *hhb;
    PS *ps;
    int dsize;
    int a, hk2[8];
    hhb = (HHB *)malloc(sizeof(HHB));
    ps = (PS *)malloc(sizeof(PS));
    id[strlen(id) - 1] = 0;
    if (btsel(hk, id, (char *)hhb, sizeof(HHB), &dsize))
    {
        puts("Khong tim thay ma ho khau nay!!!");
    }
    else
    {
        printf("\tCAC THANH VIEN TRONG SO HO KHAU %s\n", hhb->so_ho_khau);
        printf("%5s%-8s|%8s%-11s|%5s%-10s|\n", "", "ID", "", "TEN", "", "QUAN HE");
        for (int i = 0; i < hhb->so_nguoi; i++)
        {
            sprintf(s, "%02u", hhb->id_thanh_vien[i]);
            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
            printf("%3s%05u%8s%-24s%s\n", "", ps->id, "", ps->ten, ps->quan_he_chu_ho);
        }
        printf("Nhap so nhan khau can tach ra ho khau moi: ");
        scanf("%d", &dsize);
        __fpurge(stdin);
        for (int i = 0; i < dsize; i++)
        {
            printf("Nhap id thanh vien thu %d: ", i + 1);

            scanf("%d", &a);
            if (!find(hhb->id_thanh_vien, hhb->so_nguoi, a))
            {
                i--;
                continue;
                ;
            }
            hk2[i] = a;
            __fpurge(stdin);
        }
        // btdel(hk, hhb->so_ho_khau);
        tach_ra(hhb, hk2, dsize);
    }
    free(ps);
    free(hhb);
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
    // khai_bao_covid();
    // thong_ke_tt_covid();
    // khai_bao_tam_tru();
    // khai_bao_tam_vang();
    tach_ho_khau();
    close_file_bt();
    return 0;
}