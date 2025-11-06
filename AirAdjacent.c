/*
 * Bidirectional Airline Route Management System (C)
 *
 * Features:
 *   - Predefined cities
 *   - User can add/remove routes
 *   - Check if any route exists (direct or via connections)
 *   - Display graph of routes
 *
 * Uses adjacency list representation.
 * Connectivity check = BFS.
 *
 * Compile:
 *   gcc -std=c11 -O2 airline.c -o airline
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char *name;
    int *neighbors;
    int neighborCount;
    int neighborCap;
} City;

typedef struct {
    City *cities;
    int cityCount;
    int cityCap;
} Graph;

/* ---------- Utility ---------- */

static char *copy_string(const char *s) {
    char *t = malloc(strlen(s) + 1);
    strcpy(t, s);
    return t;
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
    for (int i = 0; i < g->cityCount; ++i)
        if (g->cities[i].id == cityId) return i;
    return -1;
}

static int city_has_neighbor(City *c, int destId) {
    for (int i = 0; i < c->neighborCount; ++i)
        if (c->neighbors[i] == destId) return 1;
    return 0;
}

static void add_neighbor(City *c, int destId) {
    if (!city_has_neighbor(c, destId)) {
        ensure_neighbor_capacity(c);
        c->neighbors[c->neighborCount++] = destId;
    }
}

static void remove_neighbor(City *c, int destId) {
    for (int i = 0; i < c->neighborCount; ++i) {
        if (c->neighbors[i] == destId) {
            for (int j = i; j + 1 < c->neighborCount; ++j)
                c->neighbors[j] = c->neighbors[j + 1];
            c->neighborCount--;
            return;
        }
    }
}

/* ---------- Graph Operations ---------- */

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

/* Add city (used internally, not in menu) */
void addCity(Graph *g, int cityId, const char *name) {
    ensure_city_capacity(g);
    City *c = &g->cities[g->cityCount++];
    c->id = cityId;
    c->name = copy_string(name);
    c->neighbors = NULL;
    c->neighborCount = 0;
    c->neighborCap = 0;
}

/* Add bidirectional route */
void addRoute(Graph *g, int a, int b) {
    int ai = find_city_index(g, a);
    int bi = find_city_index(g, b);
    if (ai == -1 || bi == -1) {
        printf("Invalid city IDs.\n");
        return;
    }
    if (a == b) {
        printf("Cannot add route from city to itself.\n");
        return;
    }
    add_neighbor(&g->cities[ai], b);
    add_neighbor(&g->cities[bi], a);
    printf("Added route %d <-> %d.\n", a, b);
}

/* Remove bidirectional route */
void removeRoute(Graph *g, int a, int b) {
    int ai = find_city_index(g, a);
    int bi = find_city_index(g, b);
    if (ai == -1 || bi == -1) {
        printf("Invalid city IDs.\n");
        return;
    }
    remove_neighbor(&g->cities[ai], b);
    remove_neighbor(&g->cities[bi], a);
    printf("Removed route %d <-> %d.\n", a, b);
}

/* Check if route (direct or indirect) exists using BFS */
int findRoute(Graph *g, int a, int b) {
    int ai = find_city_index(g, a);
    int bi = find_city_index(g, b);
    if (ai == -1 || bi == -1) {
        printf("Invalid city IDs.\n");
        return 0;
    }
    if (a == b) return 1;

    int n = g->cityCount;
    char *visited = calloc(n, sizeof(char));
    int *q = malloc(n * sizeof(int));
    int qh = 0, qt = 0;

    visited[ai] = 1;
    q[qt++] = ai;

    while (qh < qt) {
        int curIdx = q[qh++];
        City *cur = &g->cities[curIdx];
        for (int k = 0; k < cur->neighborCount; ++k) {
            int neighId = cur->neighbors[k];
            int ni = find_city_index(g, neighId);
            if (visited[ni]) continue;
            if (neighId == b) {
                free(visited);
                free(q);
                return 1;
            }
            visited[ni] = 1;
            q[qt++] = ni;
        }
    }

    free(visited);
    free(q);
    return 0;
}

/* Display all routes */
void print_graph(Graph *g) {
    printf("\n--- Airline Routes ---\n");
    for (int i = 0; i < g->cityCount; ++i) {
        City *c = &g->cities[i];
        printf("%d (%s):", c->id, c->name);
        if (c->neighborCount == 0)
            printf(" [no connections]");
        for (int j = 0; j < c->neighborCount; ++j)
            printf(" %d", c->neighbors[j]);
        printf("\n");
    }
    printf("----------------------\n");
}

/* ---------- Terminal Interface ---------- */

void menu() {
    printf("\n===== Airline Route Management =====\n");
    printf("1. View cities\n");
    printf("2. Add route\n");
    printf("3. Remove route\n");
    printf("4. Check route connectivity\n");
    printf("5. Display route map\n");
    printf("0. Exit\n");
    printf("====================================\n");
    printf("Enter choice: ");
}

/* ---------- Main ---------- */

int main(void) {
    Graph g;
    init_graph(&g);

    /* Predefined cities */
    addCity(&g, 1, "New Delhi");
    addCity(&g, 2, "Mumbai");
    addCity(&g, 3, "Bengaluru");
    addCity(&g, 4, "Chennai");
    addCity(&g, 5, "Kolkata");

    int choice;
    while (1) {
        menu();
        if (scanf("%d", &choice) != 1) break;

        if (choice == 0) {
            printf("Exiting...\n");
            break;
        }

        int a, b;
        switch (choice) {
        case 1:
            printf("\nCities:\n");
            for (int i = 0; i < g.cityCount; ++i)
                printf("%d: %s\n", g.cities[i].id, g.cities[i].name);
            break;

        case 2:
            printf("Enter two city IDs to connect (e.g. 1 2): ");
            scanf("%d %d", &a, &b);
            addRoute(&g, a, b);
            break;

        case 3:
            printf("Enter two city IDs to remove route (e.g. 1 2): ");
            scanf("%d %d", &a, &b);
            removeRoute(&g, a, b);
            break;

        case 4:
            printf("Enter two city IDs to check connectivity (e.g. 1 4): ");
            scanf("%d %d", &a, &b);
            if (findRoute(&g, a, b))
                printf("Route exists between %d and %d.\n", a, b);
            else
                printf("No route between %d and %d.\n", a, b);
            break;

        case 5:
            print_graph(&g);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    free_graph(&g);
    return 0;
}
