#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "t2.h"

//Converts an int to a char
char int2char(int num)
{
    num = num + 'A';
    return (char)num;
}

// creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
Graph *create_graph(int num_nodes)
{
    Graph *g = malloc(sizeof(Graph));
    g->Num_nodes = num_nodes;
    for (int i = 0; i < g->Num_nodes; i++)
    {
        for (int j = 0; j < g->Num_nodes; j++)
        {
            g->Arr[i][j] = 0;
        }
    }
    return g;
}

// adds an undirected weighted edge between from and to
void add_edge(Graph *g, int from, int to, int weight)
{
    g->Arr[from][to] = g->Arr[to][from] = weight;
}

int MinFunc(Graph *g, int dist[g->Num_nodes], int bools[g->Num_nodes])
{
    int min = INT_MAX;
    int min_index;
    for (int v = 0; v < g->Num_nodes; v++)
    {
        if (bools[v] == 0 && dist[v] < min)
        {
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

// implements the dijkstra algorithm and prints the order in which the nodes are made permament, and the length of the shortest path between the origin node and all the other nodes
void dijkstra(Graph *g, int origin)
{
    int distance[g->Num_nodes], shortestPath[g->Num_nodes];
    for (int i = 0; i < g->Num_nodes; i++)
    {
        distance[i] = INT_MAX; //set distance to infinity
        shortestPath[i] = 0;   //and shortestPath to false
    }
    distance[origin] = 0; // distance to the origin will always be 0;

    for (int eachNode = 0; eachNode < g->Num_nodes; eachNode++)
    {
        int min_index = MinFunc(g, distance, shortestPath); //get min value of current set of unsearched vertices
        shortestPath[min_index] = 1;                        // set current index to true

        for (int Vertex = 0; Vertex < g->Num_nodes; Vertex++)
        {
            if (!shortestPath[Vertex] && g->Arr[min_index][Vertex] && distance[min_index] != INT_MAX && distance[min_index] + g->Arr[min_index][Vertex] < distance[Vertex])
            {
                distance[Vertex] = distance[min_index] + g->Arr[min_index][Vertex];
            }
        }
        printf("%c ", int2char(min_index));
    }
    //The length of the shortest path between A and A is 0
    printf("\n");
    for (int i = 0; i < g->Num_nodes; i++)
    {
        printf("The length of the shortest path between %c and %c is %d\n", int2char(origin), int2char(i), distance[i]);
    }
}

//Deletes graph
void delete_graph(Graph *g)
{
    free(g);
}