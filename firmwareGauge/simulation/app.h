
#define TXTSZ 1024

#define FONT_FILE      "./arial.ttf"

#define COUNTDOWN 3.

class Cappdata
{
 public:
  Cappdata(int width, int height);
  ~Cappdata();
  
  void create_layout();
  void render_gui();
  void key_on(int code);
  void key_off(int code);
  void setlooptime();
  void quit();
  
 private:
  void set_coord(t_fcoord *coord, float prx, float pry);
  void draw_basic_rectangle(Cgfxarea *pw, int color);
  void renderTextBlended(t_coord pos, int color, std::string text, bool blended);
  void rendertext(t_coord pos, int color, char *text);
  void set_render_color(int color);

 public:
  int             m_width;
  int             m_height;
  SDL_Window      *m_sdlwindow;
  int             m_window_xpos;
  int             m_window_ypos;
  SDL_Renderer    *m_sdlRenderer;
  TTF_Font        *m_font;

  CgfxareaList    *m_layout;

  t_coord          m_cstart;
  t_coord          m_ccurrent;
  Ckeypress        m_kstates;  

  double           m_last_time;
  float            m_vbat;
  bool             m_bquit;
};

