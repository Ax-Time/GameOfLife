#ifndef MAP_H
#define MAP_H

#define GET(map_address, i, j) (* (getEl(map_address, i, j)) )
#define CORNER_X 3
#define CORNER_Y 4

typedef enum
{
    EMPTY, FULL
}status;

typedef struct map
{
    int r; 
    int c;
    status *map;
}Map;

status *getEl(Map *map, int i, int j);

Map *newMap(int r, int c);

void destroyMap(Map *map);

Map *loadMap(char *fileName);

void displayMap(Map *map);

int nearbyAliveCells(Map *map, int i, int j);

void updateMap(Map *map);

#endif
