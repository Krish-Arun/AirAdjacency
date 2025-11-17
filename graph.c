
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"

int find_city_index(Graph *g, int cityId) {
    for (int i = 0; i < g->cityCount; ++i) {
        if (g->cities[i].id == cityId) {
            return i;
        }
    }
    return -1;
}


static int city_has_edge_to(City *c, int destId) {
    for (int i = 0; i < c->edgeCount; ++i) {
        if (c->edges[i].destId == destId) {
            return 1;
        }
    }
    return 0;
}

void ensure_city_capacity(Graph *g) {
    if (g->cityCount >= g->cityCap) {
        int newCap = g->cityCap ? g->cityCap * 2 : 4;
        City *temp = realloc(g->cities, newCap * sizeof(City));
        if (temp == NULL) return;
        g->cities = temp;
        g->cityCap = newCap;
    }
}

static void ensure_edge_capacity(City *c) {
    if (c->edgeCount >= c->edgeCap) {
        int newCap = c->edgeCap ? c->edgeCap * 2 : 4;
        Edge *temp = realloc(c->edges, newCap * sizeof(Edge));
        if (temp == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        c->edges = temp;
        c->edgeCap = newCap;
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
        free(g->cities[i].edges);
    }
    free(g->cities); //finally freeing the cities array location itself
    g->cities = NULL;
    g->cityCount = 0;
    g->cityCap = 0;
}

void add_city(Graph *g, int cityId, const char *name) {
    if (g == NULL || name == NULL) return;
    
    if (find_city_index(g, cityId) != -1) {
        printf("City with ID %d already exists\n", cityId);
        return;
    }
    
    ensure_city_capacity(g);
    City *c = &g->cities[g->cityCount++];
    c->id = cityId;
    c->name = copy_string(name);
    c->edges = NULL;
    c->edgeCount = 0;
    c->edgeCap = 0;
}

void add_route(Graph *g, int from, int to, int distance) {
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
    
    City *c = &g->cities[ai];
    if (city_has_edge_to(c, to)) {
        printf("Route %d -> %d already exists\n", from, to);
        return;
    }
    
    ensure_edge_capacity(c);
    c->edges[c->edgeCount].destId = to;
    c->edges[c->edgeCount].distance = distance;
    c->edgeCount++;
    printf("Added route %d -> %d (distance: %d km)\n", from, to, distance);
}

void remove_route(Graph *g, int from, int to) {
    if (g == NULL) return;
    
    int ai = find_city_index(g, from);
    if (ai == -1) {
        printf("Source city %d not found\n", from);
        return;
    }
    
    City *c = &g->cities[ai];
    for (int i = 0; i < c->edgeCount; ++i) {
        if (c->edges[i].destId == to) {
            for (int j = i; j + 1 < c->edgeCount; ++j) {
                c->edges[j] = c->edges[j + 1];
            }
            c->edgeCount--;
            printf("Removed route %d -> %d\n", from, to);
            return;
        }
    }
    printf("Route %d -> %d does not exist\n", from, to);
}

int can_reach(Graph *g, int from, int to) {
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
        for (int i = 0; i < c->edgeCount; ++i) {
            int ni = find_city_index(g, c->edges[i].destId);
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
        
        if (c->edgeCount == 0) {
            printf(" [no outgoing routes]");
        } else {
            for (int j = 0; j < c->edgeCount; ++j) {
                int neighborIdx = find_city_index(g, c->edges[j].destId);
                if (neighborIdx != -1) {
                    printf(" %d(%s, %dkm)", c->edges[j].destId, 
                           g->cities[neighborIdx].name, c->edges[j].distance);
                } else {
                    printf(" %d(?, %dkm)", c->edges[j].destId, c->edges[j].distance);
                }
            }
        }
        printf("\n");
    }
}

int dijkstra_shortest_path(Graph *g, int source, int dest, int *path, int *pathLength) {
    if (g == NULL || path == NULL || pathLength == NULL) return -1;
    
    int n = g->cityCount;
    int *distance = malloc(n * sizeof(int));
    int *visited = malloc(n * sizeof(int));
    int *previous = malloc(n * sizeof(int));
    
    if (distance == NULL || visited == NULL || previous == NULL) {
        free(distance);
        free(visited);
        free(previous);
        return -1;
    }
    
    for (int i = 0; i < n; i++) {
        distance[i] = MAX_DISTANCE;
        visited[i] = 0;
        previous[i] = -1;
    }
    
    int sourceIdx = find_city_index(g, source);
    int destIdx = find_city_index(g, dest);
    
    if (sourceIdx == -1 || destIdx == -1) {
        free(distance);
        free(visited);
        free(previous);
        return -1;
    }
    
    distance[sourceIdx] = 0;
    
    for (int count = 0; count < n; count++) {
        int minDist = MAX_DISTANCE;
        int minIdx = -1;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i] && distance[i] < minDist) {
                minDist = distance[i];
                minIdx = i;
            }
        }
        
        if (minIdx == -1) break;
        
        visited[minIdx] = 1;
        
        if (minIdx == destIdx) break;
        
        City *current = &g->cities[minIdx];
        for (int i = 0; i < current->edgeCount; i++) {
            int neighborIdx = find_city_index(g, current->edges[i].destId);
            if (neighborIdx != -1 && !visited[neighborIdx]) {
                int newDist = distance[minIdx] + current->edges[i].distance;
                if (newDist < distance[neighborIdx]) {
                    distance[neighborIdx] = newDist;
                    previous[neighborIdx] = minIdx;
                }
            }
        }
    }
    
    if (distance[destIdx] == MAX_DISTANCE) {
        free(distance);
        free(visited);
        free(previous);
        return -1;
    }
    
    int tempPath[n];
    int tempLen = 0;
    int current = destIdx;
    
    while (current != -1) {
        tempPath[tempLen++] = g->cities[current].id;
        current = previous[current];
    }
    
    *pathLength = tempLen;
    for (int i = 0; i < tempLen; i++) {
        path[i] = tempPath[tempLen - 1 - i];
    }
    
    int shortestDist = distance[destIdx];
    free(distance);
    free(visited);
    free(previous);
    return shortestDist;
}

