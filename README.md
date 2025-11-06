# Airline Route Management (C)

A minimal and intuitive command-line program in C to manage airline routes between cities. It allows viewing predefined cities, adding/removing directed routes, checking connectivity, and displaying the route map using a unidirectional graph representation.

## Features

- Predefined set of cities and example routes
- Command-line menu for adding/removing routes
- Check if a route exists from one city to another (reachability)
- Display all cities and routes

## Files

- `graph.h`: Data structures and function declarations
- `graph.c`: Implementation of graph operations and utilities
- `main.c`: Interactive menu and default initialization

## How to Build

Compile using GCC:
`gcc graph.c main.c -o air.exe`

Run the .exe:
`air.exe`


Use the menu to view cities, add/remove routes, check connectivity, or display the map.
