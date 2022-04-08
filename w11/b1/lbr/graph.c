#include <stdio.h>
#include "dllist.h"
#include "jrb.h"
#include "jval.h"
#include <string.h>
#include "graph.h"
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