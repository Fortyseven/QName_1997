#include <alleg.h>
#include "q.h"

extern qprintf(BITMAP *where, unsigned char *s, int x, int y);

////////////////////////////////////////////////////////////////////////////////////
void title_sequence()
{
   BITMAP  *bg;
   BITMAP  *title;
   PALETTE  white;
   int      x=0;
   int      y=0;
   float    z=11;
   int      v;

   for(x=0; x<256; x++) {white[x].r = 63; white[x].g = 63; white[x].b = 63;}

   set_gfx_mode(GFX_AUTODETECT, 320, 240, 0,0);

   bg = create_bitmap(SCREEN_W, SCREEN_H);

   show_mouse(NULL);

   set_palette(black_palette);

   blit(dat[TITLE_BG].dat, bg, 0,0, 0,0, SCREEN_W, SCREEN_H);
   blit(bg, screen,            0,0, 0,0, SCREEN_W, SCREEN_H);

   title=(BITMAP*) (&dat[TITLE_FG].dat);

   v = allocate_voice(dat[RADIO].dat); //if (v==-1) quit("Error...");
   voice_start(v);
   while(voice_get_position(v) != -1);
   deallocate_voice(v);

   //play_sample(dat[RADIO].dat, 255, 128, 1000, 0); rest(3110);

   play_sample(dat[DROP].dat,  255, 128, 1000, 0);
   fade_from_range(black_palette, white, 20, 0, 255);
   fade_from_range(white, dat[QPAL2].dat,10, 0, 255);      //rocket flash

   for(y=0; y<120; y+=2, z-=0.089*2)
      {
        blit(dat[TITLE_BG].dat, bg, 0,0, 0,0, SCREEN_W, SCREEN_H);
        rotate_scaled_sprite(bg, dat[SPR_ROCKET].dat,
                             (SCREEN_W/2)-29, y,
                             itofix(y*2),
                             ftofix(z));
        vsync();
        blit(bg, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
      }

   v = allocate_voice(dat[EXPLO].dat); //if (v==-1) quit("Error...");
   voice_start(v);
   //play_sample(dat[EXPLO].dat,  255, 128, 1000, 0);
   fade_from_range(dat[QPAL2].dat,white, 10, 0, 255);      //rocket flash

   blit(dat[TITLE_BG].dat, bg, 0,0, 0,0, SCREEN_W, SCREEN_H);
   masked_blit(dat[TITLE_FG].dat, bg, 0,0, 24,50, SCREEN_W, SCREEN_H);
   qprintf(bg, "PRESS ANY KEY‘", 5, 190);

   qprintf(bg, "Ðòåóåîôåä âùº Clan Nova Marines", 5, 210);
   qprintf(bg, "     Ãïäå âùº FïòôùSåöåîNOVA",  5, 220);
   qprintf(bg, "         ÕÒÌº www.halflife.org¯nova", 5, 230);
   blit(bg, screen, 0,0, 0,0, SCREEN_W, SCREEN_H);
   
   fade_from_range(white, dat[QPAL2].dat,30, 0, 255);      //rocket flash
//   rest(2000);
   while(voice_get_position(v) != -1);
   deallocate_voice(v);

   while(!keypressed()); readkey();

   for(y=0; y<20; y++)
      {
         rotate_scaled_sprite(screen, dat[NOVA_LOGO].dat,
                              (SCREEN_W/2)-((y*60) / 2), (SCREEN_H/2)-((y*40) / 2),
                              itofix(0), itofix(y));
      }
}


