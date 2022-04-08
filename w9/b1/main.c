#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lbr/jrb.h"
#include "lbr/dllist.h"
#define INFINITIVE_VALUE 10000000
#define TRUE 1
#define FALSE 0
typedef struct _graphics
{
    JRB edges;
    JRB vertices;
} * Graph;
Graph createGraph();
void addVertex(Graph, int, char *);
char *getVertex(Graph, int);
void addEdge(Graph, int, int, double);
int hasEdge(Graph, int, int);
double getEdgeValue(Graph, int, int);
int indegree(Graph, int, int *);
int outdegree(Graph, int, int *);
int DAG(Graph);
void dropGraph(Graph);
void setParent(int, int, int *);
double shortestPath(Graph, int, int, int *, int *);
int main(int argc, char const *argv[])
{
    int path[100], length;
    Graph graph = createGraph();
    addVertex(graph, 0, "V0");
    addVertex(graph, 2, "V2");
    addVertex(graph, 3, "V3");
    addVertex(graph, 6, "V6");
    addVertex(graph, 7, "V7");
    addVertex(graph, 5, "V5");
    addVertex(graph, 4, "V4");
    addVertex(graph, 10, "V10");
    addEdge(graph, 0, 2, 9);
    addEdge(graph, 0, 6, 14);
    addEdge(graph, 0, 7, 15);
    addEdge(graph, 2, 3, 24);
    addEdge(graph, 6, 3, 18);
    addEdge(graph, 6, 5, 30);
    addEdge(graph, 6, 7, 5);
    addEdge(graph, 7, 5, 20);
    addEdge(graph, 7, 10, 44);
    addEdge(graph, 5, 4, 11);
    addEdge(graph, 5, 10, 16);
    addEdge(graph, 3, 5, 2);
    addEdge(graph, 3, 10, 19);
    addEdge(graph, 4, 3, 6);
    addEdge(graph, 4, 10, 6);
    // int output[100];
    // int n = outdegree(graph, 3, output);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d\t", output[i]);
    // }
    printf("%g\n", shortestPath(graph, 0, 10, path, &length));
    // for (int i = length - 1; i >= 0; i++)
    // {
    //     printf("%d\t", path[i]);
    // }
    printf("\n%d", length);
    // if (DAG(graph))
    //     printf("The graph is acycle\n");
    // else
    //     printf("Have cycles in the graph\n");
    dropGraph(graph);
    return 0;
}
void setParent(int u, int v, int *parent)
{
    parent[u] = v;
}
double shortestPath(Graph graph, int s, int t, int path[], int *length)
{
    JRB PQ;
    PQ = make_jrb();
    int v, n, j, o;
    int visited[100], output[100], parent[100];
    double weight[100], d;
    for (int i = 0; i < 100; i++)
    {
        weight[i] = (double)INFINITIVE_VALUE;
    }
    memset(visited, 0, sizeof(visited));
    jrb_insert_int(PQ, 0, new_jval_i(s));
    weight[s] = 0;
    while (!jrb_empty(PQ))
    {
        v = jval_i(jrb_first(PQ)->val);
        jrb_delete_node(jrb_first(PQ));
        if (!visited[v])
        {
            printf("duyet %d...\n", v);
            n = outdegree(graph, v, output);
            for (int i = 0; i < n; i++)
            {
                o = output[i];
                d = weight[v] + getEdgeValue(graph, v, o);
                if (d < weight[o])
                {
                    weight[o] = d;
                    setParent(o, v, parent);
                    printf("weight[%d] = %g\n", o, weight[o]);
                }
                if (!visited[o])
                {
                    jrb_insert_int(PQ, weight[o], new_jval_i(o));
                }
            }
        }
        visited[v] = TRUE;
    }
    j = 0;
    for (int i = t; parent[i] != s; i = parent[i])
    {
        path[j++] = parent[i];
    }
    *length = j;
    free(PQ);
    return weight[t];
}
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
void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node1 = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    jrb_insert_int(node1, v2, new_jval_d(weight));
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
double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    JRB find = jrb_find_int(node, v2);
    if (find)
    {
        return jval_d(find->val);
    }
    return INFINITIVE_VALUE;
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
    JRB node = graph->edges;
    jrb_traverse(root, node)
    {
        jrb_free_tree((JRB)jval_v(root->val));
    }
    jrb_free_tree(graph->vertices);
}