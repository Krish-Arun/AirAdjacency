#include <stdio.h>
#include "graph.h"

void show_menu() {
    printf("\nAirline Route Management\n");
    printf("1. View cities\n");
    printf("2. Add route\n");
    printf("3. Remove route\n");
    printf("4. Check route connectivity\n");
    printf("5. Display route map\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    Graph g;
    init_graph(&g);

    // Default cities
    add_city(&g, 1, "New Delhi");
    add_city(&g, 2, "Mumbai");
    add_city(&g, 3, "Bengaluru");
    add_city(&g, 4, "Chennai");
    add_city(&g, 5, "Kolkata");

    // Default unidirectional routes
    add_route(&g, 1, 2);
    add_route(&g, 2, 3);
    add_route(&g, 3, 4);

    int choice;
    while (1) {
        show_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            break;
        }
        if (choice == 0) {
            printf("Exiting...\n");
            break;
        }
        int from, to;
        switch (choice) {
            case 1:
                print_cities(&g);
                break;
            case 2:
                printf("Enter two city IDs to connect (from to): ");
                if (scanf("%d %d", &from, &to) == 2) {
                    add_route(&g, from, to);
                } else {
                    printf("Invalid input\n");
                }
                break;
            case 3:
                printf("Enter two city IDs to remove route (from to): ");
                if (scanf("%d %d", &from, &to) == 2) {
                    remove_route(&g, from, to);
                } else {
                    printf("Invalid input\n");
                }
                break;
            case 4:
                printf("Enter two city IDs to check connectivity (from to): ");
                if (scanf("%d %d", &from, &to) == 2) {
                    if (can_reach(&g, from, to)) {
                        printf("Route exists from %d to %d\n", from, to);
                    } else {
                        printf("No route from %d to %d\n", from, to);
                    }
                } else {
                    printf("Invalid input\n");
                }
                break;
            case 5:
                print_graph(&g);
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }

    free_graph(&g);
    return 0;
}
