/// Grid demo
// (c) 2023 by Stefan Roettger

#include "gfx.h"
#include "scrollarea.h"
#include "gridfont.h"
#include "gridarea.h"
#include "sound.h"

void init_font();

int main(void)
{
   // init ASCII gfx
   init_gfx();
   init_color();
   sound_init();

   // game area setup
   int sx = 18, sy = 9;
   set_grid_size(sx, sy);
   set_window_size(COLS, LINES);

   // redefine grid font
   init_font();

   // render game area
   set_grid_text(sx/2, sy/2-1, "g\nf\nx");
   render_grid_line(sx/2-3, sy/2-2, sx/2+3, sy/2-2, '#');
   render_grid_line(sx/2-3, sy/2+2, sx/2+3, sy/2+2, '#');
   render_grid_line(sx/2-3, sy/2-2, sx/2-3, sy/2+2, '#');
   render_grid_line(sx/2+3, sy/2-2, sx/2+3, sy/2+2, '#');
   flood_fill_grid(0, 0, '-');

   // create sprites
   int *sprite[4];
   const char sprite1[] = "t/^^^^^^^\\"
                          "/|   5C0  6o0|"
                          "t| ___ |2-0\\"
                          "t||ttt||tt";
   const char sprite2[] = "t/^^^^^^^\\"
                          "/|   5C0  6o0|"
                          "t| ___ |2-0\\"
                          "t//ttt\\\\tt";
   sprite[0] = convert_char_text(sprite1, 10, 4, 't', true);
   sprite[1] = convert_char_text(sprite2, 10, 4, 't', true);
   sprite[2] = convert_char_text(sprite1, 10, 4, 't', true);
   sprite[3] = convert_char_text(sprite2, 10, 4, 't', true);
   enable_sprite(1, 10, 4, false, true);
   set_sprite_data(1, 10, 4, sprite[2]);
   mirror_sprite_horizontal(1);
   set_sprite_data(1, 10, 4, sprite[3]);
   mirror_sprite_horizontal(1);
   enable_sprite(2, 10, 4, false, true);
   set_sprite_data(2, 10, 4, sprite[2]);
   center_sprite_position(2, 20, 13);

   // game loop
   do
   {
      // update sprite
      static int count = 0;
      static int px = 6, py = 16, dx = 1, dy = 0;
      if (++count%50 == 0) dx = -dx;
      px += dx;
      py += dy;
      if (px == 26) py -= dx;
      if (px == 39) py += dx;
      int s = 0;
      if ((count/3)%2 == 0) s+=1;
      if (dx<0) s+=2;
      set_sprite_data(1, 10, 4, sprite[s]);
      center_sprite_position(1, px, py);
      if (px == 55) sound_play("boing.wav", 0.5);

      // update log
      log_clear();
      log_text("sprite collision with canvas area: %s", detect_area_collision(1, '-')?"yes":"no");
      log_text("sprite touch with canvas area: %s", detect_area_touch(1, dx, dy, '-')?"yes":"no");
      log_text("sprite collision with each other: %s", detect_sprite_collision(1, 2)?"yes":"no");

      // render game area
      redraw_grid_window(sx/2, sy/2);

      // refresh screen
      refresh();

      // wait until next frame
      msleep(100); // 100 ms = 10 fps
   }
   while (tolower(getch()) != 'q');

   // release sprites
   for (int i=0; i<4; i++)
      delete[] sprite[i];

   // release allocated memory
   release_grid();

   // exit ASCII gfx
   sound_exit();
   exit_gfx();

   return(0);
}

// redefine grid font
void init_font()
{
   // redefine 'g'
   set_grid_char_text('g',
                      "2//^^\\"
                      "||  _"
                      "\\\\__/", true);

   // redefine 'f'
   set_grid_char_text('f',
                      "3||^^^"
                      "||-- "
                      "||   ", true);

   // redefine 'x'
   set_grid_char_text('x',
                      "4\\\\ / "
                      " \\\\  "
                      "/ \\\\ ", true);

   // redefine '#'
   set_grid_char_text('#',
                      "#####"
                      "#####"
                      "#####", true);

   // redefine and animate '-'
   set_grid_char_text('-',
                      "     "
                      "  -  "
                      "     ");
   set_grid_char_text('1',
                      "     "
                      "   - "
                      "     ");
   set_grid_char_text('2',
                      "     "
                      "    -"
                      "     ");
   set_grid_char_text('3',
                      "     "
                      "-    "
                      "     ");
   set_grid_char_text('4',
                      "     "
                      " -   "
                      "     ");
   set_grid_animation_string('-', "-1234");
}
