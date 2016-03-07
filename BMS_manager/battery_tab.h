

class GLBatCurvWindow : public Fl_Gl_Window
{
public:
  GLBatCurvWindow(TTF_Font *pfont, int X, int Y, int W, int H, const char *L, void *m_pshared);
  ~GLBatCurvWindow();

  void draw();
  int handle(int);

private:
  void  draw_info_lines(float x, int w, int h);
  void  draw_batteries(float x, float y, float w, float h);
  void  draw_batteries_thresholds(float x, float y, float w, float h);
  void  draw_battery(int i, float x, float y, float w, float h);
  float V2pos(float Vbat, float maxv, float minv, float h);

 private:
  void            *m_pshared;
  CGL2Dprimitives *m_prenderer;
  TTF_Font        *m_pfont;
};

void create_battery_tab(int x, int y, int w, int h, void *padd_data);
void update_battery_tab(void *app_data);

