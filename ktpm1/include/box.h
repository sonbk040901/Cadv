#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <ctype.h>
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
    for (int i = 0; i < (int)strlen(s); i++)
    {
        s[i] = tolower(s[i]);
    }
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
void BOX(WINDOW *x, WINDOW *titlew) // in ra window menu
{
    box(x, ACS_VLINE, ACS_HLINE); // sets default borders for the window
    mvwprintw(x, 9, 1, "Press <ENTER> to see the option selected");
    mvwprintw(x, 10, 1, "Up and Down arrow keys to naviage (ESC to Exit)");
    titlew = newwin(3, 50, 1, 1);
    wattrset(titlew, COLOR_PAIR(2));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%17sQUAN LY HO KHAU%18s", " ", " ");
    wattroff(titlew, COLOR_PAIR(2));
    wrefresh(titlew);
    wrefresh(x);
}
void BOXlogin(WINDOW *login, WINDOW *titlew) //in ra window function
{
    login = newwin(12, 50, 4, 1);
    box(login, ACS_VLINE, ACS_HLINE);
    wattron(login, A_STANDOUT);
    wattrset(login, COLOR_PAIR(14));
    mvwprintw(login, 1, 2, "%s", "Tai Khoan");
    mvwprintw(login, 3, 2, "%s", "Mat Khau");
    wattroff(login, COLOR_PAIR(14));
    titlew = newwin(3, 50, 1, 1);
    wattrset(titlew, COLOR_PAIR(2));
    box(titlew, ACS_BULLET, ACS_BULLET); // sets default borders for the window
    mvwprintw(titlew, 1, 1, "%17sQUAN LY HO KHAU%18s", " ", " ");
    wattroff(titlew, COLOR_PAIR(2));
    wrefresh(titlew);
    wrefresh(login);
}

