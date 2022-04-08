#include <stdio.h>
#include <curses.h>
#include <jval.h>
#include <jrb.h>
#include <dllist.h>
#include <graph.h>
#include <fields.h>
#define ESC 27
#define ENTER 10
#define DELAY 500
const char options0[5][50] = {"DOC NOI DUNG FILE", "THUC HIEN RANKPAGE (LAN DAU)", "THUC HIEN RANKPAGE (M LAN)", "SO LUONG WEBSIDE RA/VAO", "KHOANG CACH NHO NHAT"};
const char options1[3][50] = {"IN RA SO LUONG VA URL CUA TUNG WEB PAGE", "IN RA WEBPAGE IT LIEN KET TOI NHAT", "IN RA WEBPAGE NHIEU LIEN KET TOI NHAT"};
const char options4[2][50] = {"CHI CO LIEN KET DEN", "CHI CO LIEN KET RA"};
int chose0 = 1, chose1 = 1, chose4 = 1;
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
	mvwprintw(win, *y, x, "%-50s", list[*y - min]);
	wattron(win, A_STANDOUT | A_BLINK);
	if (up)
		*y = (--*y) < min ? max : *y;
	else
		*y = (++*y) > max ? min : *y;
	mvwprintw(win, *y, x, "%-50s", list[*y - min]);
	wattroff(win, A_STANDOUT | A_BLINK);
	wrefresh(win);
}
int show_menu(WINDOW *meow, int *n, int nchose, ...)
{
	wattron(meow, COLOR_PAIR(2));
	box(meow, 0, 0);
	wattron(meow, COLOR_PAIR(14));
	mvwprintw(meow, getmaxy(meow) - 2, 2, "Press ESC to exit...");
	wattrset(meow, COLOR_PAIR(12));
	va_list ap;
	va_start(ap, nchose);
	char str[10][50];
	for (int i = 0; i < nchose; i++)
	{
		if (i + 2 == *n)
			wattron(meow, A_STANDOUT | A_BLINK);
		strcpy(str[i], (char *)va_arg(ap, char *));
		mvwprintw(meow, 2 + i, 4, "%-50s", str[i]);
		if (i + 2 == *n)
			wattroff(meow, A_STANDOUT | A_BLINK);
	}
	keypad(meow, TRUE);
	noecho();
	int ch;
	while ((ch = wgetch(meow)) != ESC && ch != ENTER)
	{
		switch (ch)
		{
		case KEY_UP:
			standout_chose(meow, n, 4, 2, 1 + nchose, str, 1);
			break;
		case KEY_DOWN:
			standout_chose(meow, n, 4, 2, 1 + nchose, str, 0);
			break;
		default:
			break;
		}
	}
	wrefresh(meow);
	va_end(ap);
	return ch == ENTER ? *n - 1 : ESC;
}
void func11(FILE *f, FILE *f2, Graph g)
{
	int nline; //Max: 100 pages
	char str[100][55];
	int id[100], i;
	fseek(f, 0, SEEK_SET);
	fscanf(f, "%d\n", &nline);
	WINDOW *woof;
	woof = newwin(nline + 6, 65, 10, getmaxx(stdscr) / 2);
	box(woof, 0, 0);
	mvwprintw(woof, 2, 3, "Co %d page la:", nline);
	for (i = 0; i < nline; i++)
	{
		fscanf(f, "%s %d\n", str[i], id + i);
		addVertex(g, id[i], strdup(str[i]));
	}
	JRB root;
	i = 0;
	jrb_traverse(root, g->vertices)
		mvwprintw(woof, 3 + i++, 4, "%-55s%d", jval_s(root->val), jval_i(root->key));
	int nline2; // number of lines(Max: 100)
	int read = 0, root1;
	char ch;
	fseek(f2, 0, SEEK_SET);
	fscanf(f2, "%d\n", &nline2);
	for (i = 0; i < nline2; i++)
	{
		root1 = 0;
		while (fscanf(f2, "%d%c", &read, &ch))
		{
			if (!root1)
				root1 = read;
			else
				addEdge(g, root1, read, (double)1);
			if (ch == '\n')
				break;
		}
	}
	mvwaddstr(woof, getmaxy(woof) - 2, 2, "Press any key to continue...");
	wrefresh(woof);
	wgetch(woof);
	wclear(woof);
	wrefresh(woof);
	delwin(woof);
}
void func12(Graph g)
{
	WINDOW *wod;
	int i = 0, r = 0, output[100], min = 100;
	JRB root;
	jrb_traverse(root, g->vertices)
	{
		r = indegree(g, jval_i(root->key), output);
		if (r < min)
			min = r;
		i++;
	}
	wod = newwin(i + 4, 60, 10, getmaxx(stdscr) / 2);
	box(wod, 0, 0);
	mvwaddstr(wod, getmaxy(wod) - 2, 2, "Press any key to continue...");
	mvwprintw(wod, 2, 3, "Nhung page co it lien ket toi nhat la: ");
	mvwprintw(wod, 3, 3, "\t\t%-35s%-6s%s", "URL", "ID", "N");
	i = 0;
	jrb_traverse(root, g->vertices)
	{
		r = indegree(g, jval_i(root->key), output);
		if (r == min)
			mvwprintw(wod, 3 + ++i, 4, "%-45s %-7d%d", jval_s(root->val), jval_i(root->key), r);
	}
	wrefresh(wod);
	wgetch(wod);
	wclear(wod);
	wrefresh(wod);
	delwin(wod);
}
void func13(Graph g)
{
	WINDOW *wod;
	int i = 0, r = 0, output[100], max = 0;
	JRB root;
	jrb_traverse(root, g->vertices)
	{
		r = indegree(g, jval_i(root->key), output);
		if (r > max)
			max = r;
		i++;
	}
	wod = newwin(i + 4, 60, 10, getmaxx(stdscr) / 2);
	box(wod, 0, 0);
	mvwaddstr(wod, getmaxy(wod) - 2, 2, "Press any key to continue...");
	mvwprintw(wod, 2, 3, "Nhung page co nhieu lien ket toi nhat la: ");
	mvwprintw(wod, 3, 3, "\t\t%-35s%-6s%s", "URL", "ID", "N");
	i = 0;
	jrb_traverse(root, g->vertices)
	{
		r = indegree(g, jval_i(root->key), output);
		if (r == max)
			mvwprintw(wod, 3 + ++i, 4, "%-45s %-7d%d", jval_s(root->val), jval_i(root->key), r);
	}
	wrefresh(wod);
	wgetch(wod);
	wclear(wod);
	wrefresh(wod);
	delwin(wod);
}
void func1(Graph g)
{
	int chose;
	WINDOW *wow;
	wow = newwin(8, 60, 10, getmaxx(stdscr) / 2);
	FILE *fin1, *fin2;
	fin1 = fopen("webpages.txt", "r");
	fin2 = fopen("pageConnections.txt", "r");
	if (!fin1 || !fin2)
		return;
	do
	{
		chose = show_menu(wow, &chose1, 3, options1[0], options1[1], options1[2]);
		switch (chose)
		{
		case 1:
			func11(fin1, fin2, g);
			break;
		case 2:
			func12(g);
			break;
		case 3:
			func13(g);
			break;
		default:
			break;
		}
	} while (chose != ESC);
	fclose(fin1);
	fclose(fin2);
	wclear(wow);
	wrefresh(wow);
	delwin(wow);
}
JRB func2(Graph g)
{
	JRB rankpage, root;
	rankpage = make_jrb();
	int total, out[100], io[100], arr[100], r, n = 0;
	double rp = 0;
	WINDOW *wuw;
	jrb_traverse(root, g->vertices)
	{
		r = jval_i(root->key);
		total = indegree(g, r, io);
		rp = 0;
		for (int i = 0; i < total; i++)
		{
			out[i] = outdegree(g, io[i], arr);
			rp += 1 / (double)out[i];
		}
		jrb_insert_int(rankpage, r, new_jval_d(rp));
		n++;
	}
	wuw = newwin(n + 6, 62, 10, getmaxx(stdscr) / 2);
	box(wuw, 0, 0);
	mvwaddstr(wuw, getmaxy(wuw) - 2, 2, "Press any key to continue...");
	n = 0;
	mvwprintw(wuw, 2, 3, "\t\t\tURL\t\t\t       RANK");
	jrb_traverse(root, rankpage)
	{
		mvwprintw(wuw, 3 + n++, 3, "%-52s%.2f", getVertex(g, jval_i(root->key)) /*jval_i(root->key)*/, jval_d(root->val));
	}
	wrefresh(wuw);
	wgetch(wuw);
	wclear(wuw);
	wrefresh(wuw);
	delwin(wuw);
	return rankpage;
}
JRB func3(Graph g, JRB rankpage)
{
	JRB rp1, rp2, root, root1;
	rp1 = make_jrb();
	rp2 = rankpage;
	int total, r, n = 0, out[100], arr[100], io[100], nr = 10, ch = 0;
	double rp = 0;
	WINDOW *wuw;
	echo();
	curs_set(TRUE);
	do
	{

		wuw = newwin(6, 62, 10, getmaxx(stdscr) / 2);
		box(wuw, 0, 0);
		if (nr < 3)
		{
			curs_set(FALSE);
			mvwprintw(wuw, 2, 3, "So lan lap qua be");
			wrefresh(wuw);
			wgetch(wuw);
			curs_set(TRUE);
		}
		if (nr > 50)
		{
			curs_set(FALSE);
			mvwprintw(wuw, 2, 3, "So lan lap qua lon");
			wrefresh(wuw);
			wgetch(wuw);
			curs_set(TRUE);
		}
		mvwprintw(wuw, 2, 3, "Hay nhap so lap thuat toan: ");
		wrefresh(wuw);
		n = wscanw(wuw, "%d", &nr);
		wclear(wuw);
		wrefresh(wuw);
		delwin(wuw);
	} while (n <= 0 || nr < 3 || nr > 50);
	for (int i = 0; i < nr; i++)
	{
		n = 0;
		jrb_traverse(root, rp2)
		{
			r = jval_i(root->key);
			total = indegree(g, r, io);
			rp = 0;
			for (int j = 0; j < total; j++)
			{
				out[j] = outdegree(g, io[j], arr);
				rp += jval_d(jrb_find_int(rp2, io[j])->val) / (double)out[j];
			}
			root1 = jrb_find_int(rp1, r);
			if (root1)
				root1->val.d = rp;
			else
				jrb_insert_int(rp1, r, new_jval_d(rp));
			n++;
		}
		root = rp1;
		rp1 = rp2;
		rp2 = root;
	}
	echo();
	curs_set(FALSE);
	wuw = newwin(n + 6, 62, 10, getmaxx(stdscr) / 2);
	box(wuw, 0, 0);
	mvwaddstr(wuw, getmaxy(wuw) - 2, 2, "Press any key to continue...");
	n = 0;
	mvwprintw(wuw, 2, 3, "\t\t\tURL\t\t\t       RANK");
	jrb_traverse(root, rp2)
	{
		mvwprintw(wuw, 3 + n++, 3, "%-52s%.2lf", getVertex(g, jval_i(root->key)) /*jval_i(root->key)*/, jval_d(root->val));
	}
	wrefresh(wuw);
	wgetch(wuw);
	wclear(wuw);
	wrefresh(wuw);
	delwin(wuw);
	jrb_free_tree(rp1);
	return rp2;
}
void func41(Graph g)
{
	int i = 0, output[100];
	JRB root;
	WINDOW *wyw;
	wyw = newwin(10, 62, 10, getmaxx(stdscr) / 2);
	box(wyw, 0, 0);
	mvwaddstr(wyw, getmaxy(wyw) - 2, 2, "Press any key to continue...");
	mvwprintw(wyw, 2, 3, "\t\t\tURL\t\t\t       ");
	jrb_traverse(root, g->vertices)
	{
		if (!outdegree(g, jval_i(root->key), output))
		{
			mvwprintw(wyw, 3 + i++, 3, "%-52s", getVertex(g, jval_i(root->key)));
		}
	}
	wrefresh(wyw);
	wgetch(wyw);
	wclear(wyw);
	wrefresh(wyw);
	delwin(wyw);
}
void func42(Graph g)
{
	int i = 0, input[100];
	JRB root;
	WINDOW *wyw;
	wyw = newwin(10, 62, 10, getmaxx(stdscr) / 2);
	box(wyw, 0, 0);
	mvwaddstr(wyw, getmaxy(wyw) - 2, 2, "Press any key to continue...");
	mvwprintw(wyw, 2, 3, "\t\t\tURL\t\t\t       ");
	jrb_traverse(root, g->vertices)
	{
		if (!indegree(g, jval_i(root->key), input))
		{
			mvwprintw(wyw, 3 + i++, 3, "%-52s", getVertex(g, jval_i(root->key)));
		}
	}
	wrefresh(wyw);
	wgetch(wyw);
	wclear(wyw);
	wrefresh(wyw);
	delwin(wyw);
}
void func4(Graph g)
{
	int chose;
	WINDOW *wew;
	wew = newwin(7, 60, 10, getmaxx(stdscr) / 2);
	do
	{
		chose = show_menu(wew, &chose4, 2, options4[0], options4[1]);
		switch (chose)
		{
		case 1:
			func41(g);
			break;
		case 2:
			func42(g);
			break;
		default:
			break;
		}
	} while (chose != ESC);
	wclear(wew);
	wrefresh(wew);
	delwin(wew);
}

