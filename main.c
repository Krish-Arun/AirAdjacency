#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void show_menu(void) {
    printf("\n");
    printf("========================================\n");
    printf("    AIRLINE ROUTE MANAGEMENT SYSTEM    \n");
    printf("========================================\n");
    printf("1. View cities\n");
    printf("2. Add route\n");
    printf("3. Remove route\n");
    printf("4. Check route connectivity\n");
    printf("5. Display route map\n");
    printf("6. Find shortest path (Dijkstra)\n");
    printf("7. Find alternate route\n");
    printf("0. Exit\n");
    printf("========================================\n");
    printf("Enter choice: ");
}

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    Graph g;
    init_graph(&g);
    
    printf("Initializing airline network with 15 cities...\n");
    add_city(&g, 1, "New Delhi");
    add_city(&g, 2, "Mumbai");
    add_city(&g, 3, "Bengaluru");
    add_city(&g, 4, "Chennai");
    add_city(&g, 5, "Kolkata");
    add_city(&g, 6, "Hyderabad");
    add_city(&g, 7, "Pune");
    add_city(&g, 8, "Ahmedabad");
    add_city(&g, 9, "Jaipur");
    add_city(&g, 10, "Lucknow");
    add_city(&g, 11, "Kochi");
    add_city(&g, 12, "Visakhapatnam");
    add_city(&g, 13, "Indore");
    add_city(&g, 14, "Chandigarh");
    add_city(&g, 15, "Goa");
    
    printf("\nSetting up default routes...\n");
    // Routes from Delhi (1)
    add_route(&g, 1, 2, 1400);
    add_route(&g, 1, 3, 2150);
    add_route(&g, 1, 5, 1500);
    add_route(&g, 1, 6, 1430);
    add_route(&g, 1, 8, 950);
    add_route(&g, 1, 9, 250);
    add_route(&g, 1, 10, 500);
    add_route(&g, 1, 14, 245);
    
    // Routes from Mumbai (2)
    add_route(&g, 2, 3, 980);
    add_route(&g, 2, 4, 1330);
    add_route(&g, 2, 6, 700);
    add_route(&g, 2, 7, 210);
    add_route(&g, 2, 8, 550);
    add_route(&g, 2, 13, 700);
    add_route(&g, 2, 15, 600);
    
    // Routes from Bengaluru (3)
    add_route(&g, 3, 4, 350);
    add_route(&g, 3, 6, 470);
    add_route(&g, 3, 11, 800);
    add_route(&g, 3, 12, 550);
    
    // Routes from Chennai (4)
    add_route(&g, 4, 6, 340);
    add_route(&g, 4, 11, 680);
    add_route(&g, 4, 12, 450);
    
    // Routes from Kolkata (5)
    add_route(&g, 5, 10, 550);
    add_route(&g, 5, 12, 1100);
    
    // Routes from Hyderabad (6)
    add_route(&g, 6, 11, 800);
    add_route(&g, 6, 12, 300);
    add_route(&g, 6, 13, 550);
    
    // Routes from Pune (7)
    add_route(&g, 7, 3, 1200);
    add_route(&g, 7, 15, 450);
    
    // Routes from Ahmedabad (8)
    add_route(&g, 8, 9, 700);
    add_route(&g, 8, 13, 600);
    
    // Routes from Jaipur (9)
    add_route(&g, 9, 10, 750);
    add_route(&g, 9, 14, 350);
    
    // Routes from Kochi (11)
    add_route(&g, 11, 15, 1050);
    
    // Routes from Visakhapatnam (12)
    add_route(&g, 12, 13, 800);
    
    // Routes from Indore (13)
    add_route(&g, 13, 14, 900);
    
    printf("\nAirline network initialized successfully with 15 cities!\n");
    
    int choice;
    int from, to, distance;
    int scanResult;
    
    while (1) {
        show_menu();
        
        scanResult = scanf("%d", &choice);
        if (scanResult != 1) {
            printf("\nError: Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        if (choice == 0) {
            printf("\nThank you for using Airline Route Management System!\n");
            printf("Exiting...\n");
            break;
        }
        
        switch (choice) {
            case 1:
                print_cities(&g);
                break;
                
            case 2:
                printf("\nEnter source city ID: ");
                if (scanf("%d", &from) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter destination city ID: ");
                if (scanf("%d", &to) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter distance in km: ");
                if (scanf("%d", &distance) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                add_route(&g, from, to, distance);
                break;
                
            case 3:
                printf("\nEnter source city ID: ");
                if (scanf("%d", &from) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter destination city ID: ");
                if (scanf("%d", &to) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                remove_route(&g, from, to);
                break;
                
            case 4:
                printf("\nEnter source city ID: ");
                if (scanf("%d", &from) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter destination city ID: ");
                if (scanf("%d", &to) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("\nChecking connectivity...\n");
                if (can_reach(&g, from, to)) {
                    printf("Route exists from city %d to city %d\n", from, to);
                } else {
                    printf("No route available from city %d to city %d\n", from, to);
                }
                break;
                
            case 5:
                print_graph(&g);
                break;
                
            case 6: {
                printf("\nEnter source city ID: ");
                if (scanf("%d", &from) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter destination city ID: ");
                if (scanf("%d", &to) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                int path[100];
                int pathLength;
                int dist = dijkstra_shortest_path(&g, from, to, path, &pathLength);
                
                if (dist == -1) {
                    printf("\nNo path exists from city %d to city %d\n", from, to);
                } else {
                    printf("\n=== Shortest Path (Dijkstra) ===\n");
                    printf("Total Distance: %d km\n", dist);
                    printf("Path: ");
                    for (int i = 0; i < pathLength; i++) {
                        int idx = -1;
                        for (int j = 0; j < g.cityCount; j++) {
                            if (g.cities[j].id == path[i]) {
                                idx = j;
                                break;
                            }
                        }
                        if (idx != -1) {
                            printf("%s", g.cities[idx].name);
                            if (i < pathLength - 1) printf(" -> ");
                        }
                    }
                    printf("\n");
                }
                break;
            }
                
            case 7: {
                printf("\nEnter source city ID: ");
                if (scanf("%d", &from) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                printf("Enter destination city ID: ");
                if (scanf("%d", &to) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                
                int shortestPath[100];
                int shortestLength;
                int shortestDist = dijkstra_shortest_path(&g, from, to, shortestPath, &shortestLength);
                
                if (shortestDist == -1) {
                    printf("\nNo path exists from city %d to city %d\n", from, to);
                    break;
                }
                
                printf("\n=== Shortest Path ===\n");
                printf("Distance: %d km\n", shortestDist);
                printf("Path: ");
                for (int i = 0; i < shortestLength; i++) {
                    int idx = -1;
                    for (int j = 0; j < g.cityCount; j++) {
                        if (g.cities[j].id == shortestPath[i]) {
                            idx = j;
                            break;
                        }
                    }
                    if (idx != -1) {
                        printf("%s", g.cities[idx].name);
                        if (i < shortestLength - 1) printf(" -> ");
                    }
                }
                printf("\n");
                
                int altPath[100];
                int altLength;
                int altDist = find_alternate_route(&g, from, to, altPath, &altLength, shortestPath, shortestLength);
                
                if (altDist == -1) {
                    printf("\nNo alternate route available\n");
                } else {
                    printf("\n=== Alternate Route ===\n");
                    printf("Distance: %d km\n", altDist);
                    printf("Path: ");
                    for (int i = 0; i < altLength; i++) {
                        int idx = -1;
                        for (int j = 0; j < g.cityCount; j++) {
                            if (g.cities[j].id == altPath[i]) {
                                idx = j;
                                break;
                            }
                        }
                        if (idx != -1) {
                            printf("%s", g.cities[idx].name);
                            if (i < altLength - 1) printf(" -> ");
                        }
                    }
                    printf("\n");
                    printf("Additional distance: %d km\n", altDist - shortestDist);
                }
                break;
            }
                
            default:
                printf("\nInvalid choice! Please select a valid option (0-7).\n");
                break;
        }
    }
    
    free_graph(&g);
    
    return 0;
}