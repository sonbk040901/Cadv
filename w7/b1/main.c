#include <stdio.h>
#include <stdlib.h>
#include "lbr/queue.h"
#define FALSE 0
#define TRUE 1
typedef struct _graph
{

    int *matrix;
    int sizemax;
} Graph;
Graph createGraph(int sizemax) //Cấp phát bộ nhớ cần thiết cho đồ thị
{
    Graph note;
    note.matrix = (int *)malloc(sizeof(int) * sizemax * sizemax);
    for (int i = 0; i < note.sizemax * note.sizemax; i++)
    {
        note.matrix[i] = 0;
    }
    note.sizemax = sizemax;
    return note;
}

void addEdge(Graph *graph, int v1, int v2) //Thêm 1 cạnh vào đồ thị
{
    graph->matrix[v1 * graph->sizemax + v2] = 1;
    graph->matrix[v2 * graph->sizemax + v1] = 1;
}

int adjacent(Graph graph, int v1, int v2) //Kiểm tra đỉnh v1 và v2 có kề nhau không?
{
    return graph.matrix[v1 * graph.sizemax + v2];
}

int getAdjacentVertices(Graph graph, int vertex, int *output) // trả về tập các đỉnh kề
{
    int n = 0;
    for (int i = 0; i < graph.sizemax; i++)
    {
        if (adjacent(graph, vertex, i))
        {
            output[i] = 1;
            n = 1;
        }
        else
            output[i] = 0;
    }
    return n;
}
void dropGraph(Graph graph) //Giải phóng bộ nhớ đã cấp cho đồ thị
{
    free(graph.matrix);
}
void traverse(Graph graph, int *visited, QUEUE *q)
{
    if (!isEmptyQ(q))
    {
        DataType node;
        node = DeQueue(q);
        int ctn;
        int output[graph.sizemax];
        ctn = getAdjacentVertices(graph, node.point, output);
        if (ctn)
        {
            for (int i = 0; i < graph.sizemax; i++)
            {
                if (output[i])
                {
                    if (!visited[i])
                    {
                        node.point = i;
                        visited[node.point] = TRUE;
                        printf("%5d", node.point);
                        EnQueue(q, node);
                    }
                }
            }
        }
        traverse(graph, visited, q);
    }
    else
    {
        free(q->Front);
        free(q);
    }
}
int main(int argc, char const *argv[])
{
    int sizemax;
    Graph graph;
    int *output;
    int n, p;
    printf("Hay nhap so nut co tren do thi: ");
    scanf("%d", &sizemax);
    graph = createGraph(sizemax);
    addEdge(&graph, 0, 1);
    addEdge(&graph, 0, 2);
    addEdge(&graph, 1, 2);
    addEdge(&graph, 4, 3);
    addEdge(&graph, 6, 3);
    addEdge(&graph, 1, 3);
    addEdge(&graph, 6, 5);
    output = (int *)malloc(sizeof(int) * graph.sizemax);
    scanf("%d", &p);
    // n = getAdjacentVertices(graph, p, output);
    // if (n == 0)
    // {
    //     printf("Khong co dinh ke cua nut %d\n", p);
    // }
    // else
    // {
    //     printf("\nCac dinh ke cua nut 1: \n");
    //     for (int i = 0; i < graph.sizemax; i++)
    //         output[i] ? printf("%5d", i) : 0;
    //     printf("\n");
    // }
    //

    QUEUE *queue = MakeNullQueue();
    int visited[graph.sizemax];
    for (int i = 0; i < graph.sizemax; i++)
    {
        visited[i] = FALSE;
    }
    DataType q;
    q.point = p;
    EnQueue(queue, q);
    visited[q.point] = TRUE;
    printf("%d", q.point);
    traverse(graph, visited, queue);
    printf("\n");
    free(output);
    dropGraph(graph);
    return 0;
}
