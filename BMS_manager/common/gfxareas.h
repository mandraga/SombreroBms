/*
 Scoreview (R)
 Copyright (C) 2015 Patrick Areny
 All Rights Reserved.

 Scoreview is free software: you can redistribute it and/or modify
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
  s_coord(float xv = 0, float yv = 0, float zv = 0):
    x(xv),
    y(yv),
    z(zv)
  {
  }
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
  void get_posf(t_fcoord *pos);
  void get_dimf(t_fcoord *dim);
  bool is_in(t_coord pos);
  void resize(t_coord wsize);
  void set_ratio(float ratio);
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

