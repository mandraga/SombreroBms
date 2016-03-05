/*
 SombreroBMS - BMS_manager
 Copyright (C) 2016 Patrick Areny
 All Rights Reserved.

 BMS_manager is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

void create_about_tab(int x, int y, int w, int h)
{
  int              border;
  Fl_Text_Display *ptext;
  Fl_Text_Buffer  *pbuff;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "About");
  {
    border = 10;
    ptext = new Fl_Text_Display(x + border, y + border, w - 3 * border, h - 3 * border);
    pbuff = new Fl_Text_Buffer();
    ptext->buffer(pbuff);
    ptext->textcolor(fl_rgb_color(85, 170, 255));
    ptext->color(fl_rgb_color(8, 7, 5));
    pbuff->text("\n"
		" BMS manager Feb 2016.\n"
		" Part of SombreroBMS (battery management system)\n"
		" Copyright 2016 Patrick Xavier Areny\n"
		"\n"
		"\n"
		" Used libraries:\n"
		"\n"
		"  Tinyxml    \tLee Thomason\n"
		"  FLTK       \twww.fltk.org\n"
		"  OpenGl     \tKhronos group\n"
		"\n"
		"\n"
		" Used font: "
		"\n"
		"\n"
		" VeraMono ttf, Bitstream, Inc.\n"
		"\n");
  }
  group1->end();
}

