/*
 * graph.h
 * Header file for airline route management system
 * Contains structure definitions and function declarations
 */

#ifndef GRAPH_H
#define GRAPH_H

/* City structure representing a node in the graph */
typedef struct {
    int id;                    // Unique identifier for the city
    char *name;                // Name of the city (dynamically allocated)
    int *neighbors;            // Array of neighbor city IDs (adjacency list)
    int neighborCount;         // Current number of neighbors
    int neighborCap;           // Capacity of neighbors array
} City;

/* Graph structure representing the airline route network */
typedef struct {
    City *cities;              // Dynamic array of cities
    int cityCount;             // Current number of cities
    int cityCap;               // Capacity of cities array
} Graph;

/* Graph initialization and cleanup */
void init_graph(Graph *g);
void free_graph(Graph *g);

/* City and route management */
void add_city(Graph *g, int cityId, const char *name);
void add_route(Graph *g, int from, int to);
void remove_route(Graph *g, int from, int to);

/* Query operations */
int can_reach(Graph *g, int from, int to);

/* Display operations */
void print_cities(Graph *g);
void print_graph(Graph *g);

/* Dijkstra's algorithm: returns shortest path length, fills path[] with city IDs, returns pathLen */
int shortest_route(Graph *g, int from, int to, int *path, int maxPathLen);

#endif /* GRAPH_H */