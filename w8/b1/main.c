#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "dllist.h"
typedef struct _graphics
{
    JRB edges;
    JRB vertices;
} * Graph;
Graph createGraph()
{
    Graph graph;
    graph = (Graph)malloc(sizeof(struct _graphics));
    graph->edges = make_jrb();
    graph->vertices = make_jrb();
    return graph;
}
void addVertex(Graph graph, int id, char *name)
{
    JRB node = make_jrb();
    jrb_insert_int(graph->vertices, id, new_jval_v((void *)name));
    jrb_insert_int(graph->edges, id, new_jval_v(node));
}
char *getVertex(Graph graph, int id)
{
    return (char *)jval_v(jrb_find_int(graph->vertices, id)->val);
}
void addEdge(Graph graph, int v1, int v2)
{
    JRB node1 = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    jrb_insert_int(node1, v2, new_jval_i(1));
}
int hasEdge(Graph graph, int v1, int v2)
{
    JRB node = jrb_find_int(graph->edges, v1);
    if (!node)
    {
        return 0;
    }
    else
    {
        JRB tree = jrb_find_int((JRB)jval_v(node->val), v2);
        if (!tree)
        {
            return 0;
        }
        return 1;
    }
}
int indegree(Graph graph, int v, int *output)
{
    JRB root;
    JRB find;
    int total = 0;
    jrb_traverse(root, graph->edges)
    {
        find = jrb_find_int((JRB)jval_v(root->val), v);
        if (find)
            *(output + total++) = jval_i(root->key);
    }
    return total;
}
int outdegree(Graph graph, int v, int *output)
{
    JRB root, find;
    int total = 0;
    find = jrb_find_int(graph->edges, v);
    if (find)
    {
        JRB tree = (JRB)jval_v(find->val);
        jrb_traverse(root, tree)
        {
            *(output + total++) = jval_i(root->key);
        }
    }
    return total;
}
int DAG(Graph graph)
{
    int visited[1000];
    int n, output[100], i, u, v, start;
    Dllist node, stack;
    JRB vertex;

    jrb_traverse(vertex, graph->vertices)
    {
        memset(visited, 0, sizeof(visited));

        start = jval_i(vertex->key);
        stack = new_dllist();
        dll_append(stack, new_jval_i(start));

        while (!dll_empty(stack))
        {
            node = dll_last(stack);
            u = jval_i(node->val);
            dll_delete_node(node);
            if (!visited[u])
            {
                visited[u] = 1;
                n = outdegree(graph, u, output);
                for (i = 0; i < n; i++)
                {
                    v = output[i];
                    if (v == start)
                        return 0;
                    if (!visited[v])
                        dll_append(stack, new_jval_i(v));
                }
            }
        }
    }
    return 1;
}
void dropGraph(Graph graph)
{
    JRB root;
    jrb_traverse(root, graph->edges)
    {
        jrb_free_tree(root);
    }
    jrb_free_tree(graph->vertices);
}
int main(int argc, char const *argv[])
{
    Graph graph = createGraph();
    addVertex(graph, 0, "V0");
    addVertex(graph, 1, "V1");
    addVertex(graph, 2, "V2");
    // addVertex(graph, 3, "V3");
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 3);
    // addEdge(graph, 2, 0);
    // addEdge(graph, 1, 3);
    if (DAG(graph))
        printf("The graph is acycle\n");
    else
        printf("Have cycles in the graph\n");
    return 0;
}
