#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "btree.h"
#include <stdio_ext.h>
#include <curses.h>
#include <string.h>
#include "data_struct.h"
#include "data_struct.h"
BTA *hk; //Ho khau
BTA *nk; //Nhan khau
WINDOW *w, *titlew, *miw, *f3, *empty, *login, *plist, *tttv;
char list[2][28] = {"Thay doi nhan khau, ho khau", "Quan ly thong tin Covid19"};
char list1[7][27] = {"Them nhan khau",
                     "Thay doi nhan khau",
                     "Thay doi thong tin chu ho",
                     "Tach ho khau",
                     "Tam tru, Tam vang",
                     "Thong ke"};
char list2[2][15] = {"Khai bao y te", "Thong ke"};
char list3[2][15] = {"Chuyen di", "Khai tu"};
char list4[2][15] = {"Tam tru", "Tam Vang"};
char list5[5][20] = {"Thong ke theo tuoi", "Thong ke theo nam", "Thong ke theo nu", "Thong ke tam tru", "Thong ke tam vang"};
char item[30], s[250];
int temp = 0, temp1 = 0, temp2 = 0, temp3 = 0 /* , temp4 = 0, temp5 = 0 */;
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
    // strftime(buffer, 80, "%d/%m/%Y - %I:%M%p", &now);
    // printf("Date & time đã định dạng theo hàm strftime là: \n|%s|\n", buffer);
    return now;
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
int i_d()
{

    char *c;
    char str[100];
    int dsize, i = 1;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        i++;
    }
    free(c);
    return i;
}
void xoa(char s[90], int vitrixoa)
{
    int n = strlen(s);                 //gán n= độ dài của chuỗi
    for (int i = vitrixoa; i < n; i++) //duyệt chuỗi từ vị trí cần xóa
        s[i] = s[i + 1];
    s[n - 1] = '\0'; //kết thúc
}
void chuanHoaTen(char *s)
{
    for (int i = 0; i < (int)strlen(s); i++)
        if (s[i] == ' ' && s[i + 1] == ' ')
        {
            xoa(s, i);
            i--;
        }
    if (s[0] == ' ')
        xoa(s, 0);
    if (s[strlen(s) - 1] == ' ')
        xoa(s, strlen(s) - 1);
}
void initialise_colors()
{
    start_color();
    init_pair(9, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(11, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(12, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(13, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(14, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(15, COLOR_WHITE, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    init_pair(16, COLOR_GREEN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(17, COLOR_MAGENTA, COLOR_BLACK);
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
int find(unsigned int *hk2, int so_nguoi, /* unsigned */ int id)
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
char password(char *s, char *str, int x, int y, WINDOW *xyz)
{
    char ch, pass[50];
    int letter_count = 0;
    for (int i = 0; i < 50; i++)
    {
        s[i] = ' ';
        pass[i] = ' ';
    }
    s[50] = '\0';
    pass[50] = '\0';
    do
    {
        box(xyz, ACS_VLINE, ACS_HLINE); // sets default borders for the window
        ch = wgetch(xyz);
        s[letter_count] = ch;
        pass[letter_count] = '*';
        if (ch == 27)
            return ch;
        if (ch == '-')
        {
            s[letter_count] = ' ';
            pass[letter_count] = ' ';
            (letter_count > 0) ? (letter_count = letter_count - 2) : letter_count;
            s[letter_count + 1] = ' ';
            pass[letter_count + 1] = ' ';
        }
        letter_count++;
        mvwprintw(xyz, x, y + strlen(str), "%s", pass);
    } while ((ch != 10) && (letter_count < 50) && ch != 9);
    if (ch == 10 || ch == 9)
        s[letter_count - 1] = ' ';
    if (strcmp(str, "So Ho Khau: ") != 0)
        chuanHoaTen(s);
    return ch;
}

char nhap(char *s, char *str, int x, int y, WINDOW *xyz)
{
    char ch;
    int letter_count = 0;
    for (int i = 0; i < 50; i++)
        s[i] = ' ';
    s[50] = '\0';
    do
    {
        box(xyz, ACS_VLINE, ACS_HLINE); // sets default borders for the window
        ch = wgetch(xyz);
        s[letter_count] = ch;
        if (ch == 27)
            return ch;
        if (ch == '-')
        {
            s[letter_count] = ' ';
            (letter_count > 0) ? (letter_count = letter_count - 2) : letter_count;
            s[letter_count + 1] = ' ';
        }
        letter_count++;
        mvwprintw(xyz, x, y + strlen(str), "%s", s);
    } while ((ch != 10) && (letter_count < 50) && ch != 9);
    if (ch == 10 || ch == 9)
        s[letter_count - 1] = ' ';
    if (strcmp(str, "So Ho Khau: ") != 0)
        chuanHoaTen(s);
    return ch;
}
void BOX(WINDOW *x) // in ra window menu
{
    box(x, ACS_VLINE, ACS_HLINE); // sets default borders for the window
    mvwprintw(x, 9, 1, "Press <ENTER> to see the option selected");
    mvwprintw(x, 10, 1, "Up and Down arrow keys to naviage (ESC to Exit)");
    titlew = newwin(3, 70, 1, 1);
    wattrset(titlew, COLOR_PAIR(2));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%27sQUAN LY HO KHAU%28s", " ", " ");
    wattroff(titlew, COLOR_PAIR(2));
    wrefresh(titlew);
    wrefresh(x);
}
void BOXlogin() //in ra window function
{
    curs_set(1);
    login = newwin(12, 70, 4, 1);
    box(login, ACS_VLINE, ACS_HLINE);
    box(derwin(login, 3, 50, 1, 1), ACS_VLINE, ACS_HLINE);
    box(derwin(login, 3, 50, 4, 1), ACS_VLINE, ACS_HLINE);

    wattron(login, A_STANDOUT);
    wattrset(login, COLOR_PAIR(14));
    mvwprintw(login, 2, 2, "%s", "Tai Khoan:");
    mvwprintw(login, 5, 2, "%s", "Mat Khau:");
    wattroff(login, COLOR_PAIR(14));
    titlew = newwin(3, 70, 1, 1);
    wattrset(titlew, COLOR_PAIR(2));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%27sQUAN LY HO KHAU%28s", " ", " ");
    wattroff(titlew, COLOR_PAIR(2));
    wrefresh(titlew);
    wrefresh(login);
}
void BOXE(int a) //in ra window function
{
    empty = newwin(20, 70, 1, 1);
    box(empty, ACS_VLINE, ACS_HLINE);
    wclear(empty);
    wrefresh(empty);
    wattron(empty, A_STANDOUT);
    if (a == 2)
        sprintf(item, "%-23s", list2[temp2]);
    else if (a == 1)
        sprintf(item, "%-28s", list1[temp1]);
    else if (a == 5)
    {
        sprintf(item, "%-28s", list5[temp3]);
    }
    mvwprintw(empty, 1, 2, "%s", item);
    wattroff(empty, A_STANDOUT);
    wrefresh(empty);
}
COVID covid()
{
    char str1[250], str2[250];
    int line = 7;
    COVID khai_bao;

    khai_bao.thoi_gian = get_time_now();
    khai_bao.so_ngay = 21;

    mvwprintw(empty, 4, 2, "Phan loai F (0: F0, 1: F1, 2: F2...): ");
    nhap(str1, "Phan loai F (0: F0, 1: F1, 2: F2...): ", 4, 2, empty);
    khai_bao.muc_do = atoi(str1);
    mvwprintw(empty, 5, 2, "Da Test Covid chua?(1: roi, 0: chua):");
    nhap(str1, "Da Test Covid chua?(1: roi, 0: chua):", 5, 2, empty);
    khai_bao.test = atoi(str1);
    if (atoi(str1))
    {
        mvwprintw(empty, 6, 2, "Da test bao nhieu lan: ");
        nhap(str1, "Da test bao nhieu lan:  ", 6, 2, empty);
        khai_bao.so_lan_test = atoi(str1);
        for (int i = 0; i < atoi(str1); i++)
        {
            mvwprintw(empty, line, 2, "Ket qua lan thu %d(1: (+), 0: (-)", i + 1);
            nhap(str2, "Ket qua lan thu %d(1: (+), 0: (-)", line, 2, empty);
            line++;
            khai_bao.ket_qua[i] = atoi(str2);
        }
        if (atoi(str1))
        {
            mvwprintw(empty, line, 2, "Ngay test gan nhat(dd/mm/yyyy): ");

            mvwprintw(empty, line + 1, 2, "Ngay: ");
            nhap(str2, "Ngay: ", line + 1, 2, empty);
            khai_bao.ngay_test.tm_mday = atoi(str2);

            mvwprintw(empty, line + 1, 12, "Thang: ");
            nhap(str2, "Thang: ", line + 1, 12, empty);
            khai_bao.ngay_test.tm_mon = atoi(str2);

            mvwprintw(empty, line + 1, 22, "Nam: ");
            nhap(str2, "Nam: ", line + 1, 22, empty);
            khai_bao.ngay_test.tm_year = atoi(str2);

            khai_bao.ngay_test.tm_mon--;
            khai_bao.ngay_test.tm_year -= 1900;
        }
    }
    strcpy(khai_bao.tinh_trang_sk, "On dinh");
    return khai_bao;
}
void print_list()
{
    int a = i_d();
    plist = newwin(5 + a, 120, 1, 71);
    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    int line = 2;
    mvwprintw(plist, 1, 1, " %-5s%9s%-25s%-9s%20s%-35s", "ID", "", "TEN", "HO KHAU", "", "DIA CHI");

    PS nhan_khau;
    char *c;
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (!nhan_khau.chet && !nhan_khau.vang)
        {
            line++;
            mvwprintw(plist, line, 2, "%05u%5s%-30s%-14s%s", nhan_khau.id, "", nhan_khau.ten, nhan_khau.so_ho_khau, nhan_khau.nguyen_quan);
        }
    }
    free(c);

    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    wrefresh(plist);
}
int print_hk()
{
    plist = newwin(45, 100, 1, 71);
    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    HHB *hhb;
    PS *ps;
    char s[100], v[3];
    int dsize, line = 2;
    hhb = (HHB *)malloc(sizeof(HHB));
    ps = (PS *)malloc(sizeof(PS));
    btpos(hk, 1);
    mvwprintw(plist, 1, 1, "%5s%-8s%5s%-15s%1s%-14s%22s%-33s", "", "ID", "", "TEN CHU HO", "", "SO THANH VIEN", "", "DIA CHI");
    while (!btseln(hk, s, (char *)hhb, sizeof(HHB), &dsize))
    {
        line++;
        sprintf(v, "%02u", hhb->chu_ho);
        btsel(nk, v, (char *)ps, sizeof(PS), &dsize);
        mvwprintw(plist, line, 2, "%3s%-5s%7s%-16s%10s%-5d%8s%-10s", "", hhb->so_ho_khau, "", ps->ten, "", hhb->so_nguoi, "", hhb->dia_chi);
    }
    free(ps);
    free(hhb);

    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    wrefresh(plist);
    return line;
}
void tach_ra(HHB *hhb, unsigned int *hk2, int so_tv)
{
    BOXE(1);
    mvwprintw(empty, 3, 2, "So nguoi ban dau: %d", hhb->so_nguoi);
    HHB *a, *b;
    PS *ps;
    char s[100], str[250];
    int tv_con_lai = 0;
    int ch;
    int dsize;
    unsigned int thanh_vien[15];
    for (int i = 0; i < hhb->so_nguoi; i++)
    {
        if (!find(hk2, so_tv, hhb->id_thanh_vien[i]))
        {
            thanh_vien[tv_con_lai++] = hhb->id_thanh_vien[i];
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
        memcpy(hhb->id_thanh_vien, hk2, sizeof(unsigned int) * 15);
        a->so_nguoi = tv_con_lai;
        a->chu_ho = 0;
        memcpy(a->id_thanh_vien, thanh_vien, sizeof(unsigned int) * 15);
        mvwprintw(empty, 5, 2, "Nhap quan he chu ho cho cac thanh vien %d:", tv_con_lai);
        mvwprintw(empty, 6, 2, "%5s%-8s%8s%-11s%5s%-10s", "", "ID", "", "TEN", "", "QUAN HE");

        for (int i = 0; i < tv_con_lai; i++)
        {
            sprintf(s, "%02u", thanh_vien[i]);
            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
            strcpy(str, ps->ten);
            mvwprintw(empty, 7 + i, 2, "%3s%05u%8s%-23s", "", thanh_vien[i], "", ps->ten);
            ch = nhap(ps->quan_he_chu_ho, "", 7 + i, 40, empty);
            if (ch == 27)
                break;

            if (!strcmp(ps->quan_he_chu_ho, "Chu ho") || !strcmp(ps->quan_he_chu_ho, "chu ho") || !strcmp(ps->quan_he_chu_ho, "CHU HO"))
            {
                a->chu_ho = ps->id;
            }
            strcpy(ps->ten, str);
            btupd(nk, s, (char *)ps, sizeof(PS));
        }

        if (a->chu_ho == 0)
        {
            goto here;
        }
    }
    else
    {
        *a = *hhb;
        hhb->so_nguoi = tv_con_lai;
        memcpy(hhb->id_thanh_vien, thanh_vien, sizeof(unsigned int) * 15);
        a->so_nguoi = so_tv;
        a->chu_ho = 0;
        memcpy(a->id_thanh_vien, hk2, sizeof(unsigned int) * 15);
        mvwprintw(empty, 5, 2, "Nhap quan he chu ho cho cac thanh vien: \n");
        mvwprintw(empty, 6, 2, "%5s%-8s|%8s%-11s|%5s%-10s|\n", "", "ID", "", "TEN", "", "QUAN HE");
        for (int i = 0; i < so_tv; i++)
        {
            sprintf(s, "%02u", hk2[i]);
            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
            strcpy(str, ps->ten);
            mvwprintw(empty, 7 + i, 2, "%3s%05u%8s%-23s", "", hk2[i], "", ps->ten);
            ch = nhap(ps->quan_he_chu_ho, "", 7 + i, 40, empty);
            if (ch == 27)
                break;
            if (!strcmp(ps->quan_he_chu_ho, "Chu ho") || !strcmp(ps->quan_he_chu_ho, "chu ho") || !strcmp(ps->quan_he_chu_ho, "CHU HO"))
            {
                a->chu_ho = ps->id;
            }
            strcpy(ps->ten, str);
            btupd(nk, s, (char *)ps, sizeof(PS));
        }
        if (a->chu_ho == 0)
        {
            // mwprintf("Ho khau moi khong co chu ho!!!\n");
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
    // printf("test\n");
    free(ps);
    free(a);
}
void thongkegioitinh(int a)
{

    tttv = newwin(45, 98, 4, 1);
    box(tttv, ACS_VLINE, ACS_HLINE);
    int line = 2;
    mvwprintw(tttv, 1, 1, " %-5s%9s%-20s%-9s%20s%-35s\n", "ID", "", "TEN", "HO KHAU", "", "DIA CHI");
    PS nhan_khau;
    char *c;
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (!nhan_khau.chet && nhan_khau.gioi_tinh == a)
        {
            line++;
            mvwprintw(tttv, line, 2, "%05u%5s%-25s%-14s%s\n", nhan_khau.id, "", nhan_khau.ten, nhan_khau.so_ho_khau, nhan_khau.nguyen_quan);
        }
    }
    free(c);

    titlew = newwin(3, 98, 1, 1);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%35sBANG THONG KE THEO GIOI TINh%35s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void thongketheotuoi(int tuoi)
{
    tttv = newwin(45, 98, 4, 1);
    box(tttv, ACS_VLINE, ACS_HLINE);
    int line = 2;
    mvwprintw(tttv, 1, 1, " %-5s%9s%-20s%-9s%20s%-35s\n", "ID", "", "TEN", "HO KHAU", "", "DIA CHI");
    PS nhan_khau;
    char *c;
    char str[100];
    int dsize;
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (!nhan_khau.chet && timeinfo->tm_year - nhan_khau.ngay_sinh.tm_year == tuoi)
        {
            line++;
            mvwprintw(tttv, line, 2, "%05u%5s%-25s%-14s%s\n", nhan_khau.id, "", nhan_khau.ten, nhan_khau.so_ho_khau, nhan_khau.nguyen_quan);
        }
    }
    free(c);

    titlew = newwin(3, 98, 1, 1);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%35sBANG THONG KE THEO DO TUOI%35s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void thongketamtru()
{
    tttv = newwin(45, 98, 4, 1);
    box(tttv, ACS_VLINE, ACS_HLINE);
    int line = 2;
    mvwprintw(tttv, 1, 1, " %-5s%9s%-20s%-9s%20s%-35s\n", "ID", "", "TEN", "SO HO KHAU", "", "TAM TRU TU NGAY");
    PS nhan_khau;
    char *c, buffer[20];
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (nhan_khau.chet != '1' && nhan_khau.tru)
        {
            line++;
            strftime(buffer, 15, "%d/%m/%Y", &nhan_khau.tam_tru.tu_ngay);
            mvwprintw(tttv, line, 2, "%05u%5s%-25s%-14s%s\n", nhan_khau.id, "", nhan_khau.ten, nhan_khau.so_ho_khau, buffer);
        }
    }
    free(c);
    titlew = newwin(3, 98, 1, 1);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%35s BANG THONG KE THEO TAM TRU %35s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void thongketamvang()
{

    tttv = newwin(45, 98, 4, 1);
    box(tttv, ACS_VLINE, ACS_HLINE);
    int line = 2;
    mvwprintw(tttv, 1, 1, " %-5s%9s%-20s%-9s%20s%-35s\n", "ID", "", "TEN", "SO HO KHAU", "", "TAM VANG TU NGAY");
    PS nhan_khau;
    char *c, buffer[20];
    char str[100];
    int dsize;
    btpos(nk, 1);
    c = (char *)malloc(sizeof(PS));
    while (!btseln(nk, str, c, sizeof(PS), &dsize))
    {
        nhan_khau = *(PS *)c;
        if (!nhan_khau.chet && nhan_khau.vang)
        {
            line++;
            strftime(buffer, 15, "%d/%m/%Y", &nhan_khau.tam_vang.tu_ngay);
            mvwprintw(tttv, line, 2, "%05u%5s%-25s%-14s%s\n", nhan_khau.id, "", nhan_khau.ten, nhan_khau.so_ho_khau, buffer);
        }
    }
    free(c);
    titlew = newwin(3, 98, 1, 1);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%35sBANG THONG KE THEO TAM VANG%35s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void print_tam_tru(PS *ps)
{
    char buf1[100], buf2[100];
    strftime(buf1, 80, "%d/%m/%Y", &(ps->tam_tru.tu_ngay));
    strftime(buf2, 80, "%d/%m/%Y", &(ps->tam_tru.den_ngay));

    tttv = newwin(17, 70, 4, 71);
    box(tttv, ACS_VLINE, ACS_HLINE);
    wattron(tttv, A_STANDOUT);
    wattrset(tttv, COLOR_PAIR(14));
    mvwprintw(tttv, 2, 2, "%-30s%-30s", "Ho va ten: ", "Ma giay tam tru: ");
    mvwprintw(tttv, 3, 2, "%-32s%-30s", "Tu Ngay: ", "Den Ngay: ");
    mvwprintw(tttv, 4, 2, "%-s", "Dia chi thuong tru truoc khi chuyen den: ");
    mvwprintw(tttv, 6, 2, "%-s", "Ly do tam tru:");
    // mvwprintw(tttv, 6, 2, "%-s", "So dien thoai nguoi dang ky: ");
    wattroff(tttv, COLOR_PAIR(14));

    mvwprintw(tttv, 2, 2 + strlen("ho va ten: "), "%-s", ps->ten);
    mvwprintw(tttv, 2, 32 + strlen("Ma giay tam tru: "), "%-s", ps->tam_tru.ma_giay_tam_tru);

    mvwprintw(tttv, 3, 2 + strlen("Tu Ngay: "), "%s", buf1);
    mvwprintw(tttv, 3, 34 + strlen("Den Ngay: "), "%s", buf2);

    mvwprintw(tttv, 5, 2, "%s", ps->dia_chi_thuong_tru_tkcd);
    mvwprintw(tttv, 7, 2, "%s", ps->tam_tru.li_do);
    // mvwprintw(tttv, 6, 2 + strlen("So dien thoai nguoi dang ky: "), "%-s", ps->tam_tru.so_dien_thoai_nguoi_dk);

    titlew = newwin(3, 70, 1, 71);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%29sGIAY TAM TRU%28s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void print_tam_vang(PS *ps)
{
    char buf1[100], buf2[100];
    strftime(buf1, 80, "%d/%m/%Y", &(ps->tam_vang.tu_ngay));
    strftime(buf2, 80, "%d/%m/%Y", &(ps->tam_vang.den_ngay));

    tttv = newwin(17, 70, 4, 71);
    box(tttv, ACS_VLINE, ACS_HLINE);
    wattron(tttv, A_STANDOUT);
    wattrset(tttv, COLOR_PAIR(14));
    mvwprintw(tttv, 2, 2, "%-30s%-30s", "Ho va ten: ", "  Ma giay tam vang: ");
    mvwprintw(tttv, 3, 2, "%-32s%-30s", "Tu Ngay: ", "Den Ngay: ");
    mvwprintw(tttv, 4, 2, "%-s", "Dia chi dang ky tam tru:");
    mvwprintw(tttv, 5, 2, "%-s", "Ly do tam vang: ");
    // mvwprintw(tttv, 6, 2, "%-s", "So dien thoai nguoi dang ky: ");
    wattroff(tttv, COLOR_PAIR(14));

    mvwprintw(tttv, 2, 2 + strlen("ho va ten: "), "%-s", ps->ten);
    mvwprintw(tttv, 2, 32 + strlen("Ma giay tam tru: "), "%-s", ps->tam_vang.ma_giay_tam_vang);

    mvwprintw(tttv, 3, 2 + strlen("Tu Ngay: "), "%s", buf1);
    mvwprintw(tttv, 3, 34 + strlen("Den Ngay: "), "%s", buf2);

    mvwprintw(tttv, 4, 2 + strlen("Dia chi dang ky tam tru: "), "%s", ps->tam_vang.noi_tam_tru);
    mvwprintw(tttv, 5, 2 + strlen("Ly do tam vang: "), "%s", ps->tam_vang.li_do);
    // mvwprintw(tttv, 6, 2 + strlen("So dien thoai nguoi dang ky: "), "%-s", ps->tam_tru.so_dien_thoai_nguoi_dk);

    titlew = newwin(3, 70, 1, 71);
    wattrset(titlew, COLOR_PAIR(6));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%27sGIAY TAM VANG%28s", " ", " ");
    wattroff(titlew, COLOR_PAIR(6));
    box(tttv, ACS_VLINE, ACS_HLINE);

    wrefresh(titlew);
    wrefresh(tttv);
}
void thayDoiNhanKhau()
{
    char s[10];
    PS ps, *c;
    HHB hhb, *b;
    int dsize;
    char so_ho_khau[9];

    int ch;
    char str1[250];
    f3 = newwin(12, 70, 4, 1); // create a new window

    BOX(f3);
    // now print all the menu items and highlight the first one
    for (temp3 = 0; temp3 < 2; temp3++)
    {
        if (temp3 == 0)
            wattron(f3, A_STANDOUT); // highlights the first item.
        else
            wattroff(f3, A_STANDOUT);
        sprintf(item, "%-10s", list3[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
    }
    wrefresh(f3); // update the terminal screen
    temp3 = 0;
    noecho();         // disable echoing of characters on the screen
    keypad(f3, TRUE); // enable keyboard input for the window.
    curs_set(0);      // hide the default screen cursor.
    while ((ch = wgetch(f3)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-10s", list3[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp3--;
            temp3 = (temp3 < 0) ? 1 : temp3;
            break;
        case KEY_DOWN:
            temp3++;
            temp3 = (temp3 > 1) ? 0 : temp3;
            break;
        case 10:
            if (temp3 == 0) //function 1:
            {
                do
                {
                    BOXE(1);
                    print_list();
                    mvwprintw(empty, 3, 2, "ID nguoi muon chuyen di: ");
                    ch = nhap(str1, "ID nguoi muon chuyen di: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    sprintf(s, "%02d", atoi(str1));
                    box(empty, ACS_VLINE, ACS_HLINE);
                    c = (PS *)malloc(sizeof(PS));
                    if (btsel(nk, s, (char *)c, sizeof(PS), &dsize))
                    {
                        mvwprintw(empty, 5, 2, "Khong tim thay nhan khau nay!!");
                    }
                    else
                    {
                        mvwprintw(empty, 4, 2, "Dia chi chuyen den: ");
                        ch = nhap(ps.thong_tin_cd.noi_chuyen, "Dia chi chuyen den: ", 4, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 6, 2, "Ngay chuyen(dd/mm/yyyy): ");
                        mvwprintw(empty, 7, 2, "Ngay: ");
                        nhap(str1, "Ngay: ", 7, 2, empty);
                        ps.thong_tin_cd.ngay_chuyen.tm_mday = atoi(str1);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 7, 12, "Thang: ");
                        nhap(str1, "Thang: ", 7, 12, empty);
                        ps.thong_tin_cd.ngay_chuyen.tm_mon = atoi(str1) - 1;
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 7, 22, "Nam: ");
                        nhap(str1, "Nam: ", 7, 22, empty);
                        ps.thong_tin_cd.ngay_chuyen.tm_year = atoi(str1) - 1900;
                        if (ch == 27)
                            break;
                        *c = ps;
                        btupd(nk, s, (char *)c, sizeof(PS));
                        free(c);
                        print_list();
                        wrefresh(plist);
                        mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                        ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                        if (ch == 27)
                            break;
                        box(empty, ACS_VLINE, ACS_HLINE);
                    }
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp3 == 1) // function 2:
            {
                do
                {
                    BOXE(1);
                    print_list();
                    mvwprintw(empty, 3, 2, "ID nguoi mat: ");
                    ch = nhap(str1, "ID nguoi mat: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    sprintf(s, "%02d", atoi(str1));
                    box(empty, ACS_VLINE, ACS_HLINE);
                    c = (PS *)malloc(sizeof(PS));
                    if (btsel(nk, s, (char *)c, sizeof(PS), &dsize))
                    {
                        mvwprintw(empty, 5, 2, "Khong tim thay nhan khau nay!!\nNhan Esc de thoat");
                    }
                    else
                    {
                        mvwprintw(empty, 5, 2, "Ngay mat(dd/mm/yyyy): ");
                        mvwprintw(empty, 6, 2, "Ngay: ");
                        nhap(str1, "Ngay: ", 6, 2, empty);
                        ps.khai_tu.ngay_chet.tm_mday = atoi(str1);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 6, 12, "Thang: ");
                        nhap(str1, "Thang: ", 6, 12, empty);
                        ps.khai_tu.ngay_chet.tm_mon = atoi(str1) - 1;
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 6, 22, "Nam: ");
                        nhap(str1, "Nam: ", 6, 22, empty);
                        ps.khai_tu.ngay_chet.tm_year = atoi(str1) - 1900;
                        if (ch == 27)
                            break;
                        ps.khai_tu.ngay_khai = get_time_now();
                        if (ch == 27)
                            break;

                        *c = ps;
                        btupd(nk, s, (char *)c, sizeof(PS));
                        free(c);
                        wclear(plist);
                        wrefresh(plist);
                        print_list();

                        b = (HHB *)malloc(sizeof(HHB));
                        if (btsel(hk, so_ho_khau, (char *)b, sizeof(HHB), &dsize))
                        {
                            mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                            ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                            if (ch == 27)
                                break;
                        }

                        else
                        {
                            hhb = *b;
                            hhb.so_nguoi = hhb.so_nguoi - 1;

                            *b = hhb;
                            btupd(hk, s, (char *)b, sizeof(HHB));
                            mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                            ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                            if (ch == 27)
                                break;
                        }

                        free(c);

                        wclear(plist);
                        wrefresh(plist);
                        box(empty, ACS_VLINE, ACS_HLINE);
                    }
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(empty);
                wrefresh(empty);
            }
            break;
        }
        BOX(f3);
        wattron(f3, A_STANDOUT);
        sprintf(item, "%-10s", list3[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        wattroff(f3, A_STANDOUT);
    }
    delwin(f3);
}
void tamtrutamvang()
{
    PS *tt;
    char str[100], str1[250];
    char id[4];
    int max = 0, dsize;
    tt = (PS *)malloc(sizeof(PS));
    PS *tv;
    char s[100];
    tv = (PS *)malloc(sizeof(PS));
    int line = 6;
    // char s[10];
    int ch;

    f3 = newwin(12, 70, 4, 1); // create a new window
    BOX(f3);
    // now print all the menu items and highlight the first one
    for (temp3 = 0; temp3 < 2; temp3++)
    {
        if (temp3 == 0)
            wattron(f3, A_STANDOUT); // highlights the first item.
        else
            wattroff(f3, A_STANDOUT);
        sprintf(item, "%-10s", list4[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
    }
    wrefresh(f3); // update the terminal screen
    temp3 = 0;
    noecho();         // disable echoing of characters on the screen
    keypad(f3, TRUE); // enable keyboard input for the window.
    curs_set(0);      // hide the default screen cursor.
    while ((ch = wgetch(f3)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-10s", list4[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp3--;
            temp3 = (temp3 < 0) ? 1 : temp3;
            break;
        case KEY_DOWN:
            temp3++;
            temp3 = (temp3 > 1) ? 0 : temp3;
            break;
        case 10:
            if (temp3 == 0) //function 1:
            {
                do
                {
                    BOXE(1);
                    // print_list();
                    mvwprintw(empty, 3, 2, "Ten Nguoi muon tam tru: ");
                    ch = nhap(tt->ten, "Ten Nguoi muon tam tru: ", 3, 2, empty);
                    box(empty, ACS_VLINE, ACS_HLINE);
                    if (ch == 27)
                        break;
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
                    mvwprintw(empty, 4, 2, "Nhap dia chi thuong tru: ");
                    ch = nhap(tt->dia_chi_thuong_tru_tkcd, "Nhap dia chi thuong tru: ", 4, 2, empty);
                    if (ch == 27)
                        break;
                    mvwprintw(empty, 5, 2, "Nhap ly do tam tru: ");
                    ch = nhap(tt->tam_tru.li_do, "Nhap ly do tam tru: ", 5, 2, empty);
                    if (ch == 27)
                        break;
                    mvwprintw(empty, 6, 2, "Gioi tinh( 1: Nam, 0: Nu ) ");
                    ch = nhap(str1, "Gioi tinh( 1: Nam, 0: Nu ) ", 6, 2, empty);
                    tt->gioi_tinh = atoi(str1);
                    mvwprintw(empty, 7, 2, "Dang ky tam tru den ngay(dd/mm/yyyy): ");
                    mvwprintw(empty, 8, 2, "Ngay: ");
                    nhap(str1, "Ngay: ", 8, 2, empty);
                    tt->tam_tru.den_ngay.tm_mday = atoi(str1);
                    if (ch == 27)
                        break;
                    mvwprintw(empty, 8, 12, "Thang: ");
                    nhap(str1, "Thang: ", 8, 12, empty);
                    tt->tam_tru.den_ngay.tm_mon = atoi(str1);
                    if (ch == 27)
                        break;
                    mvwprintw(empty, 8, 22, "Nam: ");
                    nhap(str1, "Nam: ", 8, 22, empty);
                    tt->tam_tru.den_ngay.tm_year = atoi(str1);
                    if (ch == 27)
                        break;
                    tt->tam_tru.den_ngay.tm_mon--;
                    tt->tam_tru.den_ngay.tm_year -= 1900;

                    btins(nk, id, (char *)tt, sizeof(PS));
                    print_tam_tru(tt);

                    mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                    if (ch == 27)
                        break;
                    free(tt);
                    box(empty, ACS_VLINE, ACS_HLINE);
                } while (wgetch(empty) != 27);
                wclear(tttv);
                wrefresh(tttv);
                wclear(titlew);
                wrefresh(titlew);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp3 == 1) // function 2:
            {
                do
                {
                    BOXE(1);
                    print_list();
                    mvwprintw(empty, 3, 2, "ID nguoi muon tam vang: ");
                    ch = nhap(str1, "ID nguoi muon tam vang: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    sprintf(s, "%02d", atoi(str1));
                    box(empty, ACS_VLINE, ACS_HLINE);
                    if (btsel(nk, s, (char *)tv, sizeof(PS), &dsize))
                    {
                        mvwprintw(empty, 5, 2, "Khong tim thay nhan khau nay!!");
                    }
                    else
                    {
                        strcpy(tv->tam_vang.ma_giay_tam_vang, "TV001");
                        tv->vang = 1;
                        tv->tam_vang.tu_ngay = get_time_now();
                        mvwprintw(empty, 4, 2, "Nhap dia chi tam tru: ");
                        ch = nhap(tv->tam_vang.noi_tam_tru, "Nhap dia chi tam tru: ", 4, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 5, 2, "Nhap ly do tam vang: ");
                        ch = nhap(tv->tam_vang.li_do, "Nhap ly do tam tru: ", 5, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 6, 2, "Dang ky tam vang den ngay(dd/mm/yyyy): ");
                        mvwprintw(empty, line + 1, 2, "Ngay: ");
                        nhap(str1, "Ngay: ", line + 1, 2, empty);
                        tv->tam_vang.den_ngay.tm_mday = atoi(str1);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, line + 2, 2, "Thang: ");
                        nhap(str1, "Thang: ", line + 2, 2, empty);
                        tv->tam_vang.den_ngay.tm_mon = atoi(str1);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, line + 3, 2, "Nam: ");
                        nhap(str1, "Nam: ", line + 3, 2, empty);
                        tv->tam_vang.den_ngay.tm_year = atoi(str1);
                        if (ch == 27)
                            break;
                        tv->tam_vang.den_ngay.tm_mon--;
                        tv->tam_vang.den_ngay.tm_year -= 1900;
                        // print_tam_vang(tv);
                        btupd(nk, s, (char *)tv, sizeof(PS));
                        wclear(plist);
                        wrefresh(plist);
                        print_tam_vang(tv);
                        mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                        ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                        if (ch == 27)
                            break;
                        free(tv);
                        box(empty, ACS_VLINE, ACS_HLINE);
                    }
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(tttv);
                wrefresh(tttv);
                wclear(titlew);
                wrefresh(titlew);
                wclear(empty);
                wrefresh(empty);
            }
            break;
        }
        BOX(f3);
        wattron(f3, A_STANDOUT);
        sprintf(item, "%-10s", list4[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        wattroff(f3, A_STANDOUT);
    }
    delwin(f3);
}
void thongke()
{
    int ch;
    char str[250];
    f3 = newwin(12, 70, 4, 1); // create a new window
    BOX(f3);
    // now print all the menu items and highlight the first one
    for (temp3 = 0; temp3 < 5; temp3++)
    {
        if (temp3 == 0)
            wattron(f3, A_STANDOUT); // highlights the first item.
        else
            wattroff(f3, A_STANDOUT);
        sprintf(item, "%-20s", list5[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
    }
    wrefresh(f3); // update the terminal screen
    temp3 = 0;
    noecho();         // disable echoing of characters on the screen
    keypad(f3, TRUE); // enable keyboard input for the window.
    curs_set(0);      // hide the default screen cursor.
    while ((ch = wgetch(f3)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-20s", list5[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp3--;
            temp3 = (temp3 < 0) ? 4 : temp3;
            break;
        case KEY_DOWN:
            temp3++;
            temp3 = (temp3 > 4) ? 0 : temp3;
            break;
        case 10:
            if (temp3 == 0) //function 1:
            {
                do
                {
                    BOXE(1);
                    mvwprintw(empty, 3, 2, "Do tuoi can thong ke: ");
                    ch = nhap(str, "Do tuoi can thong ke: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    thongketheotuoi(atoi(str));

                    mvwprintw(tttv, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str, "Nhan ESC de thoat  ", 39, 2, tttv);
                    if (ch == 27)
                        break;
                    box(tttv, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(tttv);

                } while (wgetch(tttv) != 27);

                wclear(titlew);
                wrefresh(titlew);
                wclear(tttv);
                wrefresh(tttv);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp3 == 1) // function 2:
            {
                do
                {
                    thongkegioitinh(1);
                    mvwprintw(tttv, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str, "Nhan ESC de thoat  ", 39, 2, tttv);
                    if (ch == 27)
                        break;
                    box(tttv, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(tttv);

                } while (wgetch(tttv) != 27);

                wclear(titlew);
                wrefresh(titlew);
                wclear(tttv);
                wrefresh(tttv);
            }

            if (temp3 == 2)
            {
                do
                {
                    thongkegioitinh(0);
                    mvwprintw(tttv, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str, "Nhan ESC de thoat  ", 39, 2, tttv);
                    if (ch == 27)
                        break;
                    box(tttv, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(tttv);

                } while (wgetch(tttv) != 27);

                wclear(titlew);
                wrefresh(titlew);
                wclear(tttv);
                wrefresh(tttv);
            }
            if (temp3 == 3)
            {
                do
                {
                    thongketamtru();
                    mvwprintw(tttv, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str, "Nhan ESC de thoat  ", 39, 2, tttv);
                    if (ch == 27)
                        break;
                    box(tttv, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(tttv);

                } while (wgetch(tttv) != 27);

                wclear(titlew);
                wrefresh(titlew);
                wclear(tttv);
                wrefresh(tttv);
            }
            if (temp3 == 4)
            {
                do
                {
                    thongketamvang();
                    mvwprintw(tttv, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str, "Nhan ESC de thoat  ", 39, 2, tttv);
                    if (ch == 27)
                        break;
                    box(tttv, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(tttv);

                } while (wgetch(tttv) != 27);

                wclear(titlew);
                wrefresh(titlew);
                wclear(tttv);
                wrefresh(tttv);
            }
            break;
        }
        BOX(f3);
        wattron(f3, A_STANDOUT);
        sprintf(item, "%-20s", list5[temp3]);
        mvwprintw(f3, temp3 + 1, 2, "%s", item);
        wattroff(f3, A_STANDOUT);
    }
    delwin(f3);
}
void Ham1()
{
    char str1[250], str2[250], shk[10], ds[250];
    int dsize;
    HHB hhb, *b;
    PS ps1, *c;
    char s[100];
    int songuoi;
    HHB *hhb1, hhb2;
    int a;
    unsigned int hk2[15];
    hhb1 = (HHB *)malloc(sizeof(HHB));

    // char s[10];
    PS *ps;
    ps = (PS *)malloc(sizeof(PS));

    char ID[10];

    int line = 6;
    int ch;
    miw = newwin(12, 70, 4, 1); // create a new window
    BOX(miw);
    // now print all the menu items and highlight the first one
    for (temp1 = 0; temp1 < 6; temp1++)
    {
        if (temp1 == 0)
            wattron(miw, A_STANDOUT); // highlights the first item.
        else
            wattroff(miw, A_STANDOUT);
        sprintf(item, "%-25s", list1[temp1]);
        mvwprintw(miw, temp1 + 1, 2, "%s", item);
    }
    wrefresh(miw); // update the terminal screen
    temp1 = 0;
    noecho();          // disable echoing of characters on the screen
    keypad(miw, TRUE); // enable keyboard input for the window.
    curs_set(0);       // hide the default screen cursor.
    while ((ch = wgetch(miw)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-25s", list1[temp1]);
        mvwprintw(miw, temp1 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp1--;
            temp1 = (temp1 < 0) ? 5 : temp1;
            break;
        case KEY_DOWN:
            temp1++;
            temp1 = (temp1 > 5) ? 0 : temp1;
            break;
        case 10:
            if (temp1 == 0) //function 1:
            {               //themnhankhau

                do
                {
                    BOXE(1);
                    print_list();

                    mvwprintw(empty, 3, 2, "So Ho Khau: ");
                    ch = nhap(ps->so_ho_khau, "So Ho Khau: ", 3, 2, empty);
                    if (ch == 27)
                        break;
                    ps->so_ho_khau[6] = 0;

                    b = (HHB *)malloc(sizeof(HHB));
                    if (btsel(hk, ps->so_ho_khau, (char *)b, sizeof(HHB), &dsize))
                    {
                        mvwprintw(empty, 15, 2, "Khong tim thay so ho khau nay!! %s\n   Nhan ESC de thoat  ", shk);
                        ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                        break;
                    }
                    else
                    {
                        hhb = *b;
                        ps->id = i_d();
                        set(ps);
                        box(empty, ACS_VLINE, ACS_HLINE);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 5, 2, "Nhap Ten: ");
                        ch = nhap(str2, "Nhap ten: ", 5, 2, empty);

                        if (ch == 27)
                            break;
                        mvwprintw(empty, 6, 2, "Gioi tinh( 1: Nam, 0: Nu ) ");
                        ch = nhap(str1, "Gioi tinh( 1: Nam, 0: Nu ) ", 6, 2, empty);
                        ps->gioi_tinh = atoi(str1);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 7, 2, "Ngay Sinh:(dd/mm/yyyy): ");
                        mvwprintw(empty, 8, 2, "Ngay: ");
                        ch = nhap(str1, "Ngay: ", 8, 2, empty);
                        if (ch == 27)
                            break;
                        ps->ngay_sinh.tm_mday = atoi(str1);

                        mvwprintw(empty, 8, 12, "Thang: ");
                        ch = nhap(str1, "Thang: ", 8, 12, empty);
                        ps->ngay_sinh.tm_mon = atoi(str1) - 1;
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 8, 22, "Nam: ");
                        ch = nhap(str1, "Nam: ", 8, 22, empty);
                        ps->ngay_sinh.tm_year = atoi(str1) - 1900;
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 9, 2, "Dan toc: ");
                        ch = nhap(ps->dan_toc, "Dan toc: ", 9, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 10, 2, "Quoc tinh: ");
                        ch = nhap(ps->quoc_tich, "Quoc tinh: ", 10, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 11, 2, "Nghe nghiep: ");
                        ch = nhap(ps->nghe_nghiep, "Nghe nghiep: ", 11, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 12, 2, "Chung minh nhan dan: ");
                        ch = nhap(ps->CMND, "Chung minh nhan dan: ", 12, 2, empty);
                        if (ch == 27)
                            break;
                        mvwprintw(empty, 13, 2, "Quan he voi chu ho: ");
                        ch = nhap(ps->quan_he_chu_ho, "Quan he voi chu ho: ", 13, 2, empty);
                        if (ch == 27)
                            break;
                        strcpy(ps->nguyen_quan, hhb.dia_chi);
                        strcpy(ps->ten, str2);
                        sprintf(ID, "%02d", ps->id);
                        btins(nk, ID, (char *)ps, sizeof(PS));

                        hhb.so_nguoi++;
                        hhb.id_thanh_vien[hhb.so_nguoi - 1] = ps->id;

                        *b = hhb;
                        btupd(hk, ps->so_ho_khau, (char *)b, sizeof(HHB));
                    }

                    free(ps);

                    box(empty, ACS_VLINE, ACS_HLINE);

                    wclear(plist);
                    wrefresh(plist);
                    print_list();

                    mvwprintw(empty, 16, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 16, 2, empty);
                    if (ch == 27)
                        break;
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(titlew);
                wrefresh(titlew);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp1 == 1) // function 2:
            {
                thayDoiNhanKhau();
            }
            if (temp1 == 2) //function 1:
            {
                do
                {
                    BOXE(1);
                    print_hk();
                    mvwprintw(empty, 3, 2, "So Ho Khau: ");
                    ch = nhap(shk, "So Ho Khau: ", 3, 2, empty);
                    shk[6] = 0;

                    box(empty, ACS_VLINE, ACS_HLINE);
                    b = (HHB *)malloc(sizeof(HHB));
                    if (btsel(hk, shk, (char *)b, sizeof(HHB), &dsize))
                    {
                        mvwprintw(empty, 15, 2, "Khong tim thay so ho khau nay!! %s\n   Nhan ESC de thoat  ", shk);
                        ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                        break;
                    }

                    else
                    {
                        hhb = *b;

                        plist = newwin(45, 100, 1, 71);
                        box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window

                        mvwprintw(plist, 2, 2, "\tCAC THANH VIEN TRONG SO HO KHAU %n", hhb1->so_ho_khau);
                        mvwprintw(plist, 3, 2, "%5s%-8s%8s%-11s%5s", "", "ID", "", "TEN", "");
                        for (int i = 0; i < hhb.so_nguoi; i++)
                        {
                            line++;
                            sprintf(s, "%02u", hhb.id_thanh_vien[i]);
                            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
                            mvwprintw(plist, 4 + i, 2, "%3s%05u%8s%-24s", "", ps->id, "", ps->ten);
                        }
                        wrefresh(plist);

                        // *c = ps;
                        mvwprintw(empty, 5, 2, "Chu Ho hien tai co id :%d", hhb.chu_ho);
                        mvwprintw(empty, 6, 2, "Nhap ID chu ho moi : ");
                        ch = nhap(str2, "Nhap ID chu ho moi : ", 6, 2, empty);

                        sprintf(ID, "%02d", atoi(str2));
                        __fpurge(stdin);
                        c = (PS *)malloc(sizeof(PS));
                        if (btsel(nk, ID, (char *)c, sizeof(PS), &dsize))
                        {
                            mvwprintw(empty, 15, 2, "Khong tim thay so nhan khau nay!! \n   Nhan ESC de thoat  ");
                            ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                            break;
                        }

                        else
                        {
                            char ngay[15];
                            hhb.thay_doi->ngay = get_time_now();
                            strftime(ngay, 15, "%d/%m/%Y", &hhb.thay_doi->ngay);
                            ps1 = *c;
                            // *c = ps1;
                            mvwprintw(empty, 11, 2, "\t\tTHONG TIN CHU HO MOI");
                            mvwprintw(empty, 12, 2, "  %-5s|%-15s|%-50s|%-15s|", "ID", "TEN", "DIA CHI", "NGAY CHUYEN");
                            mvwprintw(empty, 13, 2, "  %-5d|%-15s|%-50s|%-15s|", ps1.id, ps1.ten, ps1.nguyen_quan, ngay);

                            hhb.chu_ho = atoi(ID);
                            *b = hhb;
                            btupd(hk, shk, (char *)b, sizeof(HHB));
                        }
                    }

                    // free(c);
                    print_hk();
                    free(b);
                    mvwprintw(empty, 15, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                    if (ch == 27)
                        break;
                    box(empty, ACS_VLINE, ACS_HLINE);
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(titlew);
                wrefresh(titlew);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp1 == 3) // function 2:
            {
                do
                {
                    BOXE(1);
                    line = print_hk();
                    mvwprintw(empty, 3, 2, "So Ho Khau: ");
                    ch = nhap(shk, "So Ho Khau: ", 3, 2, empty);
                    shk[6] = 0;

                    box(empty, ACS_VLINE, ACS_HLINE);
                    hhb1 = (HHB *)malloc(sizeof(HHB));
                    ps = (PS *)malloc(sizeof(PS));

                    if (btsel(hk, shk, (char *)hhb1, sizeof(HHB), &dsize))
                    {
                        mvwprintw(empty, 15, 2, "Khong tim thay so ho khau nay!! %s\n   Nhan ESC de thoat  ", str1);
                        ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                        if (ch == 27)
                            break;
                    }

                    else
                    {
                        plist = newwin(45, 100, 1, 71);
                        box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window

                        mvwprintw(plist, 2, 2, "\tCAC THANH VIEN TRONG SO HO KHAU %n", hhb1->so_ho_khau);
                        mvwprintw(plist, 3, 2, "%5s%-8s%8s%-11s%5s%-10s", "", "ID", "", "TEN", "", "QUAN HE");
                        for (int i = 0; i < hhb1->so_nguoi; i++)
                        {
                            line++;
                            sprintf(s, "%02u", hhb1->id_thanh_vien[i]);
                            btsel(nk, s, (char *)ps, sizeof(PS), &dsize);
                            mvwprintw(plist, 4 + i, 2, "%3s%05u%8s%-24s%s", "", ps->id, "", ps->ten, ps->quan_he_chu_ho);
                        }
                        wrefresh(plist);
                        mvwprintw(empty, 5, 2, "Nhap so nhan khau can tach ra ho khau moi: ");
                        ch = nhap(ds, "Nhap so nhan khau can tach ra ho khau moi: ", 5, 2, empty);
                        if (ch == 27)
                            break;
                        for (int i = 0; i < atoi(ds); i++)
                        {
                            mvwprintw(empty, 6 + i, 2, "Nhap id thanh vien thu %d: ", i + 1);
                            ch = nhap(str2, "Nhap id thanh vien thu d: ", 6 + i, 2, empty);
                            if (ch == 27)
                                break;
                            a = atoi(str2);
                            if (!find(hhb1->id_thanh_vien, hhb1->so_nguoi, a))
                            {
                                i--;
                                continue;
                            }
                            hk2[i] = a;
                        }
                        tach_ra(hhb1, hk2, atoi(ds));
                    }
                    free(b);
                    free(hhb1);

                    mvwprintw(empty, 15, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 15, 2, empty);
                    if (ch == 27)
                        break;
                    box(empty, ACS_VLINE, ACS_HLINE);
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(titlew);
                wrefresh(titlew);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp1 == 4) //function 1:
            {               // tam tru tam vang

                tamtrutamvang();
            }
            if (temp1 == 5) //unction 1:
            {
                thongke();
            }
        }
        BOX(miw);
        wattron(miw, A_STANDOUT);
        sprintf(item, "%-25s", list1[temp1]);
        mvwprintw(miw, temp1 + 1, 2, "%s", item);
        wattroff(miw, A_STANDOUT);
    }
    delwin(miw);
}
void Ham2()
{
    char s[10];
    PS ps, *c = (PS *)malloc(sizeof(PS));
    int dsize;
    int ch;
    char str1[10];
    miw = newwin(12, 70, 4, 1); // create a new window
    BOX(miw);
    // now print all the menu items and highlight the first one
    for (temp2 = 0; temp2 < 2; temp2++)
    {
        if (temp2 == 0)
            wattron(miw, A_STANDOUT); // highlights the first item.
        else
            wattroff(miw, A_STANDOUT);
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
    }
    wrefresh(miw); // update the terminal screen
    temp2 = 0;
    noecho();          // disable echoing of characters on the screen
    keypad(miw, TRUE); // enable keyboard input for the window.
    curs_set(0);       // hide the default screen cursor.
    while ((ch = wgetch(miw)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp2--;
            temp2 = (temp2 < 0) ? 1 : temp2;
            break;
        case KEY_DOWN:
            temp2++;
            temp2 = (temp2 > 1) ? 0 : temp2;
            break;
        case 10:
            if (temp2 == 0) //function 1:   khai_bao_covid();
            {
                // khai_bao_covid();
                do
                {
                    BOXE(2);
                    print_list();
                    mvwprintw(empty, 3, 2, "ID Nguoi khai bao:");
                    ch = nhap(str1, "ID nguoi khai bao:", 3, 2, empty);
                    sprintf(s, "%02d", atoi(str1));
                    if (ch == 27)
                        break;
                    box(empty, ACS_VLINE, ACS_HLINE);

                    if (btsel(nk, s, (char *)c, sizeof(PS), &dsize))
                    {
                        mvwprintw(empty, 4, 2, "Khong tim thay nhan khau nay!!");
                    }

                    else
                    {
                        ps = *c;
                        ps.lien_quan_covid = 1;
                        ps.thong_tin_khai_bao = covid();
                        *c = ps;
                        btupd(nk, s, (char *)c, sizeof(PS));
                    }
                    mvwprintw(empty, 13, 2, "Nhap thanh cong \n   Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 10, 2, empty);
                    if (ch == 27)
                        break;
                    free(c);
                    box(empty, ACS_VLINE, ACS_HLINE);
                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
                wclear(empty);
                wrefresh(empty);
            }
            if (temp2 == 1) // function 2:
            {
                do
                {
                    plist = newwin(45, 150, 1, 1);
                    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    int line = 2;
                    PS nhan_khau;
                    char *c, buffer[20];
                    char str[100];
                    int dsize;
                    btpos(nk, 1);
                    c = (char *)malloc(sizeof(PS));
                    mvwprintw(plist, 1, 1, "  %-5s|%5s%-15s| %-7s| %-5s|%-12s|  %-8s |%1s%-14s|%20s%-30s|", "ID", "", "TEN", "MUC DO", "TEST", "LAN TEST CUOI", "KET QUA", "", "TINH TRANG SK", "", "DIA CHI");
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
                                mvwprintw(plist, line, 2, "%05u    %-22s F%-7c%-7c%-13s%-14s%-15s%s", nhan_khau.id, nhan_khau.ten, nhan_khau.thong_tin_khai_bao.muc_do + 48, nhan_khau.thong_tin_khai_bao.so_lan_test + 48, buffer, nhan_khau.thong_tin_khai_bao.ket_qua[nhan_khau.thong_tin_khai_bao.so_lan_test - 1] ? "Duong tinh" : "Am tinh", nhan_khau.thong_tin_khai_bao.tinh_trang_sk, nhan_khau.nguyen_quan);
                                line++;
                            }
                    }

                    mvwprintw(plist, 39, 2, "Nhan ESC de thoat  ");
                    ch = nhap(str1, "Nhan ESC de thoat  ", 39, 2, plist);
                    if (ch == 27)
                        break;
                    box(plist, ACS_BULLET, ACS_BULLET); // sets default borders for the window
                    wrefresh(plist);
                    free(c);

                } while (wgetch(empty) != 27);
                wclear(plist);
                wrefresh(plist);
            }
            break;
        }
        BOX(miw);
        wattron(miw, A_STANDOUT);
        sprintf(item, "%-15s", list2[temp2]);
        mvwprintw(miw, temp2 + 1, 2, "%s", item);
        wattroff(miw, A_STANDOUT);
    }
    delwin(miw);
}
void truongXom()
{
    int ch;
    w = newwin(12, 70, 4, 1); // create a new window
    BOX(w);
    // now print all the menu items and highlight the first one
    for (temp = 0; temp < 2; temp++)
    {
        if (temp == 0)
            wattron(w, A_STANDOUT); // highlights the first item.
        else
            wattroff(w, A_STANDOUT);
        sprintf(item, "%-28s", list[temp]);
        mvwprintw(w, temp + 1, 2, "%s", item);
    }
    wrefresh(w); // update the terminal screen
    temp = 0;
    noecho();        // disable echoing of characters on the screen
    keypad(w, TRUE); // enable keyboard input for the window.
    curs_set(0);     // hide the default screen cursor.
    while ((ch = wgetch(w)) != 27)
    {
        // right pad with spaces to make the items appear with even width.
        sprintf(item, "%-28s", list[temp]);
        mvwprintw(w, temp + 1, 2, "%s", item);
        // use a variable to increment or decrement the value based on the input.
        switch (ch)
        {
        case KEY_UP:
            temp--;
            temp = (temp < 0) ? 1 : temp;
            break;
        case KEY_DOWN:
            temp++;
            temp = (temp > 1) ? 0 : temp;
            break;
        case 10:
            if (temp == 0) //function 1:
            {
                Ham1();
            }
            if (temp == 1) // function 2:
            {
                Ham2();
                //thong tin covid
            }
            break;
        }
        BOX(w);
        wattron(w, A_STANDOUT);
        sprintf(item, "%-28s", list[temp]);
        mvwprintw(w, temp + 1, 2, "%s", item);
        wattroff(w, A_STANDOUT);
    }
    delwin(w);
}
void dangNhap()
{
    int ch;
    char tai_khoan[50], mat_khau[50];
    // login = newwin(12, 50, 4, 1); // create a new window
    BOXlogin();
    wrefresh(login); // update the terminal screen
    temp = 0;
    noecho();            // disable echoing of characters on the screen
    keypad(login, TRUE); // enable keyboard input for the window.
    curs_set(0);         // hide the default screen cursor.

    do
    {
        BOXlogin();
        ch = nhap(tai_khoan, "Tai Khoan: ", 2, 2, login);
        box(derwin(login, 3, 50, 1, 1), ACS_VLINE, ACS_HLINE);
        chuanHoaTen(tai_khoan);
        if (ch == 27)
            break;

        ch = password(mat_khau, "Mat Khau: ", 5, 2, login);
        box(derwin(login, 3, 50, 4, 1), ACS_VLINE, ACS_HLINE);
        chuanHoaTen(mat_khau);
        if (ch == 27)
            break;
        if ((strcmp(tai_khoan, "admin1") == 0 && strcmp(mat_khau, "123456") == 0) || (strcmp(tai_khoan, "admin2") == 0 && strcmp(mat_khau, "123456") == 0) || (strcmp(tai_khoan, "admin3") == 0 && strcmp(mat_khau, "123456") == 0))
        {
            truongXom();
        }
        else if (strcmp(tai_khoan, "covid19") == 0 && strcmp(mat_khau, "123456") == 0)
        {
            Ham2();
        }
        else
            mvwprintw(login, 7, 2, "%s", "Sai Tai Khoan, Mat Khau");

        box(login, ACS_VLINE, ACS_HLINE); // sets default borders for the window
    } while (wgetch(login) != 27);
    wclear(login);
    wrefresh(login);
    delwin(login);
}
int main()
{
    if (btinit())
    {
        printf("Err\n");
        return 0;
    }
    open_file_bt("ho_khau.data", "nhan_khau.data");
    // resest();
    initscr(); // initialize Ncurses
    initialise_colors();
    // truongXom();
    // Ham1();
    dangNhap();
    endwin();
    close_file_bt();}