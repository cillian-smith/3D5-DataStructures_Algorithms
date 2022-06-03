#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "t3.h"
#define MAX_STRING_SIZE 50

BusStop *VertexArray[MAX_GRAPH];
Graph *g;
int num_vertices = 0;

// The CSV parser
int next_field(FILE *f, char *buf, int max)
{
    int i = 0, end = 0, quoted = 0;

    for (;;)
    {
        // fetch the next character from file
        buf[i] = fgetc(f);
        // if we encounter quotes then flip our state and immediately fetch next char
        if (buf[i] == '"')
        {
            quoted = !quoted;
            buf[i] = fgetc(f);
        }
        // end of field on comma if we're not inside quotes
        if (buf[i] == ',' && !quoted)
        {
            break;
        }
        // end record on newline or end of file
        if (feof(f) || buf[i] == '\n')
        {
            end = 1;
            break;
        }
        // truncate fields that would overflow the buffer
        if (i < max - 1)
        {
            ++i;
        }
    }

    buf[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}

struct BusStop *fetch_stop(FILE *csv)
{
    char buf[MAX_STRING_SIZE];

    BusStop *p = malloc(sizeof(struct BusStop));

    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->StopID = atoi(buf);
    next_field(csv, p->Name, MAX_STRING_SIZE);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->Latitude = atof(buf);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->Longitude = atof(buf);

    return p;
}

struct Edge *fetch_edge(FILE *csv)
{
    char buf[MAX_STRING_SIZE];

    Edge *p = malloc(sizeof(struct BusStop));

    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->StopIDFrom = atoi(buf);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->StopIDTo = atoi(buf);
    next_field(csv, buf, MAX_STRING_SIZE); // load id into buffer as string
    p->weight = atoi(buf);

    return p;
}

void init_graph()
{
    g = malloc(sizeof(Graph));
    for (int i = 0; i < MAX_GRAPH; i++)
    {
        for (int j = 0; j < MAX_GRAPH; j++)
        {
            g->Arr[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int from, int to, int weight)
{
    g->Arr[from][to] = g->Arr[to][from] = weight;
}

//loads the edges from the CSV file of name fname
int load_edges(char *fname)
{
    FILE *f;
    int i = 0;
    int j = 0;
    int weight;
    int num_edges = 0;

    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    fetch_edge(f);
    init_graph();

    Edge *temp = malloc(sizeof(Edge));
    // read until the end of the file
    while (!feof(f))
    {
        temp = fetch_edge(f);
        i = temp->StopIDFrom;
        j = temp->StopIDTo;
        weight = temp->weight;
        add_edge(g, i, j, weight);
        num_edges++;
    }

    // always remember to close your file stream
    fclose(f);
    free(temp);
    printf("Loaded %d edges\n", num_edges);

    return 1;
}

//loads the vertices from the CSV file of name fname
int load_vertices(char *fname)
{
    FILE *f;
    num_vertices = 0;

    // boiler plate code to ensure we can open the file
    f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open %s\n", fname);
        return 0;
    }
    fetch_stop(f);
    BusStop *temp = malloc(sizeof(BusStop));
    // read until the end of the file
    while (!feof(f))
    {
        // VertexArray[num_vertices++] = fetch_stop(f);
        temp = fetch_stop(f);
        VertexArray[temp->StopID] = temp;
        num_vertices++;
    }
    temp = NULL;
    // always remember to close your file stream
    fclose(f);
    free(temp);
    printf("Loaded %d vertices\n", num_vertices);

    return 1;
}

int MinFunc(Graph *g, int dist[MAX_GRAPH], int bools[MAX_GRAPH])
{
    int min = INT_MAX;
    int min_index;
    for (int v = 0; v < MAX_GRAPH; v++)
    {
        if (bools[v] == 0 && dist[v] < min)
        {
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

void dijkstra(Graph *g, int origin, int dest)
{
    int distance[MAX_GRAPH], shortestPath[MAX_GRAPH], prev[MAX_GRAPH];
    for (int i = 0; i < MAX_GRAPH; i++)
    {
        distance[i] = INT_MAX; //set distance to infinity
        shortestPath[i] = 0;   //and shortestPath to false
        prev[i] = 0;
    }
    distance[origin] = 0; // distance to the origin will always be 0;

    for (int eachNode = 0; eachNode < MAX_GRAPH; eachNode++)
    {
        int min_index = MinFunc(g, distance, shortestPath); //get min value of current set of unsearched vertices
        shortestPath[min_index] = 1;                        // set current index to true

        for (int Vertex = 0; Vertex < MAX_GRAPH; Vertex++)
        {
            if (!shortestPath[Vertex] && g->Arr[min_index][Vertex] && distance[min_index] != INT_MAX && distance[min_index] + g->Arr[min_index][Vertex] < distance[Vertex])
            {
                prev[Vertex] = min_index;
                distance[Vertex] = distance[min_index] + g->Arr[min_index][Vertex];
            }
        }
    }
    while (prev[dest] != origin)
    {
        printf("%d %s\n", VertexArray[prev[dest]]->StopID, VertexArray[prev[dest]]->Name);
        dest = prev[dest];
    }
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// prints the shortest path between startNode and endNode, if there is any
void shortest_path(int startNode, int endNode)
{
    swap(&startNode, &endNode);
    printf("%d %s\n", VertexArray[endNode]->StopID, VertexArray[endNode]->Name);
    dijkstra(g, startNode, endNode);
    printf("%d %s\n", VertexArray[startNode]->StopID, VertexArray[startNode]->Name);
}

// frees any memory that was used
void free_memory(void)
{
    free(g);
}
