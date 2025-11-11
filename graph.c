
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

int find_city_index(Graph *g, int cityId) {
    for (int i = 0; i < g->cityCount; ++i) {
        if (g->cities[i].id == cityId) {
            return i;
        }
    }
    return -1;
}

int city_has_neighbor(City *c, int destId) {
    for (int i = 0; i < c->neighborCount; ++i) {
        if (c->neighbors[i] == destId) { //run through neighbour (destination array) to see if required one is there
            return 1;
        }
    }
    return 0;
}

void ensure_city_capacity(Graph *g) {
    if (g->cityCount >= g->cityCap) {
        int newCap = g->cityCap ? g->cityCap * 2 : 4;
        City *temp = realloc(g->cities, newCap * sizeof(City));
        if (temp == NULL) return
        g->cities = temp;
        g->cityCap = newCap;
    }
}

void ensure_neighbor_capacity(City *c) {
    if (c->neighborCount >= c->neighborCap) {
        int newCap = c->neighborCap ? c->neighborCap * 2 : 4;
        int *temp = realloc(c->neighbors, newCap * sizeof(int));
        if (temp == NULL) return
        c->neighbors = temp;
        c->neighborCap = newCap;
    }
}

void init_graph(Graph *g) {
    if (g == NULL) return;
    g->cities = NULL;
    g->cityCount = 0;
    g->cityCap = 0;
}

void free_graph(Graph *g) {
    if (g == NULL) return;
    
    for (int i = 0; i < g->cityCount; ++i) { //freeing all allocated mem of every city
        free(g->cities[i].name);
        free(g->cities[i].neighbors);
    }
    free(g->cities); //finally freeing the cities array location itself
    g->cities = NULL;
    g->cityCount = 0;
    g->cityCap = 0;
}

void add_city(Graph *g, int cityId, const char *name) {
    if (g == NULL || name == NULL) return;
    
    if (find_city_index(g, cityId) != -1) { //checks for non empty 'id', could just check if city node is null or not
        printf("City with ID %d already exists\n", cityId);
        return;
    }
    
    ensure_city_capacity(g);
    City *c = &g->cities[g->cityCount++];
    c->id = cityId;
    c->name = malloc(strlen(name) + 1);
    if (c->name) strcpy(c->name, name);

    c->neighbors = NULL;
    c->neighborCount = 0;
    c->neighborCap = 0;
}

void add_route(Graph *g, int from, int to) {
    if (g == NULL) return;
    
    int ai = find_city_index(g, from); //from city
    int bi = find_city_index(g, to); //to city
    
    if (ai == -1) {
        printf("Source city %d not found\n", from);
        return;
    }
    if (bi == -1) {
        printf("Destination city %d not found\n", to);
        return;
    }
    if (from == to) {
        printf("Cannot create route to same city\n");
        return;
    }
    
    City *c = &g->cities[ai]; //created a temp city for checking
    if (city_has_neighbor(c, to)) {
        printf("Route %d -> %d already exists\n", from, to);
        return;
    }
    
    ensure_neighbor_capacity(c);
    c->neighbors[c->neighborCount++] = to; //appended new city to neighbour
    printf("Added route %d -> %d\n", from, to);
}

void remove_route(Graph *g, int from, int to) {
    if (g == NULL) return;
    
    int ai = find_city_index(g, from);
    if (ai == -1) {
        printf("Source city %d not found\n", from);
        return;
    }
    
    City *c = &g->cities[ai];
    for (int i = 0; i < c->neighborCount; ++i) {
        if (c->neighbors[i] == to) {
            for (int j = i; j + 1 < c->neighborCount; ++j) { // we must shift back the remaning cities in the neighbour arr
                c->neighbors[j] = c->neighbors[j + 1];
            }
            c->neighborCount--;
            printf("Removed route %d -> %d\n", from, to);
            return;
        }
    }
    printf("Route %d -> %d does not exist\n", from, to);
}