void func5(Graph g)
{
	WINDOW *w5;
	int id1 = 0, id2 = 0, length = 9, path[20], total, i = 0;
	w5 = newwin(10, 60, 10, getmaxx(stdscr) / 2);
	JRB root;
	echo();
	curs_set(TRUE);
	memset(path, 0, sizeof(path));
	do
	{
		box(w5, 0, 0);
		mvwaddstr(w5, 2, 3, "Hay nhap id cua webpage thu nhat: ");
		wscanw(w5, "%d", &id1);
		mvwaddstr(w5, 3, 3, "Hay nhap id cua webpage thu hai: ");
		wscanw(w5, "%d", &id2);
		wclear(w5);
		wrefresh(w5);
		if (!getVertex(g, id1) || !getVertex(g, id2))
			id1 = (id2 = 0);
		else
		{
			box(w5, 0, 0);
			mvwaddstr(w5, 2, 3, "Duong di ngan nhat giua hai web la: ");
			// jrb_traverse(root, g->vertices)
			// {
			// 	total = outdegree(g, jval_i(root->key), path);
			// 	if (total)
			// 	{
			// 		mvwprintw(w5, 3 + i++, 3, "%d %d", jval_i(root->key), total);
			// 	}
			// 	for (int j = 0; j < total; j++)
			// 	{
			// 		wprintw(w5, " %d", path[j]);
			// 	}
			// }
			wprintw(w5, "%d", (int)shortestPath(g, id1, id2, path, &length));
			// wprintw(w5, "%d", (int)dijkstra(g, id1, id2, path, &length));
			wrefresh(w5);
			noecho();
			curs_set(FALSE);
			wgetch(w5);
			wclear(w5);
			wrefresh(w5);
			delwin(w5);
		}
	} while (id1 == 0 || id2 == 0);
}
int main()
{
	Graph graph;
	graph = createGraph();
	JRB rankpage = NULL;
	initscr();
	initialise_colors();
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	echo();
	cbreak();
	int chose, do1 = FALSE, do2 = FALSE;
	WINDOW *wang;
	wang = newwin(10, 60, 10, getmaxx(stdscr) / 2 - 65);
	do
	{
		chose = show_menu(wang, &chose0, 5, options0[0], options0[1], options0[2], options0[3], options0[4]);
		mvwaddstr(wang, chose + 1, 52, "=>");
		wrefresh(wang);
		switch (chose)
		{
		case 1:
			func1(graph);
			do1 = TRUE;
			break;
		case 2:
			if (do1 == FALSE)
				break;
			rankpage = func2(graph);
			do2 = TRUE;
			break;
		case 3:
			if (do1 == FALSE || do2 == FALSE)
				break;
			rankpage = func3(graph, rankpage);
			break;
		case 4:
			if (do1 == FALSE)
				break;
			func4(graph);
			break;
		case 5:
			if (do1 == FALSE)
				break;
			func5(graph);
			break;
		default:
			break;
		}
	} while (chose != ESC);
	if (do2)
		jrb_free_tree(rankpage);
	delwin(wang);
	dropGraph(graph);
	endwin();
	return 0;
}