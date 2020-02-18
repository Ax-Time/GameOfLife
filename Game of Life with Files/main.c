#include <curses.h>
#include "./map.h"
#include <windows.h>
#include <stdlib.h>

int main(void)
{
    initscr();
    echo();
    curs_set(false);
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, TRUE);

    char buffer[100] = { 0 };
    printw("Type the name of the file or its path: (with .txt or whatever) ");
    refresh();
    scanw("%s", buffer);

    Map *gameMap = loadMap(buffer);

    while(1)
    {
        displayMap(gameMap);
        updateMap(gameMap);
        Sleep(20);
    }

    endwin();

    return 0;
}