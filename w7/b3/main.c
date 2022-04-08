#include <string.h>
#include "jrb.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct jrb_node *Graph;
typedef struct _stations
{
    FILE *f;
    char *nameOfFile;
    int numberOfStations;
    char **nameOfStation;
    int numberOfLines;
    char **nameOfLine;
    Graph graph;
} * STS;

//Cấp phát bộ nhớ cần thiết cho đồ thị
Graph createGraph()
{
    Graph note = (Graph)make_jrb();
    return note;
}
//Lay data tu file
int convert_data_from_file(STS s)
{
    char line[256];
    s->graph = createGraph();
    s->f = fopen(s->nameOfFile, "r");
    if (!s->f)
        return 0;
    fscanf(s->f, "%s\n", line);
    if (strcmp(line, "[STATIONS]"))
    {
        return 0;
    }

    for (s->numberOfStations = 0; strcmp(fgets(line, 256, s->f), "[LINES]\n"); s->numberOfStations++)
    {
        line[strlen(line) - 1] = '\0';
        s->nameOfStation = (char **)realloc(s->nameOfStation, (s->numberOfStations + 1) * sizeof(char *));
        s->nameOfStation[s->numberOfStations] = strdup(strstr(line, "=") + 2);
    }
    // for (int i = 0; i < s->numberOfStations; i++)
    // {
    //     printf("%s", s->nameOfStation;[i]);
    // }
    for (s->numberOfLines = 0; !feof(s->f) && fgets(line, 256, s->f); s->numberOfLines++)
    {
        s->nameOfLine = (char **)realloc(s->nameOfLine, (s->numberOfLines + 1) * sizeof(char *));
        line[strlen(line) - 1] = 0;
        s->nameOfLine[s->numberOfLines] = strdup(strstr(line, "=") + 2);
    }
    return 1;
}
void dropGraph(Graph graph) //Giải phóng bộ nhớ đã cấp cho đồ thị
{
    JRB root;
    jrb_traverse(root, graph)
        jrb_free_tree((JRB)jval_v(root->val));
    jrb_free_tree(graph);
}
//Giai phong bo nho
void free_stations(STS station)
{
    fclose(station->f);
    free(station->nameOfFile);
    for (int i = 0; i < station->numberOfLines; i++)
    {
        free(station->nameOfLine[i]);
    }
    for (int i = 0; i < station->numberOfStations; i++)
    {
        free(station->nameOfStation[i]);
    }
    free(station->nameOfLine);
    free(station->nameOfStation);
    dropGraph(station->graph);
    free(station);
}
/*********Them canh cho do thi*************/

void addEdge1(Graph graph, int v1, int v2) //Thêm 1 cạnh vào đồ thị
{
    JRB node = jrb_find_int(graph, v1);
    if (node == NULL)
    {
        JRB tree = make_jrb();
        jrb_insert_int(graph, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
    else
    {
        JRB buf = jrb_find_int((JRB)jval_v(node->val), v2);
        if (!buf)
        {
            JRB tree = (JRB)jval_v(node->val);
            jrb_insert_int(tree, v2, new_jval_i(1));
        }
    }
}
void add_edge(STS s, int v1, int v2)
{
    addEdge1(s->graph, v1, v2);
    addEdge1(s->graph, v2, v1);
}
void add_edge_of_a_line_to_Graph(STS s, int index)
{
    char *buf1, *buf2;
    buf2 = strtok(s->nameOfLine[index], " ");
    for (int i = 0; i < 4; i++) //Bai nay chi dung cho mot tuyen co 5 ben
    {
        buf1 = buf2;
        buf2 = strtok(NULL, " ");
        add_edge(s, atoi(buf1 + 1), atoi(buf2 + 1));
    }
}
void add_all_of_edge(STS s)
{
    for (int i = 0; i < s->numberOfLines; i++)
    {
        add_edge_of_a_line_to_Graph(s, i);
    }
}
/**********************************************/
void getAdjacentVertices(STS s, int vertex) // trả về tập các đỉnh kề
{
    JRB root;
    JRB graph = s->graph;
    JRB search = jrb_find_int(graph, vertex);
    if (search)
    {
        JRB tree = (JRB)jval_v(search->val);
        printf("Cac ben gan ben nay la: ");
        jrb_traverse(root, tree)
            printf("%s \t", s->nameOfStation[jval_i(root->key) - 1]);
        printf("\n");
    }
    else
    {
        printf("Khong co dinh ke nut %d\n", vertex);
    }
}
//Ham main
int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        return 1;
    }
    int i;
    char search[256];
    STS station;
    station = (STS)malloc(sizeof(struct _stations));
    station->nameOfFile = strdup(argv[1]);
    //Lay data tu file
    if (!convert_data_from_file(station))
    {
        printf("Khong the lay data\n");
        free(station);
        return 1;
    }
    add_all_of_edge(station);
    printf("Hay nhap ten cua ga ma ban muon tim: ");
    fgets(search, 256, stdin);
    search[strlen(search) - 1] = '\0';
    for (i = 0; i < station->numberOfStations; i++)
    {
        if (!strcmp(search, station->nameOfStation[i]))
        {
            break;
        }
    }
    if (i != station->numberOfStations)
    {
        getAdjacentVertices(station, i + 1);
    }
    else
    {
        printf("Khong ton tai ga nay\n");
    }
    free_stations(station);
    return 0;
}