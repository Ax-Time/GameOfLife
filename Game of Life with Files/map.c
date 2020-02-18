#include "./map.h"
#include <windows.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>

status *getEl(Map *map, int i, int j)
{
    return (map->map + (map->c * i) + j);
}

Map *newMap(int r, int c)
{
    status *new_map_matrix = (status*) malloc(r * c * sizeof(int));
    if(!new_map_matrix) return NULL;

    Map *new_map = (Map*) malloc(sizeof(Map));
    if(!new_map) return NULL;

    new_map->r = r;
    new_map->c = c;
    new_map->map = new_map_matrix;

    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            GET(new_map, i, j) = EMPTY;
        }
    }

    return new_map;
}

void destroyMap(Map *map)
{
    free(map->map);
    map->map = NULL;

    free(map);
    map = NULL;
}

Map *loadMap(char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    
    int r = 0;
    int c = 0;

    char buffer[1000];

    fgets(buffer, 1000, fp);
    r = atoi(buffer);

    fgets(buffer, 1000, fp);
    c = atoi(buffer);

    Map *new_map = newMap(r, c);
    if(!new_map) return NULL;

    char empty = fgetc(fp);
    fseek(fp, -sizeof(char), SEEK_CUR);

    for(int i = 0; i < r; i++)
    {
        fgets(buffer, 1000, fp);
        for(int j = 0; j < c; j++)
        {
            if(buffer[j] == empty) GET(new_map, i, j) = EMPTY;
            else GET(new_map, i, j) = FULL;
        }
        
    }

    return new_map;
}

void displayMap(Map *map)
{
    for(int i = 0; i < map->r; i++)
    {
        for(int j = 0; j < map->c; j++)
        {
            if(GET(map, i, j) == FULL) mvprintw(CORNER_X + i, CORNER_Y + (2 * j), "%c", 254);
            else if(GET(map, i, j) == EMPTY) mvprintw(CORNER_X + i, CORNER_Y + (2 * j), "%c", 250);
        }
    }

    refresh();
}

int nearbyAliveCells(Map *map, int i, int j)
{
    int alive = 0;

    for(int m = -1; m <= 1; m++)
    {
        for(int n = -1; n <= 1; n++)
        {
            if( !(m == 0 && n == 0) && i + m >= 0 && i + m < map->r && j + n >= 0 && j + n < map->c )
            {
                if(GET(map, i + m, j + n) == FULL) alive++;
            }
        }
    }

    return alive;
}

void updateMap(Map *map)
{
    Map *suppMap = newMap(map->r, map->c);

    for(int i = 0; i < map->r; i++)
    {
        for(int j = 0; j < map->c; j++)
        {
            if(GET(map, i, j) == EMPTY)
            {
                if(nearbyAliveCells(map, i, j) == 3) GET(suppMap, i, j) = FULL;
            }
            else if(GET(map, i, j) == FULL)
            {
                if(nearbyAliveCells(map, i, j) < 2 || nearbyAliveCells(map, i, j) > 3) GET(suppMap, i, j) = FULL;
            }
        }
    }

    for(int i = 0; i < map->r; i++)
    {
        for(int j = 0; j < map->c; j++)
        {
            if(GET(suppMap, i, j) == FULL) 
            {
                if(GET(map, i, j) == FULL) GET(map, i, j) = EMPTY;
                else if(GET(map, i, j) == EMPTY) GET(map, i, j) = FULL;
            }
        }
    }

    destroyMap(suppMap);
}

