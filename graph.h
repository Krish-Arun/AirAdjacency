#ifndef GRAPH_H
#define GRAPH_H

#define MAX_DISTANCE 999999

typedef struct {
    int destId;
    int distance;
} Edge;

typedef struct {
    int id;
    char *name;
    Edge *edges;
    int edgeCount;
    int edgeCap;
} City;

typedef struct {
    City *cities;
    int cityCount;
    int cityCap;
} Graph;

void init_graph(Graph *g);
void free_graph(Graph *g);
void add_city(Graph *g, int cityId, const char *name);
void add_route(Graph *g, int from, int to, int distance);
void remove_route(Graph *g, int from, int to);
int can_reach(Graph *g, int from, int to);
void print_cities(Graph *g);
void print_graph(Graph *g);
int dijkstra_shortest_path(Graph *g, int source, int dest, int *path, int *pathLength);
int find_alternate_route(Graph *g, int source, int dest, int *path, int *pathLength, int *shortestPath, int shortestLength);

#endif