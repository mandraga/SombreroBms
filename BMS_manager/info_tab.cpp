
#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

void create_info_tab(int x, int y, int w, int h)
{
  int              border;
  Fl_Text_Display *ptext;
  Fl_Text_Buffer  *pbuff;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "info");
  {
    border = 10;
    ptext = new Fl_Text_Display(x + border, y + border, w - 3 * border, h - 3 * border);
    pbuff = new Fl_Text_Buffer();
    ptext->buffer(pbuff);
    pbuff->text("\n\n"
		" Scoreview (tm) beta test version, Revision 1, Jan 2016.\n"
		"\n"
		"            Copyright 2015 vreemdelabs.com\n"
		"\n"
		" Code: Patrick Areny\n"
		" Artwork: Clement Caminal\n"
		"\n"
		" Uses the following libraries:\n"
		"\n"
		"  The SDL 2.0\t(Simple Direct Media Layer library)\n"
		"  SDL_ttf\tSam Lantinga\n"
		"  SDL_image\tSam Lantinga and Mattias Engdegård\n"
		"  Portaudio  \twww.portaudio.com\n"
		"  DSPFilters \tVinnie Falco\n"
		"  Tinyxml    \tLee Thomason\n"
		"  libevent   \tNick Mathewson and Niels Provos\n"
		"  libsndfile \tErik de Castro Lopo\n"
		"  OpenGl     \tKhronos group\n"
		"  OpenCl     \tKhronos group\n"
		"  Protocol Buffers\tGoogle\n"
		"  FLTK       \twww.fltk.org\n"
		"\n"
		" Uses the following fonts: "
		"\n\n"
		" ArmWrestler ttf, AJ Paglia\n"
		" VeraMono ttf, Bitstream, Inc.\n"
		"\n");
  }
  group1->end();
}

