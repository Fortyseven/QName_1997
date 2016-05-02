#include "qname.h"

#define VID_X 320
#define VID_Y 200

#define QUAKE_MAX_CHAR (14*2)

#define DEFAULT_NAME "Right click!"

//During development, I just define DATFILE_NAME to be QNAME.DAT, rather than
//rebind EVERY RUN.  (How dumb would THAT be, eh? :))

#define DATFILE_NAME "#"
//#define DATFILE_NAME "QNAME.DAT"

#define quit(s) { allegro_exit();                                                    \
                  printf("\007ERROR: %s\nLine %d, file %s\n", s, __LINE__, __FILE__);\
                  puts(allegro_error);                                               \
                  exit(0x47);}


#define NAME_X 8
#define NAME_Y 8

#define CHAR_LIST_X  32
#define CHAR_LIST_Y  ((VID_Y-(CHAR_HEIGHT*CHAR_DOWN))-8)
#define CHAR_LIST_XX (CHAR_LIST_X+(CHAR_WIDTH*CHAR_ACROSS))
#define CHAR_LIST_YY (CHAR_LIST_Y+(CHAR_DOWN*CHAR_HEIGHT))

#define CHAR_WIDTH   8
#define CHAR_HEIGHT  8

#define CHAR_ACROSS  32
#define CHAR_DOWN    8

#define mouse_in_char_box (((mouse_x>=CHAR_LIST_X) && (mouse_x<=CHAR_LIST_XX)) && \
                           ((mouse_y>=CHAR_LIST_Y) && (mouse_y<=CHAR_LIST_YY)))

#define char_mouse_x ((mouse_x-CHAR_LIST_X)/CHAR_WIDTH)
#define char_mouse_y ((mouse_y-CHAR_LIST_Y)/CHAR_HEIGHT)

#define char_at_mouse ((char_mouse_y*CHAR_ACROSS)+char_mouse_x)

#define play(smp, pan, freq, loop) if (usesnd) play_sample(smp, 255, pan, freq, loop)

extern DATAFILE *dat;

extern BITMAP *bg;
extern BITMAP *buf;

extern int usesnd;
