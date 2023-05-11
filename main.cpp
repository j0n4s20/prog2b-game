#include <ncurses.h>
#include <unistd.h>

int main()
{
   initscr();
   clear();
   flash();
   refresh();
   endwin();
   return(0);
}
