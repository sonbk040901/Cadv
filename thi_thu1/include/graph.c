#include <stdio.h>
#include "dllist.h"
#include "jrb.h"
#include "jval.h"
#include <string.h>
#include <curses.h>
#include "graph.h"
void setParent(int u, int v, int *parent)
{
    parent[u] = v;
}
double dijkstra(Graph g, int s, int t, int *path, int *length)
{
    double *d = malloc(10000 * sizeof(double)), min, w, total; //khoang cach tu diem dau
    for (int i = 0; i < 10000; i++)
    {
        d[i] = INFINITIVE_VALUE;
    }
    d[s] = 0;
    int *diemtruoc = (int *)malloc(10000 * sizeof(int));
    int *check = (int *)calloc(10000, sizeof(int)); //visited
    Dllist queue = new_dllist(), node, ptr;
    dll_append(queue, new_jval_i(s));
    int u, v;
    int n;
    int output[1000];
    JRB tmp, tmp1, tmp2;
    while (!dll_empty(queue))
    {
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if (min > d[u])
            {
                min = d[u];
                node = ptr;
            }
        }
        u = jval_i(node->val);
        dll_delete_node(node);

        if (u == t)
            break;

        n = outdegree(g, u, output);
        tmp = jrb_find_int(g->edges, u);
        if (check[u] == 0)
        {
            for (int i = 0; i < n; i++)
            {
                v = output[i];
                w = getEdgeValue(g, u, v);
                dll_append(queue, new_jval_i(output[i]));
                if (d[output[i]] == 1000)
                {
                    d[v] = w + d[u];
                    diemtruoc[v] = u;
                }
                else if (w + d[u] < d[v])
                {
                    d[v] = w + d[u];
                    diemtruoc[v] = u;
                }
            }
        }
        check[u] = 1;
    }
    total = d[t];
    if (total != INFINITIVE_VALUE)
    {
        int arr[100];
        int x = 0;
        int m = t;
        while (m != s)
        {
            arr[x++] = m;
            m = diemtruoc[m];
        }
        arr[x++] = s;
        *length = 0;
        for (int i = x - 1; i >= 0; i--)
        {
            path[(*length)++] = arr[i];
            //printf("%s ", getVertex(g, arr[i]));
        }
    }
    return total;
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
    memset(parent, 0, sizeof(parent));
    jrb_insert_int(PQ, 0, new_jval_i(s));
    weight[s] = 0;
    while (!jrb_empty(PQ))
    {
        v = jval_i(jrb_first(PQ)->val);
        jrb_delete_node(jrb_first(PQ));
        if (!visited[v])
        {
            visited[v] = TRUE;
            printw("duyet %d...\n", v);
            clear();
            n = outdegree(graph, v, output);
            for (int i = 0; i < n; i++)
            {
                o = output[i];
                d = weight[v] + getEdgeValue(graph, v, o);
                if (d < weight[o])
                {
                    weight[o] = d;
                    setParent(o, v, parent);
                    printw("weight[%d] = %g\n", o, weight[o]);
                    clear();
                }
                if (o == t) //maybe error is here
                {
                    jrb_free_tree(PQ);
                    break;
                }
                if (!visited[o] && o != v)
                {
                    jrb_insert_int(PQ, weight[o], new_jval_i(o));
                }
            }
        }
    }
    j = 0;
    for (int i = t; parent[i] != s; i = parent[i])
    {
        path[j++] = parent[i];
    }
    *length = j;
    jrb_free_tree(PQ);
    printw("%g", weight[t]);
    return weight[t];
}
Graph createGraph()
{
    Graph graph;
    graph = (Graph)malloc(sizeof(struct _graphics));
    graph->edges = make_jrb();
    graph->vertices = make_jrb();
    graph->id = make_jrb();
    return graph;
}
int addVertex(Graph graph, int id, char *name /*Note: strdup*/)
{
    if (!jrb_find_str(graph->id, name))
    {
        JRB node = make_jrb();
        jrb_insert_str(graph->id, name, new_jval_i(id));
        jrb_insert_int(graph->vertices, id, new_jval_s(name));
        jrb_insert_int(graph->edges, id, new_jval_v(node));
        return 1;
    }
    return 0;
}
char *getVertex(Graph graph, int id)
{
    JRB find = jrb_find_int(graph->vertices, id);
    if (id == -1 || !find)
        return NULL;
    return (char *)jval_s(find->val);
}
int getId(Graph g, char *name)
{
    JRB node = jrb_find_str(g->id, name);
    if (node)
    {
        return jval_i(node->val);
    }
    else
        return INFINITIVE_VALUE;
}
void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node = (JRB)jval_v(jrb_find_int(graph->edges, v1)->val);
    if (!jrb_find_int(node, v2))
        jrb_insert_int(node, v2, new_jval_d(weight));
}
int hasVertex(Graph g, char *name)
{
    int find = getId(g, name);
    if (find != INFINITIVE_VALUE)
    {
        return 1;
    }
    return 0;
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
    JRB rINFINITIVE_VALUEt;
    JRB find;
    int total = 0;
    jrb_traverse(rINFINITIVE_VALUEt, graph->edges)
    {
        find = jrb_find_int((JRB)jval_v(rINFINITIVE_VALUEt->val), v);
        if (find)
            *(output + total++) = jval_i(rINFINITIVE_VALUEt->key);
    }
    return total;
}
int outdegree(Graph graph, int v, int *output)
{
    JRB rINFINITIVE_VALUEt, find;
    int total = 0;
    find = jrb_find_int(graph->edges, v);
    if (find)
    {
        JRB tree = (JRB)jval_v(find->val);
        jrb_traverse(rINFINITIVE_VALUEt, tree)
        {
            *(output + total++) = jval_i(rINFINITIVE_VALUEt->key);
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
    JRB rINFINITIVE_VALUEt, rINFINITIVE_VALUEt1, node1;
    JRB node = graph->edges;
    jrb_traverse(rINFINITIVE_VALUEt, node)
    {
        jrb_free_tree((JRB)jval_v(rINFINITIVE_VALUEt->val));
    }
    jrb_free_tree(node);
    node = graph->vertices;
    jrb_traverse(rINFINITIVE_VALUEt, node)
    {
        free(jval_s(rINFINITIVE_VALUEt->val));
    }
    jrb_free_tree(graph->vertices);
    jrb_free_tree(graph->id);
}