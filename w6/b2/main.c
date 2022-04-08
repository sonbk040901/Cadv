#include "jrb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct jrb_node *Graph;
Graph createGraph() //Cấp phát bộ nhớ cần thiết cho đồ thị
{
    Graph note = (Graph)make_jrb();
    return note;
}

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
        JRB tree = (JRB)jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
}
void addEdge(Graph graph, int v1, int v2)
{
    addEdge1(graph, v1, v2);
    addEdge1(graph, v2, v1);
}
int adjacent(Graph graph, int v1, int v2) //Kiểm tra đỉnh v1 và v2 có kề nhau không?
{
    JRB root = jrb_find_int((JRB)jval_v(jrb_find_int(graph, v1)->val), v2);
    return root ? jval_i(root->val) : 0;
}

void getAdjacentVertices(Graph graph, int vertex) // trả về tập các đỉnh kề
{
    JRB root;
    JRB search = jrb_find_int(graph, vertex);
    if (search)
    {
        JRB tree = (JRB)jval_v(search->val);
        printf("Cac dinh ke cua nut %d la: ", vertex);
        jrb_traverse(root, tree)
            printf("%-5d", jval_i(root->key));
        printf("\n");
    }
    else
    {
        printf("Khong co dinh ke nut %d\n", vertex);
    }
}
void dropGraph(Graph graph) //Giải phóng bộ nhớ đã cấp cho đồ thị
{
    JRB root;
    jrb_traverse(root, graph)
        jrb_free_tree((JRB)jval_v(root->val));
}
int main(int argc, char const *argv[])
{
    Graph graph;
    int n;
    graph = createGraph();
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    scanf("%d", &n);
    getAdjacentVertices(graph, n);
    dropGraph(graph);
    return 0;
}