static int paths_are_different(int *path1, int len1, int *path2, int len2) {
    if (len1 != len2) return 1;
    
    for (int i = 0; i < len1; i++) {
        if (path1[i] != path2[i]) return 1;
    }
    return 0;
}

static void dfs_find_alternate(Graph *g, int currentIdx, int destIdx, int *visited, 
                               int *currentPath, int currentLen, 
                               int *bestPath, int *bestLen, int *bestDist,
                               int currentDist, int *shortestPath, int shortestLen) {
    
    if (currentIdx == destIdx) {
        if (paths_are_different(currentPath, currentLen, shortestPath, shortestLen)) {
            if (*bestLen == 0 || currentDist < *bestDist) {
                *bestLen = currentLen;
                *bestDist = currentDist;
                for (int i = 0; i < currentLen; i++) {
                    bestPath[i] = currentPath[i];
                }
            }
        }
        return;
    }
    
    City *current = &g->cities[currentIdx];
    for (int i = 0; i < current->edgeCount; i++) {
        int neighborIdx = find_city_index(g, current->edges[i].destId);
        if (neighborIdx != -1 && !visited[neighborIdx]) {
            visited[neighborIdx] = 1;
            currentPath[currentLen] = current->edges[i].destId;
            
            dfs_find_alternate(g, neighborIdx, destIdx, visited, 
                             currentPath, currentLen + 1, 
                             bestPath, bestLen, bestDist,
                             currentDist + current->edges[i].distance,
                             shortestPath, shortestLen);
            
            visited[neighborIdx] = 0;
        }
    }
}

int find_alternate_route(Graph *g, int source, int dest, int *path, int *pathLength, 
                        int *shortestPath, int shortestLength) {
    if (g == NULL || path == NULL || pathLength == NULL) return -1;
    
    int sourceIdx = find_city_index(g, source);
    int destIdx = find_city_index(g, dest);
    
    if (sourceIdx == -1 || destIdx == -1) return -1;
    
    int n = g->cityCount;
    int *visited = calloc(n, sizeof(int));
    int *currentPath = malloc(n * sizeof(int));
    int *bestPath = malloc(n * sizeof(int));
    
    if (visited == NULL || currentPath == NULL || bestPath == NULL) {
        free(visited);
        free(currentPath);
        free(bestPath);
        return -1;
    }
    
    visited[sourceIdx] = 1;
    currentPath[0] = source;
    int bestLen = 0;
    int bestDist = MAX_DISTANCE;
    
    dfs_find_alternate(g, sourceIdx, destIdx, visited, currentPath, 1, 
                      bestPath, &bestLen, &bestDist, 0, shortestPath, shortestLength);
    
    if (bestLen == 0) {
        free(visited);
        free(currentPath);
        free(bestPath);
        return -1;
    }
    
    *pathLength = bestLen;
    for (int i = 0; i < bestLen; i++) {
        path[i] = bestPath[i];
    }
    
    free(visited);
    free(currentPath);
    free(bestPath);
    return bestDist;
}