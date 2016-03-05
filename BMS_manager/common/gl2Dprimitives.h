/*
 Scoreview (R)
 Copyright (C) 2015-2016 Patrick Areny
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

// This class replaces the SDL primitives by other OpenGL primitives, allowing anti aliasing and shit.
// Color format is 32bits AABBGGRR, alpha channel laways on

#define IDENTCOLOR 0xFFFFFFFF

#define MAX_GL_VERTEX_ARRAY_SIZE (3 * 1024)
#define MAX_TRI_SZ 64
#define PROJECTION_STACK_SIZE 4

class Cpicture
{
 public:
  Cpicture(SDL_Surface *ptexture, t_coord size, std::string name, GLuint OpenGL_texture_id);
  ~Cpicture();

  bool is_name(std::string name);

 public:
  std::string m_name;
  t_coord     m_dim;
  SDL_Surface *m_texture;
  GLuint      m_OpenGL_texture_id;
};

typedef struct s_gl_shader_Inouts
{
  GLuint       program_color;
  GLint        uniform_mvp;
  GLint        uniform_color;
  GLint        attribute_vertpos;
  GLuint       program_texture;
  GLint        uniform_text_mvp;  
  GLint        uniform_text_unit;
  GLint        attribute_text_vertpos;
  GLint        attribute_text_coord0;  
}              t_gl_shader_Inouts;

class CGL2Dprimitives
{
 public:
  CGL2Dprimitives(int screen_w, int screen_h);
  ~CGL2Dprimitives();

  void enable_blending(bool enabled);
  int  init_OpenGL();

  void set_clear_color(int color);
  void Clear();
  void RenderPresent();

  void SetLogicalSize(int width, int height);

  void set_viewport(t_coord pos, t_coord dim, t_fcoord orthodim);
  void reset_viewport();

  // Surface compositing
  void blend_surfaces(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dest, SDL_Rect *dstrect);

  // Textures
  GLuint create_OpenGL_texture(SDL_Surface *surface, std::string name);
  void add_picture(Cpicture* picture);
  void add_empty_texture(int w, int h, std::string name);
  bool get_texture_size(std::string name, t_coord *pdim);
  void update_texture_texels(std::string name, int width, int height, void *pmap);
  void draw_texture(std::string name, t_fcoord pos, t_fcoord dim, bool filtering, bool blend = false, t_fcoord *psubdim = NULL, t_fcoord *poffset = NULL);
  void draw_texture_quad(std::string name, t_fcoord *points, bool filtering, bool blend, t_fcoord *psubdim = NULL, t_fcoord *poffset = NULL);
  void surface_to_screen2(t_fcoord pos, t_fcoord dim, SDL_Surface *surface, bool blended = false);
  void surface_to_screen2(t_fcoord pos, t_fcoord dim, int width, int height, void *pmap, bool blended = false);
  void pixdata_to_screen_area(Cgfxarea *pw, int width, int height, void *pmap);
  void draw_key(std::string texturename, t_fcoord pos, t_fcoord dim, bool blending);

  // Text
  bool get_textSize(char *str, TTF_Font *pfont, t_fcoord *pfdim);
  void print(char *str, TTF_Font *pfont, t_fcoord pos, int colorin, bool blended = false, float outline = 0, int colorout = 0xFF000000);
  void print(char *str, TTF_Font *pfont, t_fcoord pos, t_fcoord dim, int colorin, bool blended = false, float outline = 0, int colorout = 0xFF000000);

  // Shapes
  void hline(float x, float y, float width, int color);
  void vline(float x, float y, float height, int color);
  void line(float xsta, float ysta, float xsto, float ysto, int color);
  void polygon(t_fcoord *ppointarr, int points, int color, bool bantialiased);
  void triangle_mesh2D(t_fcoord pos, t_fcoord dim, CMesh *pmesh, int color, bool bantialiased);
  void circle(t_fcoord center, float radius, float thickness, int color, bool bantialiased);
  void disc(t_fcoord center, float radius, int color, bool bantialiased);
  void rectangle(float xstart, float ystart, float xstop, float ystop, int color, bool bantialiased = false);
  void rectangle(t_fcoord pos, t_fcoord dim, int color, bool bantialiased = false);
  void box(float xstart, float ystart, float xstop, float ystop, int color, bool bantialiased = false);
  void box(t_fcoord pos, t_fcoord dim, int color, bool bantialiased = false);
  void rounded_box(t_fcoord pos, t_fcoord dim, int color, bool bantialiased, float radius);

 private:
  void color2openGl(int color);
  void texture_quad(t_fcoord *points, GLuint tidentifier, t_fcoord *ptcoord, bool blending);
  void texture_rectangle(t_fcoord pos, t_fcoord dim, GLuint tidentifier, t_fcoord *ptcoord = NULL, bool blending = false);
  Cpicture* get_picture(std::string name);
  Cpicture* create_empty_texture(int w, int h, std::string name);
  void create_work_texture();
  void destroy_work_texture();
  void surface_to_texture(Cpicture *dsttext, int width, int height, void *pmap, t_fcoord *subdim);
  bool error_handling(int line);
  void add_circular_contour(int *pindex, t_fcoord center, float angle_start, float angle_stop, float radius);
  void my_ortho(float left, float right, float bottom, float top, float nearval, float farval);
  void push_projection_matrix();
  void pop_projection_matrix();
  bool CreateProgram(t_gl_shader_Inouts *pgl_io);

 private:
  //SDL_GLContext          m_GLContext;
  int                    m_w;
  int                    m_h;
  std::list<Cpicture*>   m_picturelist;
  Cpicture*              m_transfert_texture;
  CMesh                  m_reallocMesh;
 public:
  GLuint                 m_vbo_vertexbuffer;
  GLfloat                m_vertex_buffer_data[MAX_GL_VERTEX_ARRAY_SIZE];
  GLuint                 m_ibo_triangles;
  GLushort               m_triangles[MAX_TRI_SZ];
  t_gl_shader_Inouts     m_sh;
  float                  m_color[4];
  float                  m_clear_color;
  float                  m_projection_matrix[16];
  int                    m_stack_index;
  float                  m_projection_matrix_sav[PROJECTION_STACK_SIZE * 16];
};

