#include "game.h"
#include <ncurses.h>

int main() {
    // Initialisiere NCurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // Starte den Game-Loop
    gameLoop();

    // Beende NCurses
    endwin();

    return 0;
}
