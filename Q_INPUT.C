#include <allegro.h>
#include "q.h"
#include "q_input.h"

volatile unsigned char cursor_on=0;

//----------------------------------------------------------------------------
void int_cursor()
{
  if (cursor_on==1) cursor_on=0; else cursor_on=1;
}
END_OF_FUNCTION(int_cursor);
//----------------------------------------------------------------------------

void init_mouse()
{
  if (install_mouse()==-1) quit("Hairy rat not found! (I NEED A MOUSE!)");

  install_int(int_cursor, 200);

  LOCK_VARIABLE(cursor_on);
  LOCK_FUNCTION(int_cursor);
  set_mouse_sprite(dat[BMP_CUR].dat);
  set_mouse_sprite_focus(0,9);
  set_mouse_range(CHAR_LIST_X, CHAR_LIST_Y, CHAR_LIST_XX, CHAR_LIST_YY);
}
