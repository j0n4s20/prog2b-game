#include "gfx.h"
#include "gridfont.h"
#include "sound.h"
#include "game.h"
#include <ncurses.h>

int main() {
    // Initialisiere NCurses
    //initscr();
    init_gfx();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    // init sound
   sound_init();

    // Starte den Game-Loop
    gameLoop();

    // Beende NCurses
    //endwin();
    exit_gfx();
       // exit sound
   sound_exit();

    return 0;
}

