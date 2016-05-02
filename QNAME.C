//=================================
// Freeware DOS Quake Name Utility
//=================================
#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "q.h"
#include "q_input.h"

DATAFILE *dat;

BITMAP   *bg;
BITMAP   *buf;

int done      = 0;
int usesnd    = 1;
int skiptitle = 0;

unsigned char cur_name[QUAKE_MAX_CHAR]=DEFAULT_NAME; //excessive, but safe. :)~
unsigned char cp=0;

//----------------------------------------------------------------------------
inline
void qprintch(BITMAP *where, unsigned char c, int x, int y)
{
  draw_sprite(where, dat[ CHAR_000+c ].dat, x,y);
}
//----------------------------------------------------------------------------
void qprintf(BITMAP *where, unsigned char *s, int x, int y)
{
  int z=0;

  while(s[z])
     {
       qprintch(where, s[z], x, y);
       x+=CHAR_WIDTH;
       z++;
     }
  if (cursor_on) qprintch(where, 11, x, y);
}
//----------------------------------------------------------------------------
inline
void _qprintch(unsigned char c, int x, int y)
{
 draw_sprite(bg, dat[ CHAR_000+c ].dat, x,y);
}
//----------------------------------------------------------------------------
void _qprintf(BITMAP *where, unsigned char *s, int x, int y)
{
  int z=0;

  while(s[z])
    {
      qprintch(where, s[z], x, y);
      x+=CHAR_WIDTH;
      z++;
    }
  if (cursor_on)
    {
      qprintch(where, 11, x, y);
    }
}
//----------------------------------------------------------------------------
void draw_char_box()
{
  int x=0, y=0,
      c=0, z;

  while(c<256)
    {
       z=0;
       for (x=0; x<CHAR_ACROSS; x++)
          {
            _qprintch(c, CHAR_LIST_X+z, CHAR_LIST_Y+y);
            z+=CHAR_WIDTH;
            c++; if (c>=256) return;
          }
       y+=CHAR_HEIGHT;
    }
}
//----------------------------------------------------------------------------
void redraw_screen()
{
   blit(bg, buf, 0,0, 0,0, VID_X, VID_Y);
   qprintf(buf, cur_name, NAME_X, NAME_Y);
   blit(buf, screen, 0,0, 0,0, VID_X, VID_Y);
}
//----------------------------------------------------------------------------
void update_screen()
{
   blit(bg, buf, 0,0, 0,0, VID_X, 20);
   qprintf(buf, cur_name, NAME_X, NAME_Y);
   blit(buf, screen, 0,0, 0,0, VID_X, 20);
}
//----------------------------------------------------------------------------
void handle_input()
{
  if (key[KEY_ESC])
    {
      if (dump_name())
        {
          done=1;
          return;
        }
         else redraw_screen();
    }

  if (keypressed())
    {
      int ch = readkey();

      if ((ch>>8) == KEY_BACKSPACE)
        {
          play(dat[SND_BS].dat, 127, 1000, 0);
          if (cp>0) cp--;
          cur_name[cp]=0;
          rest(100);                                 //delay to cause slow repeat
          return;
        }
      if (ch == 27) return;
      play(dat[SND_SELECT].dat, 127, 1000, 0);
      if (ch > 0)
        {
          if (cp<QUAKE_MAX_CHAR-1)
            {
              cur_name[cp]=ch;                       //insert new char
              cur_name[cp+1]=0;                        //erase it (just for safety)
              cp++;                                  //next char in line
            }
          rest(100);                                 //delay to cause slow repeat
          return;
        }
    }

    if (mouse_b & 1)
        {
          play(dat[SND_SELECT].dat, 127, 1000, 0);
          if (char_at_mouse==0) return;
          if (cp<QUAKE_MAX_CHAR-1)
            {
              cur_name[cp]=(unsigned char)char_at_mouse; //insert new char
              cur_name[cp+1]=0;                          //erase it (just for safety)
              cp++;                                      //next char in line

            }
          rest(100);                                     //delay to cause slow repeat
        }
    else
    if (mouse_b & 2)
      {
        play(dat[SND_BS].dat, 127, 1000, 0);
        if (cp>0) cp--;
        cur_name[cp]=0;
        rest(100);                                       //delay to cause slow repeat
      }

}
//----------------------------------------------------------------------------
void mainloop()
{
  int y;

  play(dat[SND_LOOP].dat, 255, 800, 1);
  play(dat[SND_LOOP].dat, 128, 1000, 1);
  play(dat[SND_LOOP].dat, 0, 1100, 1);

  for(y=20; y>0; y--)  //"Batman" TV series-esque Nova logo zoom
      {
         blit(bg, buf,    0,0, 0,0, VID_X, VID_Y);
         rotate_scaled_sprite(buf, dat[NOVA_LOGO].dat,
                              (SCREEN_W/2)-((y*60) / 2), (SCREEN_H/2)-((y*40) / 2),
                              itofix(0), itofix(y));
         vsync();
         blit(buf, screen, 0,0, 0,0, VID_X, VID_Y);
      }

  blit(bg, buf,    0,0, 0,0, VID_X, VID_Y);
  blit(buf, screen, 0,0, 0,0, VID_X, VID_Y);


  show_mouse(screen);
  while (!done)
    {
      update_screen();
      handle_input();
    }
}
//----------------------------------------------------------------------------
int dump_name()
{
  FILE *F;
  char fn[40]="NAME.CFG";
  unsigned char l;

  gui_fg_color=makecol8(64,64,64);
  gui_bg_color=makecol8(200,200,200);

  font = dat[F_QUAKE].dat;

  switch(alert3("Choose your destiny.", NULL, NULL,
                "Save", "Cancel", "Exit",
                KEY_S, KEY_C, KEY_E))
    {
      case 1: break;
      case 2: return 0;
      case 3: return 1;
    };

  font=dat[F_QUAKE].dat;
  if (file_select("Save Quake Name", fn, "CFG")==0) return 0;

  l=strlen(cur_name);

  F=fopen(fn,"wb");
  if (F==NULL) quit("Error writing NAME.CFG...");
  fputs("name \"",F);
  fwrite(cur_name, l, 1, F);
  fputs("\"\n",F);
  fclose(F);
  return 0;
}
//----------------------------------------------------------------------------
void init()
{
  allegro_init();
  check_cpu();
  install_keyboard();
  install_timer();
  if (usesnd == 1)  //if sound hasn't already been forced off, then detect...
    {
      if (install_sound(-1,-1,0)) usesnd = 0;
    }

  dat=load_datafile(DATFILE_NAME);
  if (dat==NULL) quit("Couldn't find "DATFILE_NAME);

  if (!skiptitle) title_sequence();

  set_gfx_mode(GFX_AUTODETECT, VID_X, VID_Y, 0,0);

  set_palette(dat[QPAL2].dat);

  init_mouse();

  buf=create_bitmap(VID_X, VID_Y);
  bg =create_bitmap(VID_X, VID_Y);

  blit(dat[BG].dat, bg, 0,0, 0,0, bg->w, bg->h);
  draw_char_box();
}
//----------------------------------------------------------------------------
void deinit()
{
  allegro_exit();
}
//============================================================================
//============================================================================
int main(int argc, char *argv[])
{
  if (argc>1) skiptitle = 1;

  init();

  draw_char_box();
  mainloop();

  deinit();
  return 0;
}

