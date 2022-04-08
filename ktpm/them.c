#include "data_struct.h"
#include <stdio_ext.h>
#define MAX_CHOSE 4
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
        nk = btopn(name2, 0, FALSE);
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

void menu()
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%14sLUA CHON TU CACH DANG NHAP%14s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Them ho khau");
    printf("\t\t\t|  2  |   %-45s|\n", "Them nhan khau");
    printf("\t\t\t|  3  |   %-45s|\n", "In ra ho khau");
    printf("\t\t\t|  4  |   %-45s|\n", "Ket thuc chuong trinh");
    printf("\t\t\t|======================================================|\n");
}
int main(int argc, char const *argv[])
{
    btinit();
    open_file_bt("ho_khau.data", "nhan_khau.data");
    HHB ho_khau, *root;
    PS nhan_khau, *ruut;
    char str[256];
    int dsize;
    int chose;
    int d;
    char *c, *f;
    do
    {
        menu();
        printf("Hay nhap lua chon cua ban: ");
        while (!scanf("%d", &chose))
        {
            printf("Nhap sai: Khong duoc nhap chu!\nChi duoc nhap so tu %d - %d\n", 1, MAX_CHOSE);
            menu();
            printf("Hay nhap lua chon cua ban: ");
            __fpurge(stdin);
        }
        __fpurge(stdin);
        switch (chose)
        {
        case 1:
            root = (HHB *)malloc(sizeof(HHB));
            printf("Nhap so ho khau: ");
            fgets(ho_khau.so_ho_khau, 10, stdin);
            ho_khau.so_ho_khau[strlen(ho_khau.so_ho_khau) - 1] = 0;
            ho_khau.so_nguoi = 8;
            printf("Nhap chu ho: ");
            scanf("%u", &(ho_khau.chu_ho));
            for (int i = 0; i < 8; i++)
            {
                printf("Nhap id tung nguoi trong gd: ");
                scanf("%u", &(ho_khau.id_thanh_vien[i]));
            }

            printf("Nhap noi thuong tru:\n");
            __fpurge(stdin);
            fgets(ho_khau.dia_chi, 100, stdin);
            ho_khau.dia_chi[strlen(ho_khau.dia_chi) - 1] = 0;
            ho_khau.so_thay_doi = 1;
            strcpy(ho_khau.thay_doi[0].noi_dung, "Tao moi so ho khau");
            ho_khau.thay_doi[0].ngay = get_time_now();
            memcpy(root, &ho_khau, sizeof(ho_khau));
            btins(hk, root->so_ho_khau, (char *)root, sizeof(HHB));
            free(root);
            break;
        case 2:
            ruut = (PS *)malloc(sizeof(PS));
            printf("Nhap so ho khau: ");
            fgets(nhan_khau.so_ho_khau, 10, stdin);
            nhan_khau.so_ho_khau[strlen(nhan_khau.so_ho_khau) - 1] = 0;
            printf("Nhap id: ");
            scanf("%s", str);
            nhan_khau.id = atoi(str);
            printf("Nhap ten: ");
            __fpurge(stdin);
            fgets(nhan_khau.ten, 50, stdin);
            nhan_khau.ten[strlen(nhan_khau.ten) - 1] = 0;
            __fpurge(stdin);
            printf("Ngay test gan nhat(dd/mm/yyyy): ");
            scanf("%d/%d/%d", &(nhan_khau.ngay_sinh.tm_mday), &(nhan_khau.ngay_sinh.tm_mon), &(nhan_khau.ngay_sinh.tm_year));
            nhan_khau.ngay_sinh.tm_mon--;
            nhan_khau.ngay_sinh.tm_year -= 1900;
            printf("Nam hay nu: ");
            scanf("%d", &d);
            __fpurge(stdin);
            nhan_khau.gioi_tinh = d;
            printf("Quan he voi chu ho: ");
            fgets(nhan_khau.quan_he_chu_ho, 30, stdin);
            nhan_khau.quan_he_chu_ho[strlen(nhan_khau.quan_he_chu_ho) - 1] = 0;
            printf("Nhap dia chi thuong tru: ");
            fgets(nhan_khau.nguyen_quan, 100, stdin);
            nhan_khau.nguyen_quan[strlen(nhan_khau.nguyen_quan) - 1] = 0;
            memcpy(ruut, &nhan_khau, sizeof(PS));
            btins(nk, str, (char *)ruut, sizeof(PS));
            free(ruut);
            break;
        case 3:
            // btpos(hk, 1);
            // c = (char *)malloc(sizeof(HHB));
            // while (!btseln(hk, str, c, sizeof(HHB), &dsize))
            // {
            //     ho_khau = *(HHB *)c;
            //     printf("So ho khau: %s\n", ho_khau.so_ho_khau);
            //     printf("Dia chi thuong tru: %s\n", ho_khau.dia_chi);
            //     printf("id chu ho: %09u\n", ho_khau.chu_ho);
            //     for (int i = 0; i < 8; i++)
            //     {
            //         printf("id tv %d: %09u\n", i + 1, ho_khau.id_thanh_vien[i]);
            //     }
            // }
            // free(c);
            btpos(nk, 1);
            c = (char *)malloc(sizeof(PS));
            while (!btseln(nk, str, c, sizeof(PS), &dsize))
            {
                nhan_khau = *(PS *)c;
                printf("So ho khau: %s\n", nhan_khau.so_ho_khau);
                printf("Dia chi thuong tru: %s\n", nhan_khau.nguyen_quan);
                printf("id: %s\tTen: %s\n", str, nhan_khau.ten);
                printf("Quan he: %s\n", nhan_khau.quan_he_chu_ho);
            }
            free(c);
            break;
        case 4:
            printf("\tKet thuc chuong trinh\n");
            chose = 0;
            close_file_bt();
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}
