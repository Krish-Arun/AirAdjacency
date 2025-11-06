#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

static char *copy_string(const char *s) {
    char *t = malloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
}

void init_graph(Graph *g) {
    g->cities = NULL;
    g->cityCount = 0;
    g->cityCap = 0;
}

void free_graph(Graph *g) {
    for (int i = 0; i < g->cityCount; ++i) {
        free(g->cities[i].name);
        free(g->cities[i].neighbors);
    }
    free(g->cities);
}

static void ensure_city_capacity(Graph *g) {
    if (g->cityCount >= g->cityCap) {
        int newCap = g->cityCap ? g->cityCap * 2 : 4;
        g->cities = realloc(g->cities, newCap * sizeof(City));
        g->cityCap = newCap;
    }
}

static void ensure_neighbor_capacity(City *c) {
    if (c->neighborCount >= c->neighborCap) {
        int newCap = c->neighborCap ? c->neighborCap * 2 : 4;
        c->neighbors = realloc(c->neighbors, newCap * sizeof(int));
        c->neighborCap = newCap;
    }
}

static int find_city_index(Graph *g, int cityId) {
    for (int i = 0; i < g->cityCount; ++i) {
        if (g->cities[i].id == cityId) return i;
    }
    return -1;
}

static int city_has_neighbor(City *c, int destId) {
    for (int i = 0; i < c->neighborCount; ++i) {
        if (c->neighbors[i] == destId) return 1;
    }
    return 0;
}

void add_city(Graph *g, int cityId, const char *name) {
    ensure_city_capacity(g);
    City *c = &g->cities[g->cityCount++];
    c->id = cityId;
    c->name = copy_string(name);
    c->neighbors = NULL;
    c->neighborCount = 0;
    c->neighborCap = 0;
}

void add_route(Graph *g, int from, int to) {
    int ai = find_city_index(g, from);
    int bi = find_city_index(g, to);
    if (ai == -1 || bi == -1 || from == to) return;
    City *c = &g->cities[ai];
    if (!city_has_neighbor(c, to)) {
        ensure_neighbor_capacity(c);
        c->neighbors[c->neighborCount++] = to;
        printf("Added route %d -> %d\n", from, to);
    }
}

void remove_route(Graph *g, int from, int to) {
    int ai = find_city_index(g, from);
    if (ai == -1) return;
    City *c = &g->cities[ai];
    for (int i = 0; i < c->neighborCount; ++i) {
        if (c->neighbors[i] == to) {
            for (int j = i; j + 1 < c->neighborCount; ++j) {
                c->neighbors[j] = c->neighbors[j + 1];
            }
            c->neighborCount--;
            printf("Removed route %d -> %d\n", from, to);
            return;
        }
    }
}

int can_reach(Graph *g, int from, int to) {
    int ai = find_city_index(g, from);
    int bi = find_city_index(g, to);
    if (ai == -1 || bi == -1) return 0;
    int n = g->cityCount;
    char *visited = calloc(n, 1);
    int *queue = malloc(n * sizeof(int));
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
    printf("Cities:\n");
    for (int i = 0; i < g->cityCount; ++i) {
        printf("ID %d: %s\n", g->cities[i].id, g->cities[i].name);
    }
}

void print_graph(Graph *g) {
    printf("--- Routes ---\n");
    for (int i = 0; i < g->cityCount; ++i) {
        City *c = &g->cities[i];
        printf("%d (%s) ->", c->id, c->name);
        if (c->neighborCount == 0) {
            printf(" [no connections]");
        }
        for (int j = 0; j < c->neighborCount; ++j) {
            printf(" %d", c->neighbors[j]);
        }
        printf("\n");
    }
}
