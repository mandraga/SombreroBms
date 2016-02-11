
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

void create_baterry_tab(int x, int y, int w, int h)
{
  int              border;
  Fl_Text_Display *ptext;
  Fl_Text_Buffer  *pbuff;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "Battery");
  {
    border = 10;
    ptext = new Fl_Text_Display(x + border, y + border, w - 3 * border, h - 3 * border);
    pbuff = new Fl_Text_Buffer();
    ptext->buffer(pbuff);
    pbuff->text("\n\n"
		"\n");
  }
  group1->end();
}

