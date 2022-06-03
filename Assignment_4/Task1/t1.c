#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "t1.h"
#include "queue.h"

//Converts an int to a char
char int2char(int num)
{
    num = num + 'A';
    return (char)num;
}

// creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
Graph *create_graph(int num_nodes)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->N = num_nodes;

    graph->array = (AdjListHead *)malloc(num_nodes * sizeof(AdjListHead));
    for (int i = 0; i < num_nodes; i++)
    {
        graph->array[i].head = NULL;
    }
    return graph;
}

//Add a node at destination
AJList *addNewNode(int dest)
{
    AJList *newNode = malloc(sizeof(AJList));
    newNode->destination = dest;
    newNode->next = NULL;
    return newNode;
}

// adds a directed edge
void add_edge(Graph *g, int from, int to)
{
    struct AJList *newNode = addNewNode(to);
    newNode->next = g->array[from].head;
    g->array[from].head = newNode;
}

//implements breath first search and prints the results
void bfs(Graph *g, int origin)
{
    AJList *check_adj;
    int *visited = (int *)malloc(sizeof(int) * g->N);
    printf("BFS ");

    for (int i = 0; i < g->N; i++)
    {
        visited[i] = 0;
    }
    visited[origin] = 1;
    enqueue(origin);
    while (!isEmpty())
    {
        origin = frontData();

        printf(" %c ", int2char(origin));
        dequeue();
        for (check_adj = g->array[origin].head; check_adj; check_adj = check_adj->next)
        {
            if (!visited[check_adj->destination])
            {
                visited[check_adj->destination] = 1;
                enqueue(check_adj->destination);
            }
        }
    }
    free(visited);
    printf("\n");
}

//Utility Function for DFS to serach recursively
void dfs_visited(Graph *g, int v, int visited[])
{
    visited[v] = 1;
    printf(" %c ", int2char(v));

    AJList *check_adj;
    for (check_adj = g->array[v].head; check_adj; check_adj = check_adj->next)
    {
        if (!visited[check_adj->destination])
        {
            dfs_visited(g, check_adj->destination, visited);
        }
    }
}

//implements depth first search and prints the results
void dfs(Graph *g, int origin)
{
    int *visited = (int *)malloc(sizeof(int) * g->N);
    printf("DFS:");
    for (int i = 0; i < g->N; i++)
    {
        visited[i] = 0;
    }
    dfs_visited(g, origin, visited);
    free(visited);
    printf("\n");
}

void recursive_free(AJList *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    recursive_free(ptr->next);
    free(ptr);
}

void delete_graph(Graph *g)
{
    for (int i = 0; i < g->N; i++)
    {
        if (g->array[i].head != NULL)
        {
            recursive_free(g->array[i].head->next);
        }
    }
    for (int i = 0; i < g->N; i++)
    {
        free(g->array[i].head);
    }
    free(g->array);
    free(g);
}