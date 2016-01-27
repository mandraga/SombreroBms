
#define TXTSZ 1024

#define FONT_FILE      "./VeraMono.ttf"

//#define BATNUMBER 11;

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
  
  Cfake_battery *get_fake_bats();
  Cfake_AD7280A *get_fake_AD7280();
  Cfake_charger *get_fake_charger();

  void set_error_led(bool v);
  void set_balancing_led(bool v);
  void set_buzer(bool v);
  void set_cutmains(bool cutmains);

  void set_run_current(int current);
  int  get_run_current();

 private:
  void set_coord(t_fcoord *coord, float prx, float pry);
  void draw_basic_rectangle(Cgfxarea *pw, int color);
  void renderTextBlended(t_coord pos, int color, std::string text, bool blended);
  void rendertext(t_coord pos, int color, char *text);
  void set_render_color(int color);
  void render_bats();

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

  bool             m_bquit;

  // Battery related
  double           m_last_time;
  float            m_vbat;
  Cfake_battery    m_bats[CFGBATNUMBER];
  Cfake_AD7280A    m_fake_AD7280;
  Cfake_charger    m_fake_charger;

  bool             m_errorled;
  bool             m_balancingled;

  bool             m_buzer;
  bool             m_cutmains;

  int              m_run_current;
};

