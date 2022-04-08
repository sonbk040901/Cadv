#include <stdio.h>
#include <jval.h>
#include <jrb.h>
#include <dllist.h>
#include <fields.h>
#include <graph.h>
#include <curses.h>
#define DELAY 500000
#define ESC 27
#define ENTER 10
WINDOW *topbox, *menuw, *chosew, *minibox;
char text[20][50] = {"DOC FILE DATA", "THUC HIEN CONG VIEC", "DANH SACH CONG VIEC", "THOI GIAN TOI THIEU", "KIEM TRA TINH HOP LE CUA DATA"};
char text1[3][50] = {"CONG VIEC NGAY TRUOC", "TAT CA CONG VIEC"};
int isKill = 0;
void initialise_colors()
{
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
	init_pair(6, COLOR_BLACK, COLOR_GREEN);
	init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(8, COLOR_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_BLACK);
	init_pair(10, COLOR_CYAN, COLOR_BLACK);
	init_pair(11, COLOR_BLUE, COLOR_BLACK);
	init_pair(12, COLOR_YELLOW, COLOR_BLACK);
	init_pair(13, COLOR_WHITE, COLOR_BLACK);
	init_pair(14, COLOR_GREEN, COLOR_BLACK);
	init_pair(15, COLOR_MAGENTA, COLOR_BLACK);
}
void standout_chose(WINDOW *win, int *y, int x, int min, int max, char list[][50], int up /*1: up, 0: down*/)
{
	wattroff(win, A_STANDOUT | A_BLINK);
	mvwprintw(win, *y, x, "%-43s", list[*y - min]);
	wattron(win, A_STANDOUT | A_BLINK);
	if (up)
		*y = (--*y) < min ? max : *y;
	else
		*y = (++*y) > max ? min : *y;
	mvwprintw(win, *y, x, "%-43s", list[*y - min]);
	wattroff(win, A_STANDOUT | A_BLINK);
	wrefresh(win);
}
void kill(void)
{
	wbkgd(topbox, COLOR_PAIR(10));
	wclear(topbox);
	wclear(menuw);
	wrefresh(menuw);
	wrefresh(topbox);
	delwin(topbox);
	delwin(menuw);
	isKill = TRUE;
}
int show_menu()
{
	clear();
	refresh();
	isKill = FALSE;
	int xmax, ch, x = 3, y = 2, min = 2, max = 6;
	xmax = getmaxx(stdscr);
	topbox = newwin(5, 50, 5, xmax / 2 - 25);
	box(topbox, '#', '-');
	wbkgd(topbox, COLOR_PAIR(2) | A_BOLD);
	mvwaddstr(topbox, 0, 13, " _  _  ____  __ _  _  _ ");
	mvwaddstr(topbox, 1, 13, "( \\/ )(  __)(  ( \\/ )( \\");
	mvwaddstr(topbox, 2, 13, "/ \\/ \\ ) _) /  \\ /) \\/ (");
	mvwaddstr(topbox, 3, 13, "\\_)(_/(____)\\_)__)\\____/");
	menuw = newwin(10, 50, 9, xmax / 2 - 25);
	wattron(menuw, COLOR_PAIR(2));
	box(menuw, 0, 0);
	wattron(menuw, COLOR_PAIR(14));
	mvwaddstr(menuw, 8, 2, "Press ESC to exit!");
	wattrset(menuw, COLOR_PAIR(12));
	for (int i = 0; i < 5; i++)
	{
		if (!i)
			wattron(menuw, A_STANDOUT | A_BLINK);
		mvwprintw(menuw, 2 + i, 3, "%-43s", text[i]);
		if (!i)
			wattroff(menuw, A_STANDOUT | A_BLINK);
	}
	wrefresh(topbox);
	wrefresh(menuw);
	keypad(menuw, TRUE);
	noecho();
	while ((ch = wgetch(menuw)) != ENTER && ch != ESC)
	{
		switch (ch)
		{
		case KEY_UP:
			standout_chose(menuw, &y, x, min, max, text, 1);
			break;
		case KEY_DOWN:
			standout_chose(menuw, &y, x, min, max, text, 0);
			break;
		default:
			break;
		}
	}
	return ch == ESC ? 0 : y - min + 1;
}
void convert(Graph g)
{
	clear();
	refresh();
	char *cut;
	int line = 0, edges = 0, vertices = 0;
	IS is = new_inputstruct("data.txt");
	while (get_line(is) >= 0)
	{
		edges += is->NF;
		vertices += addVertex(g, vertices, strdup(is->fields[0]));
		if (vertices > 1)
		{
			for (int i = 2; i < is->NF; i++)
			{
				cut = strtok(is->fields[i], "-");
				addEdge(g, vertices - 1, getId(g, cut), (double)atoi(strtok(NULL, "\n")));
			}
		}
		line++;
	}
	JRB root, root1, node;
	jrb_traverse(root, g->vertices)
	{
		printw("%s: ", jval_s(root->val));
		node = (JRB)jval_v(jrb_find_int(g->edges, jval_i(root->key))->val);
		jrb_traverse(root1, node)
		{
			printw("%s-%g ", getVertex(g, jval_i(root1->key)), jval_d(root1->val));
		}
		printw("\n");
	}
	refresh();
	int xm, ym;
	getyx(menuw, ym, xm);
	chosew = newwin(10, 50, ym, xm);
	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	wattron(chosew, COLOR_PAIR(14));
	mvwprintw(chosew, 2, 2, "Tong cac cong viec la: ");
	wattron(chosew, COLOR_PAIR(13));
	wprintw(chosew, "%d", line);
	wattroff(chosew, COLOR_PAIR(13));
	wattron(chosew, COLOR_PAIR(14));
	mvwprintw(chosew, 3, 2, "Tong so cac quan he thuc hien truoc: ");
	wattroff(chosew, COLOR_PAIR(14));
	wattron(chosew, COLOR_PAIR(13));
	wprintw(chosew, "%d", edges - line * 2);
	wattroff(chosew, COLOR_PAIR(13));
	wattron(chosew, COLOR_PAIR(14));
	mvwaddstr(chosew, 8, 2, "Press any key to continue...");
	wattrset(chosew, COLOR_PAIR(12));
	wrefresh(chosew);
	wgetch(chosew);
	wclear(chosew);
	wrefresh(chosew);
	jettison_inputstruct(is);
	delwin(chosew);
}
int show_menu_2()
{

	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	int ch, x = 3, y = 2, min = 2, max = 3;
	for (int i = 0; i < 3; i++)
	{
		if (!i)
			wattron(chosew, A_STANDOUT | A_BLINK);
		mvwprintw(chosew, 2 + i, 3, "%-43s", text1[i]);
		if (!i)
			wattroff(chosew, A_STANDOUT | A_BLINK);
	}
	mvwaddstr(chosew, 8, 2, "Press ESC to back...");
	wrefresh(chosew);
	keypad(chosew, TRUE);
	while ((ch = wgetch(chosew)) != ENTER && ch != ESC)
	{
		switch (ch)
		{
		case KEY_UP:
			standout_chose(chosew, &y, x, min, max, text1, 1);
			break;
		case KEY_DOWN:
			standout_chose(chosew, &y, x, min, max, text1, 0);
			break;
		default:
			break;
		}
	}
	return ch == ESC ? 0 : y - min + 1;
}
int visited1[100], sum;
void case2(Graph g, int id)
{
	int output[10], total;
	total = outdegree(g, id, output);
	if (!total)
		return;
	for (int i = 0; i < total; i++)
	{
		if (visited1[output[i]])
			continue;
		visited1[output[i]] = TRUE;
		mvwprintw(minibox, 1 + sum++, 2, "%s", getVertex(g, output[i]));
		case2(g, output[i]);
		wrefresh(minibox);
	}
}
void job(Graph g)
{
	clear();
	refresh();
	int xm, ym;
	char cv[10];
	int running = 1;
	int output[10], total = 0;
	getyx(menuw, ym, xm);
	chosew = newwin(10, 50, ym, xm);
	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	wattron(chosew, COLOR_PAIR(14));
	echo();
	curs_set(TRUE);
	mvwaddstr(chosew, 2, 3, "Hay nhap cong viec ban can xem: ");
	wattroff(chosew, COLOR_PAIR(14));
	wattron(chosew, COLOR_PAIR(13));
	wgetstr(chosew, cv);
	wrefresh(chosew);
	noecho();
	curs_set(FALSE);
	if (!hasVertex(g, cv))
	{
		mvwprintw(chosew, 4, 4, "Khong ton tai cong viec %s trong danh sach", cv);
		wattron(chosew, COLOR_PAIR(14));
		mvwaddstr(chosew, 8, 2, "Press any key to continue...");
		wattroff(chosew, COLOR_PAIR(14));
		wrefresh(chosew);
		wgetch(chosew);
	}
	else
	{
		do
		{
			running = show_menu_2();
			switch (running)
			{
			case 1:
				total = outdegree(g, getId(g, cv), output);
				minibox = newwin(10, 50, ym, xm);
				box(minibox, 0, 0);
				if (total == 0)
				{
					mvwprintw(minibox, 4, 4, "Cong viec %s cong viec co so!", cv);
				}
				else
				{
					mvwprintw(minibox, 2, 3, "Co %d cong viec ngay truoc %s la: ", total, cv);
					for (int i = 0; i < total; i++)
						mvwprintw(minibox, 3 + i, 3, "\t%s", getVertex(g, output[i]));
				}
				wattron(minibox, COLOR_PAIR(14));
				mvwprintw(minibox, 8, 2, "Press any key to continue...");
				wattroff(minibox, COLOR_PAIR(14));
				wrefresh(minibox);
				wgetch(minibox);
				wclear(minibox);
				wrefresh(minibox);
				delwin(minibox);
				break;
			case 2:
				memset(visited1, 0, sizeof(visited1));
				sum = 0;
				minibox = newwin(10, 50, ym, xm);
				box(minibox, 0, 0);
				case2(g, getId(g, cv));
				if (!sum)
				{
					mvwprintw(minibox, 4, 4, "Cong viec %s la cong viec co so!", cv);
				}
				wattron(minibox, COLOR_PAIR(14));
				mvwprintw(minibox, 8, 2, "Press any key to continue...");
				wattroff(minibox, COLOR_PAIR(14));
				wrefresh(minibox);
				wgetch(minibox);
				wclear(minibox);
				wrefresh(minibox);
				delwin(minibox);
				break;
			case 0:
				break;
			}
		} while (running);
	}
	wclear(chosew);
	wrefresh(chosew);
	delwin(chosew);
}
void list_j(Graph g)
{
	clear();
	refresh();
	int xm, ym, input[10], total, v;
	Dllist queue;
	JRB root;
	getyx(menuw, ym, xm);
	chosew = newwin(10, 50, ym, xm);
	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	wattron(chosew, COLOR_PAIR(14));
	noecho();
	curs_set(FALSE);
	mvwprintw(chosew, 2, 3, "Danh sach cong viec: ");
	memset(visited1, 0, sizeof(visited1));
	sum = 0;
	jrb_traverse(root, g->edges)
	{
		if (!outdegree(g, jval_i(root->key), input))
		{
			break;
		}
	}
	queue = new_dllist();
	dll_append(queue, new_jval_i(jval_i(root->key)));
	visited1[jval_i(root->key)] = TRUE;
	wattron(chosew, COLOR_PAIR(13));
	while (!dll_empty(queue))
	{
		v = jval_i(dll_first(queue)->val);
		wprintw(chosew, "%s ", getVertex(g, v));
		wrefresh(chosew);
		dll_delete_node(dll_first(queue));
		total = indegree(g, v, input);
		for (int i = 0; i < total; i++)
		{
			if (visited1[input[i]])
				continue;
			visited1[input[i]] = TRUE;
			dll_append(queue, new_jval_i(input[i]));
		}
	}
	wattroff(chosew, COLOR_PAIR(13));
	wattron(chosew, COLOR_PAIR(14));
	mvwaddstr(chosew, 8, 2, "Press any key to continue...");
	wattroff(chosew, COLOR_PAIR(14));
	wgetch(chosew);
	wclear(chosew);
	wrefresh(chosew);
	delwin(chosew);
}
void time_min(Graph g)
{

	clear();
	refresh();
	int xm, ym, s, f, output[20], length;
	double min;
	char cv[20];
	JRB root;
	getyx(menuw, ym, xm);
	chosew = newwin(10, 60, ym, xm);
	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	wattron(chosew, COLOR_PAIR(14));
	mvwaddstr(chosew, 2, 3, "Nhập công việc mà bạn muốn tính thời gian");
	wattroff(chosew, COLOR_PAIR(14));
	wrefresh(chosew);
	echo();
	curs_set(TRUE);
	mvwgetstr(chosew, 4, 20, cv);
	s = getId(g, cv);
	jrb_traverse(root, g->vertices)
	{
		if (!outdegree(g, jval_i(root->key), output))
			break;
	}
	f = jval_i(root->key);
	if (s == f)
	{
		mvwaddstr(chosew, 5, 10, "Cong viec nay la cong viec co so!");
	}
	else if (s == INFINITIVE_VALUE)
	{
		mvwprintw(chosew, 5, 5, "Khong ton tai cong viec %s trong du lieu!", cv);
	}
	else
	{
		min = shortestPath(g, s, f, output, &length);
		wattron(chosew, COLOR_PAIR(14));
		mvwprintw(chosew, 2, 2, "Thoi gian ngan nhat de hoan thanh cong viec %s la", cv);
		wattroff(chosew, COLOR_PAIR(14));
		mvwprintw(chosew, 4, 20, "%-3g", min);
	}
	wrefresh(chosew);
	noecho();
	curs_set(FALSE);
	wattron(chosew, COLOR_PAIR(14));
	mvwaddstr(chosew, 8, 2, "Press any key to continue...");
	wattroff(chosew, COLOR_PAIR(14));
	wgetch(chosew);
	wclear(chosew);
	wrefresh(chosew);
	delwin(chosew);
}
void check(Graph g)
{
	clear();
	refresh();
	int xm, ym;
	getyx(menuw, ym, xm);
	chosew = newwin(10, 60, ym, xm);
	wattron(chosew, COLOR_PAIR(7));
	box(chosew, 0, 0);
	wattroff(chosew, COLOR_PAIR(7));
	wattron(chosew, COLOR_PAIR(14));
	if (!DAG(g))
		mvwprintw(chosew, 2, 3, "Ton tai chu trinh");
	else
		mvwprintw(chosew, 2, 3, "Khong ton tai chu trinh");
	noecho();
	curs_set(FALSE);
	mvwaddstr(chosew, 8, 2, "Press any key to continue...");
	wattroff(chosew, COLOR_PAIR(14));
	wrefresh(chosew);
	wgetch(chosew);
	wclear(chosew);
	wrefresh(chosew);
	delwin(chosew);
}
int main()
{
	Graph graph;
	graph = createGraph();
	int running;
	initscr();
	initialise_colors();
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	echo();
	cbreak();
	do
	{
		running = show_menu();
		switch (running)
		{
		case 1:
			convert(graph);
			break;
		case 2:
			job(graph);
			break;
		case 3:
			list_j(graph);
			break;
		case 4:
			time_min(graph);
			break;
		case 5:
			check(graph);
			break;
		case 0:
			break;
		}
		if (!isKill)
			kill();
	} while (running);
	dropGraph(graph);
	endwin();
	return 0;
}