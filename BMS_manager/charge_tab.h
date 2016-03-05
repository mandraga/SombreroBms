
class GLBatWindow : public Fl_Gl_Window
{
public:
  GLBatWindow(int X, int Y, int W, int H, const char *L, void *m_pshared);
  ~GLBatWindow();

  void draw();
  int handle(int);

private:

 void draw_info_lines(int w, int h);
 void draw_batteries(float x, float y, float w, float h);

 private:
  void            *m_pshared;
  CGL2Dprimitives *m_prenderer;
  TTF_Font        *m_pfont;
};

void create_charge_tab(int x, int y, int w, int h, void *app_data);
void update_charge_tab(void *app_data);