int can_reach(Graph *g, int from, int to) { //using simple bfs here
    if (g == NULL) return 0;
    
    int ai = find_city_index(g, from);
    int bi = find_city_index(g, to);
    
    if (ai == -1 || bi == -1) {
        return 0;
    }
    
    if (from == to) {
        return 1;
    }
    
    int n = g->cityCount;
    char *visited = calloc(n, sizeof(char)); // we need to initialize to 0
    int *queue = malloc(n * sizeof(int));
    
    if (visited == NULL || queue == NULL) {
        return 0;
    }
    
    int front = 0, rear = 0;
    queue[rear++] = ai;
    visited[ai] = 1;
    
    while (front < rear) {
        int cur = queue[front++];
        
        if (g->cities[cur].id == to) {
            free(queue);
            free(visited);
            return 1;
        }
        
        City *c = &g->cities[cur];
        for (int i = 0; i < c->neighborCount; ++i) {
            int ni = find_city_index(g, c->neighbors[i]);
            if (ni != -1 && !visited[ni]) {
                queue[rear++] = ni;
                visited[ni] = 1;
            }
        }
    }
    
    free(queue);
    free(visited);
    return 0;
}

void print_cities(Graph *g) {
    if (g == NULL) return;
    
    printf("\n=== Cities ===\n");
    if (g->cityCount == 0) {
        printf("No cities in the network\n");
        return;
    }
    
    for (int i = 0; i < g->cityCount; ++i) {
        printf("ID %d: %s\n", g->cities[i].id, g->cities[i].name);
    }
}

void print_graph(Graph *g) {
    if (g == NULL) return;
    
    printf("\n=== Route Map ===\n");
    if (g->cityCount == 0) {
        printf("No cities in the network\n");
        return;
    }
    
    for (int i = 0; i < g->cityCount; ++i) {
        City *c = &g->cities[i];
        printf("%d (%s) ->", c->id, c->name);
        
        if (c->neighborCount == 0) { // skippable tbh
            printf(" [no outgoing routes]");
        } else {
            for (int j = 0; j < c->neighborCount; ++j) {
                int neighborIdx = find_city_index(g, c->neighbors[j]);
                if (neighborIdx != -1) {
                    printf(" %d(%s)", c->neighbors[j], g->cities[neighborIdx].name);
                } else {
                    printf(" %d(?)", c->neighbors[j]);
                }
            }
        }
        printf("\n");
    }
}

// Dijkstra's algorithm for unweighted graph (returns shortest path length, fills path[] with city IDs)
int shortest_route(Graph *g, int from, int to, int *path, int maxPathLen) {
    if (!g || !path || maxPathLen <= 0) return -1;
    int n = g->cityCount;
    int *prev = malloc(n * sizeof(int));
    char *visited = calloc(n, sizeof(char));
    int *queue = malloc(n * sizeof(int));
    if (!prev || !visited || !queue) {
        free(prev); free(visited); free(queue);
        return -1;
    }
    for (int i = 0; i < n; ++i) prev[i] = -1;
    int srcIdx = find_city_index(g, from);
    int dstIdx = find_city_index(g, to);
    if (srcIdx == -1 || dstIdx == -1) {
        free(prev); free(visited); free(queue);
        return -1;
    }
    int front = 0, rear = 0;
    queue[rear++] = srcIdx;
    visited[srcIdx] = 1;
    while (front < rear) {
        int cur = queue[front++];
        if (cur == dstIdx) break;
        City *c = &g->cities[cur];
        for (int i = 0; i < c->neighborCount; ++i) {
            int ni = find_city_index(g, c->neighbors[i]);
            if (ni != -1 && !visited[ni]) {
                queue[rear++] = ni;
                visited[ni] = 1;
                prev[ni] = cur;
            }
        }
    }
    if (!visited[dstIdx]) {
        free(prev); free(visited); free(queue);
        return -1;
    }
    int routeLen = 0;
    int trace = dstIdx;
    while (trace != -1 && routeLen < maxPathLen) {
        path[routeLen++] = g->cities[trace].id;
        trace = prev[trace];
    }
    free(prev); free(visited); free(queue);
    for (int i = 0; i < routeLen / 2; ++i) {
        int tmp = path[i];
        path[i] = path[routeLen - 1 - i];
        path[routeLen - 1 - i] = tmp;
    }
    return routeLen;
}