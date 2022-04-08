#include <curses.h>
void send_out(WINDOW *win, const char *fmt, ...)
{
    char InStr[1024];
    va_list args;
    va_start(args, fmt);
    vsprintf(InStr, fmt, args);
    va_end(args);
    int x, y = 0;
    char OutStr[1024];

    if (win == NULL)
        win = stdscr;

    for (x = 0; x <= 1024; x++)
    {
        if (InStr[x] == '\0')
        {
            OutStr[y] = '\0';
            wprintw(win, OutStr);
            break;
        }
        else if (InStr[x] != '&')
        {
            OutStr[y] = InStr[x];
            ++y;
        }
        else if (InStr[x] == '&')
        {
            OutStr[y] = '\0';
            wprintw(win, OutStr);
            y = 0;
            ++x;
            if (InStr[x] == 'C')
            {
                wattron(win, COLOR_PAIR(3) | A_BLINK);
                wattrset(win, COLOR_PAIR(3) | A_BLINK);
            }
            if (InStr[x] == 'K')
            {
                wattron(win, COLOR_PAIR(2) | A_BLINK);
                wattrset(win, COLOR_PAIR(2) | A_BLINK);
            }
        }
    }
}

int main()
{
    initscr();
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_WHITE);
    attron(COLOR_PAIR(1) | A_BLINK);
    bkgd(COLOR_PAIR(2) | A_BLINK); // Green Text on a White Screen

    send_out(stdscr, "This text should be GREEN on a WHITE screen.\n\n");
    send_out(stdscr, "&KThis text should be BLACK on a WHITE screen.\n\n");
    send_out(stdscr, "&CThis text should be CYAN on a WHITE screen.\n\n");
    send_out(stdscr, "&CCyan &KBlack &CDifferent &KColors.\n\n");

    refresh();

    getch();
    endwin();

    return 0;
}