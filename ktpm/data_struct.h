#ifndef _DATA_STRUCT
#define _DATA_STRUCT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include "bt-5.0.1/inc/btree.h"
#include <time.h>
typedef struct tm DAY;
typedef char ADR[100];
typedef struct _remove
{
    DAY ngay_chuyen; //Ngay chuyen di noi khac
    ADR noi_chuyen;  //Dia chi noi da chuyen den
} RM;
typedef struct _change
{
    char noi_dung[100]; //Noi dung thay doi
    DAY ngay;           //Ngay thay doi
} CH;
typedef struct _covid
{
    char tinh_trang_sk[100]; //

    DAY thoi_gian;     //Thoi gian bat dau cach ly
    char so_ngay;      //So ngay can cach ly
    char muc_do;       //Muc do cach ly(0: F0, 1: F1, 2: F2...)
    char test;         //1 neu da test covid, 0 neu chua test
    char so_lan_test;  //So lan thuc hien test covid
    DAY ngay_test;     //Ngay test gan nhat
    char ket_qua[100]; //Ket qua cac lan test; 0 neu am tinh, 1 neu duong tinh
} COVID;
typedef struct _khai_tu
{
    char so_giay_khai_tu[100];
    unsigned int id_nguoi_khai;
    DAY ngay_khai;
    DAY ngay_chet;
    char li_do_chet[100];
} KT;
typedef struct _tam_tru
{
    char ma_giay_tam_tru[20];
    int so_dien_thoai_nguoi_dk;
    DAY tu_ngay;
    DAY den_ngay;
    char li_do[100];
} TT;
typedef struct _tam_vang
{
    char ma_giay_tam_vang[20];
    ADR noi_tam_tru;
    DAY tu_ngay;
    DAY den_ngay;
    char li_do[100];
} TV;
typedef struct _person
{
    unsigned int id;
    char so_ho_khau[9];          //So ho khau nguoi nay dang truc thuoc
    char chu_ho;                 // 1 neu la chu ho, 0 neu ko la chu ho
    char gioi_tinh;              // 1 neu la nam,0 neu la nu
    char quan_he_chu_ho[30];     //0 neu la chu ho, 1: ong(ba), 2:cha(me), 3: vo(chong), 4: con, 5: anh(chi), 6: em, 7: chau ruot
    char ten[50];                //Ho va ten
    char bi_danh[20];            //Ten thuong goi
    DAY ngay_sinh;               //Ngay thang nam sinh
    ADR noi_sinh;                //Noi sinh
    ADR nguyen_quan;             //Nguyen quan
    char dan_toc[20];            //Dan toc
    char quoc_tich[20];          //Quoc tich
    char nghe_nghiep[20];        //Nghe nghiep
    char noi_lam_viec[50];       //Noi lam viec
    char CMND[12];               //So chung minh nhan dan hoac can cuoc cong dan
    DAY ngay_cap;                //Ngay cap cmnd(cccd)
    ADR noi_cap;                 //Noi cap cmnd(cccd)
    DAY ngay_dk_thuong_tru;      //Ngay dang ky thuong tru
    ADR dia_chi_thuong_tru_tkcd; //Dia chi thuong tru truoc khi chuyen den
    /*******************************************************************************/
    char chuyen_di;  //0 neu chua chuyen di noi khac, 1 neu da chuyen di noi khac
    RM thong_tin_cd; //Thong tin chuyen di noi khac
    /*******************************************************************************/
    char chet;
    KT khai_tu;
    /*******************************************************************************/
    char tru;
    TT tam_tru;
    /*******************************************************************************/
    char vang;
    TV tam_vang;
    /*******************************************************************************/
    char lien_quan_covid;     //0 neu khong lien quan den vung dich, 1 neu co lien quan den dung dich
    COVID thong_tin_khai_bao; //Thong tin khai bao covid(co lien quan den vung dich);

    char ghi_chu[100]; //Phan ghi chu them
} PS;
typedef struct _house_hold_book
{
    char so_ho_khau[9];  //So ho khau(9 so)
    unsigned int chu_ho; //id cua chu ho
    ADR dia_chi;
    char so_nguoi;                  //Tong so nguoi trong gia dinh
    unsigned int id_thanh_vien[15]; //id cac thanh vien trong gia dinh
    char so_thay_doi;
    CH thay_doi[10]; //Thong tin thay doi lien quan den ca ho
} HHB;
/************************CAC HOAT DONG BIEN DOI NHAN KHAU************************
 * 1-Them nhan khau moi
 * 2-Thay doi nhan khau
 *********************QUAN LY THONG TIN PHONG TRANH COVID-19*********************/
#endif
// DAY get_time_now()
// {
//     time_t raw_time;
//     time(&raw_time);
//     struct tm now;
//     now = *localtime(&raw_time);
//     char *buffer[80];
//     // strftime(buffer, 80, "%d/%m/%Y - %I:%M%p", &now);
//     // printf("Date & time đã định dạng theo hàm strftime là: \n|%s|\n", buffer);
//     return now;
// }

// BTA *hk; //Nhan khau
// BTA *nk; //Ho khau
// int file_not_found(char *name)
// {
//     FILE *f;
//     f = fopen(name, "r");
//     if (!f)
//         return 1;
//     else
//     {
//         fclose(f);
//         return 0;
//     }
// }
// void open_file_bt(char *name1, char *name2)
// {
//     if (file_not_found(name1))
//     {
//         hk = btcrt(name1, 0, FALSE);
//         btdups(hk, TRUE);
//     }
//     else
//         hk = btopn(name1, 0, FALSE);
//     if (file_not_found(name2))
//     {
//         nk = btcrt(name2, 0, FALSE);
//         btdups(nk, FALSE);
//     }
//     else
//         nk = btopn(name2, 0, FALSE);
// }
// void close_file_bt()
// {
//     btcls(nk);
//     btcls(hk);
// }