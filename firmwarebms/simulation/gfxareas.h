
typedef struct s_fcoord
{
  s_fcoord(float xv = 0, float yv = 0, float zv = 0):
    x(xv),
    y(yv),
    z(zv)
  {
  }
  float x;
  float y;
  float z;
}              t_fcoord;

typedef struct s_coord
{
  int x;
  int y;
  int z;
}              t_coord;

class Cgfxarea
{
 public:
  Cgfxarea(t_fcoord pos, t_fcoord dimensions, t_coord wsize, char *name);
  ~Cgfxarea();

  void get_pos(t_coord *pos);
  void get_dim(t_coord *dim);
  bool is_in(t_coord pos);
  void resize(t_coord wsize);
  void resize_keep_ratio(t_coord wsize);

 private:
  t_coord m_pos;
  t_coord m_dimensions;
  t_fcoord m_fpos;        // Percentage of the window size
  t_fcoord m_fdimensions;
  float    m_ratio;

 public:
  char    m_name[256];
  bool    m_update;
  bool    m_clicked;
};

class CgfxareaList
{
 public:
  CgfxareaList();
  ~CgfxareaList();

  void add(Cgfxarea *pw);
  Cgfxarea *get_first();
  Cgfxarea *get_next();
  bool is_in(int x, int y, char *pname);
  Cgfxarea* get_area(char *pname);
  void resize(t_coord window_size);
 private:
  std::list<Cgfxarea*> m_Cgfxarea_list;
  std::list<Cgfxarea*>::iterator m_iter;
};

