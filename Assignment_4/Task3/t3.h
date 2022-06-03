#ifndef T3_H_
#define T3_H_
#define MAX_GRAPH 8000

typedef struct Graph Graph;
struct Graph
{
    int Arr[MAX_GRAPH][MAX_GRAPH];
};

typedef struct BusStop BusStop;
struct BusStop
{
    int StopID;
    char Name[50];
    double Latitude;
    double Longitude;
};

typedef struct Edge Edge;
struct Edge
{
    int StopIDFrom;
    int StopIDTo;
    int weight;
};

int load_edges(char *fname);                    //loads the edges from the CSV file of name fname
int load_vertices(char *fname);                 //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory(void);                         // frees any memory that was used

#endif