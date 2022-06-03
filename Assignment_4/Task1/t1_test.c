#include <stdio.h>
#include <stdlib.h>
#include "t1.h"
//#include "queue.h"

// converts an upper case character (A-Z) to a numerical value (between 0 and 25)
static int char2val(char c)
{
    if (c < 'A' || c > 'Z')
    {
        return -1;
    }

    return c - 'A';
}

void printGraph(struct Graph *graph)
{
    int v;
    for (v = 0; v < graph->N; ++v)
    {
        struct AJList *pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {

            printf("-> %d", pCrawl->destination);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

int main()
{
    int num_nodes = 7;
    Graph *graph = create_graph(num_nodes);

    // add_edge(graph, char2val('A'), char2val('B'));
    // add_edge(graph, char2val('B'), char2val('C'));
    // add_edge(graph, char2val('B'), char2val('E'));
    // add_edge(graph, char2val('C'), char2val('D'));
    // add_edge(graph, char2val('D'), char2val('E'));
    // add_edge(graph, char2val('D'), char2val('F'));
    // add_edge(graph, char2val('E'), char2val('F'));
    // add_edge(graph, char2val('F'), char2val('G'));

    add_edge(graph, char2val('A'), char2val('E'));
    add_edge(graph, char2val('B'), char2val('D'));
    add_edge(graph, char2val('A'), char2val('D'));
    add_edge(graph, char2val('A'), char2val('B'));
    add_edge(graph, char2val('B'), char2val('C'));
    add_edge(graph, char2val('C'), char2val('B'));
    add_edge(graph, char2val('E'), char2val('D'));
    add_edge(graph, char2val('D'), char2val('F'));
    add_edge(graph, char2val('F'), char2val('C'));
    add_edge(graph, char2val('C'), char2val('D'));
    //printGraph(graph);

    dfs(graph, char2val('A'));
    bfs(graph, char2val('A'));

    // printGraph(graph);
    delete_graph(graph);
    return 0;
}