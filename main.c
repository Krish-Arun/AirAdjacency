/*
 * main.c
 * Main driver program for airline route management system
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/* Display the menu options */
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
    printf("6. Find shortest route\n");
    printf("0. Exit\n");
    printf("========================================\n");
    printf("Enter choice: ");
}

/* Clear input buffer */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    Graph g;
    init_graph(&g);
    
    // Initialize with default cities
    printf("Initializing airline network...\n");
    add_city(&g, 1, "New Delhi");
    add_city(&g, 2, "Mumbai");
    add_city(&g, 3, "Bengaluru");
    add_city(&g, 4, "Chennai");
    add_city(&g, 5, "Kolkata");
    add_city(&g, 6, "Hyderabad");
    add_city(&g, 7, "Ahmedabad");
    add_city(&g, 8, "Pune");
    add_city(&g, 9, "Jaipur");
    add_city(&g, 10, "Goa");
    add_city(&g, 11, "Lucknow");
    add_city(&g, 12, "Patna");
    add_city(&g, 13, "Bhopal");
    add_city(&g, 14, "Surat");
    add_city(&g, 15, "Indore");
    
    // Add default unidirectional routes
    printf("\nSetting up default routes...\n");
    add_route(&g, 1, 2);  // New Delhi -> Mumbai
    add_route(&g, 2, 3);  // Mumbai -> Bengaluru
    add_route(&g, 3, 4);  // Bengaluru -> Chennai
    add_route(&g, 4, 5);  // Chennai -> Kolkata
    add_route(&g, 5, 1);  // Kolkata -> New Delhi
    add_route(&g, 1, 6);  // New Delhi -> Hyderabad
    add_route(&g, 6, 7);  // Hyderabad -> Ahmedabad
    add_route(&g, 7, 8);  // Ahmedabad -> Pune
    add_route(&g, 8, 9);  // Pune -> Jaipur
    add_route(&g, 9, 10); // Jaipur -> Goa
    add_route(&g, 10, 11); // Goa -> Lucknow
    add_route(&g, 11, 12); // Lucknow -> Patna
    add_route(&g, 12, 13); // Patna -> Bhopal
    add_route(&g, 13, 14); // Bhopal -> Surat
    add_route(&g, 14, 15); // Surat -> Indore
    add_route(&g, 15, 1);  // Indore -> New Delhi
    add_route(&g, 2, 8);  // Mumbai -> Pune
    add_route(&g, 3, 9);  // Bengaluru -> Jaipur
    add_route(&g, 4, 10); // Chennai -> Goa
    add_route(&g, 5, 11); // Kolkata -> Lucknow
    add_route(&g, 6, 12); // Hyderabad -> Patna
    add_route(&g, 7, 13); // Ahmedabad -> Bhopal
    add_route(&g, 8, 14); // Pune -> Surat
    add_route(&g, 9, 15); // Jaipur -> Indore
    add_route(&g, 10, 1); // Goa -> New Delhi
    add_route(&g, 11, 2); // Lucknow -> Mumbai
    add_route(&g, 12, 3); // Patna -> Bengaluru
    add_route(&g, 13, 4); // Bhopal -> Chennai
    add_route(&g, 14, 5); // Surat -> Kolkata
    add_route(&g, 15, 6); // Indore -> Hyderabad
    
    printf("\nAirline network initialized successfully!\n");
    
    int choice;
    int from, to;
    int scanResult;
    
    while (1) {
        show_menu();
        
        scanResult = scanf("%d", &choice);
        if (scanResult != 1) {
            printf("\nError: Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        // Exit option
        if (choice == 0) {
            printf("\nThank you for using Airline Route Management System!\n");
            printf("Exiting...\n");
            break;
        }
        
        switch (choice) {
            case 1:
                // View all cities
                print_cities(&g);
                break;
                
            case 2:
                // Add a new route
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
                
                add_route(&g, from, to);
                break;
                
            case 3:
                // Remove an existing route
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
                // Check connectivity between two cities
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
                    printf("✓ Route exists from city %d to city %d\n", from, to);
                } else {
                    printf("✗ No route available from city %d to city %d\n", from, to);
                }
                break;
                
            case 5:
                // Display complete route map
                print_graph(&g);
                break;
                
            case 6:
                // Find shortest route between two cities
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
                
                int path[100], pathLen;
                pathLen = shortest_route(&g, from, to, path, 100);
                if (pathLen == -1) {
                    printf("No route found from city %d to city %d\n", from, to);
                } else {
                    printf("Shortest route from city %d to city %d (%d stops):\n", from, to, pathLen - 1);
                    for (int i = 0; i < pathLen; ++i) {
                        int idx = -1;
                        for (int j = 0; j < g.cityCount; ++j) {
                            if (g.cities[j].id == path[i]) {
                                idx = j;
                                break;
                            }
                        }
                        if (idx != -1) {
                            printf("%s", g.cities[idx].name);
                            if (i < pathLen - 1) printf(" -> ");
                        }
                    }
                    printf("\n");
                }
                break;
                
            default:
                printf("\nInvalid choice! Please select a valid option (0-6).\n");
                break;
        }
    }
    
    // Clean up and free all allocated memory
    free_graph(&g);
    
    return 0;
}