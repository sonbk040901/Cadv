#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lbr/jrb.h"
#include "lbr/dllist.h"

typedef JRB Graph;

Graph createGraph();
void addEdge(Graph graph, int v1, int v2);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int vertex, int *output);
void dropGraph(Graph graph);

void BFS(Graph graph, int start, int stop, void (*func)(int));
void DFS(Graph graph, int start, int stop, void (*func)(int));

void printVertex(int v)
{
    printf("%4d", v);
}

int main()
{
    Graph g = createGraph();
    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4); //
    addEdge(g, 2, 3);
    addEdge(g, 2, 4);
    addEdge(g, 4, 5);
    printf("\nBFS: start from node 1 to   4 : ");
    BFS(g, 1, 4, printVertex);
    printf("\nBFS: start from node 1 to all : ");
    BFS(g, 1, -1, printVertex);
    printf("\nDFS: start from node 1 to   4 : ");
    DFS(g, 2, -1, printVertex);
    printf("\nDFS: start from node 1 to all : ");
    DFS(g, 1, -1, printVertex);
    dropGraph(g);
    printf("\n");
}

Graph createGraph()
{
    return make_jrb();
}

void addEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    if (!adjacent(graph, v1, v2))
    {
        node = jrb_find_int(graph, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
    }
    if (!adjacent(graph, v2, v1))
    {
        node = jrb_find_int(graph, v2);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph, v2, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v1, new_jval_i(1));
    }
}

int adjacent(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph, v1);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int getAdjacentVertices(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph);
}

void BFS(Graph graph, int start, int stop, void (*func)(int))
{
    int visited[1000] = {};
    int n, output[100], i, u, v;
    Dllist node, queue;

    queue = new_dllist();
    dll_append(queue, new_jval_i(start));

    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        u = jval_i(node->val);
        dll_delete_node(node);
        if (!visited[u])
        {
            func(u);
            visited[u] = 1;
            if (u == stop)
                break;
            n = getAdjacentVertices(graph, u, output);
            for (i = 0; i < n; i++)
            {
                v = output[i];
                if (!visited[v])
                    dll_append(queue, new_jval_i(v));
            }
        }
    }
}

void DFS(Graph graph, int start, int stop, void (*func)(int))
{
    int visited[1000] = {};
    int n, output[100], i, u, v;
    Dllist node, stack;

    stack = new_dllist();
    dll_append(stack, new_jval_i(start));

    while (!dll_empty(stack))
    {
        node = dll_last(stack);
        u = jval_i(node->val);
        dll_delete_node(node);
        if (!visited[u])
        {
            func(u);
            visited[u] = 1;
            if (u == stop)
                return;
            n = getAdjacentVertices(graph, u, output);
            for (i = 0; i < n; i++)
            {
                v = output[i];
                if (!visited[v])
                    dll_append(stack, new_jval_i(v));
            }
        }
    }
}
