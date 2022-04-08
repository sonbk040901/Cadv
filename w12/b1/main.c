#include <stdio.h>
#include "lbr/dllist.h"
#include "lbr/jrb.h"
#include "lbr/jval.h"
#include <string.h>
#include "lbr/graph.h"
#include <stdlib.h>
#define MAX_CHAR 256
typedef struct
{
    Graph graph;
    JRB root;
} HuffmanTree;
typedef struct
{
    int size;
    char bits[20];
} Coding;
typedef struct
{
    int size;
    int *node;
} HuffmanTreeArray;
HuffmanTree makeHuffman(char *, int, int *);
void get_code(HuffmanTree, Coding *, int *);
int add_to_queue(Graph, JRB, int *);
int Compact(Graph, JRB, int);
void fix_code(Coding *);
int get_bit(Graph, int, int);
void free_huf(HuffmanTree *);
void add_huffman_char(char ch, Coding htable[], char *huffman, int *nbit);
int compress(char *buffer, char *huffman, Coding htable[]);
void set_bit(char *, int, int);

HuffmanTreeArray tree_to_array(HuffmanTree htree, int *);
HuffmanTree makeHuffmanF(FILE *fin, int *);
void compressFile(char *in, char *out);
void set_huffman_tree_array(Graph g, int, HuffmanTreeArray, int);
int main(int argc, char const *argv[])
{
    char huffman[1000];
    Coding htable[MAX_CHAR];
    HuffmanTree htree;
    int nbit, bytes;
    int time[MAX_CHAR];
    char buffer[MAX_CHAR] = "ledun\nso\t";
    // fgets(buffer, MAX_CHAR, stdin);
    // buffer[strlen(buffer) - 1] = 0;
    // htree = makeHuffman(buffer, (int)strlen(buffer), time);
    // get_code(htree, htable, time);
    // nbit = compress(buffer, huffman, htable);
    // for (int i = 0; i < (nbit / 8) + (nbit % 8 ? 1 : 0); i++)
    // {
    //     printf("%d\t", huffman[i]);
    // }
    // printf("%d\n", nbit);
    compressFile("main.txt", "zip");
    // tree_to_array(htree, time);
    // free_huf(&htree);
    return 0;
}
HuffmanTree makeHuffmanF(FILE *fin, int time[])
{
    int i;
    while ((i = fgetc(fin)) != -1)
        time[i]++;
    HuffmanTree tree;
    tree.graph = createGraph();
    tree.root = make_jrb();
    Compact(tree.graph, tree.root, add_to_queue(tree.graph, tree.root, time));
    return tree;
}
void compressFile(char *in, char *out) //?????
{
    FILE *fin, *fout;
    HuffmanTree htree;
    int time[MAX_CHAR], nbit = 0, i, sum = 0, sizecode = 0;
    Coding htable[MAX_CHAR];
    char huffman[100000], ch, *code;
    fin = fopen(in, "rb");
    if (!fin)
    {
        perror("File not found!1\n");
        return;
    }
    fout = fopen(out, "w+b");
    if (!fout)
    {
        perror("File not found!2\n");
        return;
    }
    htree = makeHuffmanF(fin, time);
    get_code(htree, htable, time);
    fseek(fin, 0, SEEK_SET);
    while ((i = fgetc(fin)) != -1)
        add_huffman_char(i, htable, huffman, &nbit);
    fwrite("HM", 2, 1, fout);
    for (i = 0; i < MAX_CHAR; i++)
        if (time[i])
        {
            sum++;
            sizecode += htable[i].size;
        }
    fwrite(&sum, sizeof(int), 1, fout);
    code = (char *)malloc(sizeof(char) * (sizecode / 8 + (sizecode % 8) ? 1 : 0));
    sizecode = 0;
    for (i = 0; i < MAX_CHAR; i++)
        if (htable[i].size)
        {
            ch = i;
            fwrite(&ch, 1, 1, fout);
            add_huffman_char(i, htable, code, &sizecode);
        }
    fwrite(code, (sizecode / 8 + (sizecode % 8) ? 1 : 0), 1, fout);
    fwrite(&nbit, sizeof(int), 1, fout);
    fwrite(huffman, (nbit / 8 + (nbit % 8) ? 1 : 0), 1, fout);
    free_huf(&htree);
    free(code);
    fclose(fin);
    fclose(fout);
}
void set_huffman_tree_array(Graph g, int start, HuffmanTreeArray arr, int index)
{
    int out[2];
    if (start < 256)
    {
        arr.node[index] = start;
    }
    else
        arr.node[index] = -1;
    if (outdegree(g, start, out))
    {
        set_huffman_tree_array(g, out[0], arr, index * 2 + 1);
        set_huffman_tree_array(g, out[1], arr, index * 2 + 2);
    }
}
HuffmanTreeArray tree_to_array(HuffmanTree htree, int time[])
{
    Coding htable[MAX_CHAR];
    int start = jval_i(jrb_last(htree.graph->edges)->key);
    HuffmanTreeArray htarray;
    int maxlen = 0;
    get_code(htree, htable, time);
    for (int i = 0; i < MAX_CHAR; i++)
        maxlen = htable[i].size > maxlen ? htable[i].size : maxlen;
    int size = 1, exp2 = 1;
    for (int i = 1; i <= maxlen; i++)
    {
        exp2 *= 2;
        size += exp2;
    }
    htarray.node = (int *)malloc(sizeof(int) * size);
    memset(htarray.node, 0, size * sizeof(int));
    htarray.size = size;
    set_huffman_tree_array(htree.graph, start, htarray, 0);
    // for (int i = 0; i < size; i++)
    // {
    //     printf("%d\t", htarray.node[i]);
    // }
    free(htarray.node);
}
int compress(char *buffer, char *huffman, Coding htable[])
{
    int nbit = 0;
    for (int i = 0; i < strlen(buffer); i++)
        add_huffman_char(buffer[i], htable, huffman, &nbit);
    return nbit;
}
void add_huffman_char(char ch, Coding htable[], char *huffman, int *nbit)
{
    for (int i = 0; i < htable[ch].size; i++)
        set_bit(huffman, (*nbit)++, htable[ch].bits[i]);
}
void set_bit(char *huf, int nbit, int bit_val)
{
    int byte = nbit / 8, bit = nbit % 8;
    if (bit_val - 48)
        huf[byte] |= (1 << (7 - bit));
    else
        huf[byte] &= ~(1 << (7 - bit));
}
void free_huf(HuffmanTree *htree) //done
{
    dropGraph(htree->graph);
    jrb_free_tree(htree->root);
}
int add_to_queue(Graph g, JRB root, int time[]) //done
{
    int j = 0;
    char s[1];
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (time[i])
        {
            s[0] = i;
            j++;
            addVertex(g, i, strdup(s));
            jrb_insert_int(root, time[i], new_jval_i(i));
        }
    }
    return j;
}
int get_bit(Graph g, int v1, int v2) //done
{
    return (int)jval_d(jrb_find_int((JRB)jval_v(jrb_find_int(g->edges, v1)->val), v2)->val);
}
void fix_code(Coding tb[]) //done
{
    int temp;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (tb[i].size)
            for (int j = 0; j < tb[i].size / 2; j++)
            {
                temp = tb[i].bits[j];
                tb[i].bits[j] = tb[i].bits[tb[i].size - j - 1];
                tb[i].bits[tb[i].size - j - 1] = temp;
            }
    }
}
int Compact(Graph g, JRB root, int n) //done
{
    int j = 0;
    JRB node;
    while (j < n - 1)
    {
        j++;
        node = jrb_first(root);
        jrb_insert_int(root, jval_i(node->key) + jval_i(jrb_next(node)->key), new_jval_i(j + 255));
        addVertex(g, j + 255, "\0");
        addEdge(g, j + 255, jval_i(node->val), 0);
        addEdge(g, j + 255, jval_i(jrb_next(node)->val), 1);
        jrb_delete_node(jrb_next(node));
        jrb_delete_node(node);
    }
}
void get_code(HuffmanTree tree, Coding codeTable[], int time[]) //done
{
    int input[10], begin;
    Graph g = tree.graph;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        codeTable[i].size = 0;
        if (time[i])
        {
            begin = i;
            while (indegree(g, begin, input))
            {
                sprintf(codeTable[i].bits + codeTable[i].size++, "%d", get_bit(g, input[0], begin));
                begin = input[0];
            }
            codeTable[i].bits[codeTable[i].size] = 0;
        }
    }
    fix_code(codeTable);
    puts("Code:");
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (codeTable[i].size)
            printf("%c%d: %s\n", i, codeTable[i].size, codeTable[i].bits);
    }
}
HuffmanTree makeHuffman(char *buffer, int size, int time[]) //done
{
    memset(time, 0, sizeof(time));
    for (int i = 0; i < size; i++)
    {
        time[buffer[i]]++;
    }
    HuffmanTree tree;
    tree.graph = createGraph();
    tree.root = make_jrb();
    Compact(tree.graph, tree.root, add_to_queue(tree.graph, tree.root, time));
    return tree;
}