#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int id;
    char *name;
    int *neighbors;
    int neighborCount, neighborCap;
} City;

typedef struct {
    City *cities;
    int cityCount, cityCap;
} Graph;

void init_graph(Graph *g);
void free_graph(Graph *g);
void add_city(Graph *g, int cityId, const char *name);
void add_route(Graph *g, int from, int to);
void remove_route(Graph *g, int from, int to);
int can_reach(Graph *g, int from, int to);
void print_cities(Graph *g);
void print_graph(Graph *g);

#endif
