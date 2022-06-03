#include <stdlib.h>
#include <stdio.h>
#include "t2.h"

// converts an upper case character (A-Z) to a numerical value (between 0 and 25)
static int char2val(char c)
{
    if (c < 'A' || c > 'Z')
    {
        return -1;
    }

    return c - 'A';
}

int main()
{
    int num_nodes = 7;
    Graph *graph = create_graph(num_nodes);

    add_edge(graph, char2val('A'), char2val('B'), 1);
    add_edge(graph, char2val('A'), char2val('C'), 3);
    add_edge(graph, char2val('A'), char2val('F'), 10);
    add_edge(graph, char2val('B'), char2val('G'), 2);
    add_edge(graph, char2val('B'), char2val('D'), 7);
    add_edge(graph, char2val('B'), char2val('E'), 5);
    add_edge(graph, char2val('B'), char2val('C'), 1);
    add_edge(graph, char2val('C'), char2val('D'), 9);
    add_edge(graph, char2val('C'), char2val('E'), 3);
    add_edge(graph, char2val('D'), char2val('G'), 12);
    add_edge(graph, char2val('D'), char2val('F'), 1);
    add_edge(graph, char2val('D'), char2val('E'), 2);
    add_edge(graph, char2val('E'), char2val('F'), 2);

    dijkstra(graph, char2val('A'));

    delete_graph(graph);

    return 0;
}