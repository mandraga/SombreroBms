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
#include <math.h>
#include <list>
#include <vector>
#include <iterator>

#ifdef __ANDROID__
// Android
#include <SDL.h>
#include <SDL_ttf.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#else
// Linux and windows
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glew.h>
//#include <arb_multisample.h>
#endif

// Operations on matrices
#define  GLM_FORCE_CXX98 
#define  GLM_PRECISION_MEDIUMP_INT
#define  GLM_PRECISION_HIGHP_FLOAT
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfxareas.h"
#include "mesh.h"
#include "gl2Dprimitives.h"

using namespace std;

Cpicture::Cpicture(SDL_Surface *ptexture, t_coord size, string name, GLuint OpenGL_texture_id):
  m_name(name),
  m_dim(size),
  m_texture(ptexture),
  m_OpenGL_texture_id(OpenGL_texture_id)
{
  //printf("create texture %d - %s\n", m_OpenGL_texture_id, m_name.c_str());
}

Cpicture::~Cpicture()
{
  //printf("delete texture %d - %s\n", m_OpenGL_texture_id, m_name.c_str());
  glDeleteTextures(1, &m_OpenGL_texture_id);
  SDL_FreeSurface(m_texture);
}

bool Cpicture::is_name(string name)
{
  return m_name == name;
}

//----------------------------------------------------------------------------

CGL2Dprimitives::CGL2Dprimitives(int scr_w, int scr_h):
  m_w(scr_w),
  m_h(scr_h),
  m_stack_index(0)
{
  m_reallocMesh.set_name(string("reallocmesh"));
}

CGL2Dprimitives::~CGL2Dprimitives()
{
  std::list<Cpicture*>::iterator iter;
  
  iter = m_picturelist.begin();
  while (iter != m_picturelist.end())
    {
      delete *iter;
      iter++;
    }
  glDeleteProgram(m_sh.program_color);
  glDeleteProgram(m_sh.program_texture);
  glDeleteBuffers(1, &m_vbo_vertexbuffer);
  glDeleteBuffers(1, &m_ibo_triangles);
}

void CGL2Dprimitives::enable_blending(bool enabled)
{
  if (enabled)
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  else
    {
      glDisable(GL_BLEND);
    }
}

void CGL2Dprimitives::set_clear_color(int color)
{
  unsigned char R, G, B, A;
  float         fR, fG, fB, fA;

  m_clear_color = color;
  R = color & 0xFF;
  G = (color >> 8) & 0xFF;
  B = (color >> 16) & 0xFF;
  A = (color >> 24) & 0xFF;
  fR = (float)R / 255.;
  fG = (float)G / 255.;
  fB = (float)B / 255.;
  fA = (float)A / 255.;
  glClearColor(fR, fG, fB, fA);
}

int CGL2Dprimitives::init_OpenGL()
{
#ifndef __ANDROID__
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      // Problem: glewInit failed, something is seriously wrong.
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      exit(EXIT_FAILURE);
    }
  if (!GLEW_VERSION_2_0)
    {
      printf("Error: your graphic card does not support OpenGL 2.0.\n");
      exit(EXIT_FAILURE);
    }
#endif

  glClear(GL_COLOR_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
#ifndef __ANDROID__
  glDisable(GL_FOG);
#endif
  enable_blending(false);
  //create_work_texture();
  // Compile the shaders
  if (!CreateProgram(&m_sh))
    {
      printf("Error: OpenGL shader creation failed.\n");
      exit(EXIT_FAILURE);
    }
  // Buffers used in the shaders
  glGenBuffers(1, &m_vbo_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_DYNAMIC_DRAW);
  // Vertex indexes
  glGenBuffers(1, &m_ibo_triangles);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_triangles);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_triangles), m_triangles, GL_STATIC_DRAW);
  //
  color2openGl(IDENTCOLOR);
  reset_viewport();
  //RenderPresent();
  error_handling(__LINE__);
  return false;
}

void CGL2Dprimitives::Clear()
{
  set_clear_color(m_clear_color);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
void CGL2Dprimitives::RenderPresent()
{
  SDL_GL_SwapWindow(m_sdl_window);
}*/

void CGL2Dprimitives::SetLogicalSize(int width, int height)
{
  m_w = width;
  m_h = height;
  destroy_work_texture();
  create_work_texture();
  reset_viewport();
}

bool CGL2Dprimitives::error_handling(int line)
{
  GLenum err;
  string errcode;

  return false;
  err = glGetError();
  if (err)
    {
      switch (err)
      {
      case GL_INVALID_ENUM:
	printf("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
      case GL_INVALID_VALUE:
	printf("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
      case GL_INVALID_OPERATION:
	printf("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
#ifndef __ANDROID__
      case GL_INVALID_FRAMEBUFFER_OPERATION:
	printf("The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
      case GL_STACK_OVERFLOW:
	printf("OpenGL stack overflow\n");
	break;
      case GL_STACK_UNDERFLOW:
	printf("An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
	break;
#endif
      case GL_OUT_OF_MEMORY:
	printf("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
	break;
      default:
	printf("OpenGL unknown error\n");
	break;
      };
      printf("OpenGl error: code %d line %d\n", err, line);
      return true;
    }
  return false;
}

void CGL2Dprimitives::push_projection_matrix()
{
  memcpy(&m_projection_matrix_sav[m_stack_index * 16], m_projection_matrix, sizeof(m_projection_matrix));
  m_stack_index++;
}

void CGL2Dprimitives::pop_projection_matrix()
{
  m_stack_index--;
  m_stack_index = m_stack_index < 0? 0 : m_stack_index;
  memcpy(m_projection_matrix, &m_projection_matrix_sav[m_stack_index * 16], sizeof(m_projection_matrix));
}

void CGL2Dprimitives::my_ortho(float left, float right, float bottom, float top, float nearval, float farval)
{
  unsigned int  i;
  float        *pf;

  // glOrtho
  glm::detail::tmat4x4<float> projectionm = glm::ortho(left, right, bottom, top, nearval, farval);
  //glm::mat4 projectionm = glm::ortho(left, right, bottom, top, nearval, farval);
  pf = glm::value_ptr(projectionm);
  for (i = 0; i < sizeof(m_projection_matrix) / sizeof(float); i++)
    {
      m_projection_matrix[i] = pf[i];
    }
}

// Everything outside the window will not be rendered
void CGL2Dprimitives::set_viewport(t_coord pos, t_coord dim, t_fcoord orthodim)
{
  glViewport(pos.x, m_h - (pos.y + dim.y), dim.x, dim.y);
  my_ortho(0, orthodim.x, orthodim.y, 0, -1., 1.);
  if (error_handling(__LINE__))
    {
      printf("Set ViewPort failed.\n");
      exit(EXIT_FAILURE);
    }
}

void CGL2Dprimitives::reset_viewport()
{
  glViewport(0, 0, m_w, m_h);
  my_ortho(0, m_w, m_h, 0, -1., 1.);
  if (error_handling(__LINE__))
    {
      printf("Set ViewPort failed.\n");
      exit(EXIT_FAILURE);
    }
}

void CGL2Dprimitives::blend_surfaces(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dest, SDL_Rect *dstrect)
{
  if (SDL_SetSurfaceBlendMode(dest, SDL_BLENDMODE_BLEND))
    printf("Error: Surface blend mode failed with code \"%s\".\n", SDL_GetError());  
  if (SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND))
    printf("Error: Surface blend mode failed with code \"%s\".\n", SDL_GetError());
  if (SDL_BlitSurface(src, srcrect, dest, dstrect))
    {
      printf("Error: Surface blit failed with code \"%s\".\n", SDL_GetError());
    }
}

Cpicture* CGL2Dprimitives::get_picture(std::string name)
{
  std::list<Cpicture*>::iterator iter;

  iter = m_picturelist.begin();
  while (iter != m_picturelist.end())
    {
      if ((*iter)->is_name(name))
	{
	  return *iter;
	}
      iter++;
    }
  return NULL;
}

bool CGL2Dprimitives::get_texture_size(std::string name, t_coord *pdim)
{
  Cpicture* pp;

  pp = get_picture(name);
  if (pp != NULL)
    {
      *pdim = pp->m_dim;
      return true;
    }
  pdim->x = pdim->y = -1;
  return false;
}

Cpicture* CGL2Dprimitives::create_empty_texture(int w, int h, string tname)
{
  SDL_Surface *img;
  t_coord      dim;
  GLuint       texture_id;
  int          depth;
  Uint32       rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
  depth = 32;
  dim.x = w;
  dim.y = h;
  dim.z = 0;
  img = SDL_CreateRGBSurface(0, dim.x, dim.y, depth, rmask, gmask, bmask, amask);
  texture_id = create_OpenGL_texture(img, tname);
  return (new Cpicture(img, dim, tname, texture_id));
}

void CGL2Dprimitives::add_empty_texture(int w, int h, std::string name)
{
  Cpicture *pp;

  pp = create_empty_texture(w, h, name);
  add_picture(pp);
}

void CGL2Dprimitives::create_work_texture()
{
  string tname("work");

  m_transfert_texture = create_empty_texture(m_w, m_h, tname);
}

void CGL2Dprimitives::destroy_work_texture()
{
  if (m_transfert_texture)
    {
      delete m_transfert_texture;
      m_transfert_texture = NULL;
    }
}

// Loads a surface into the graphic memory.
GLuint CGL2Dprimitives::create_OpenGL_texture(SDL_Surface *psurface, std::string name)
{
  GLuint  textureID;
  t_coord dim;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  dim.x = psurface->clip_rect.w;
  dim.y = psurface->clip_rect.h;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.x, dim.y, 0, GL_RGBA,
	       GL_UNSIGNED_BYTE, psurface->pixels);
  glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
  error_handling(__LINE__);
  return (textureID);
}

void CGL2Dprimitives::add_picture(Cpicture* pp)
{
  m_picturelist.push_front(pp);
}

void CGL2Dprimitives::texture_quad(t_fcoord *points, GLuint tidentifier, t_fcoord *ptcoord, bool blending)
{
  int      i;
  t_fcoord coord[4];

  // Texture coordinates, pos
  if (ptcoord == NULL)
    {
      ptcoord = coord;
      ptcoord[0].x = 0;
      ptcoord[0].y = 0;
      ptcoord[0].z = 0;
      //
      ptcoord[1].x = 0;
      ptcoord[1].y = 1;
      ptcoord[1].z = 0;
      //
      ptcoord[2].x = 1;
      ptcoord[2].y = 1;
      ptcoord[2].z = 0;
      //
      ptcoord[3].x = 1;
      ptcoord[3].y = 0;
      ptcoord[3].z = 0;
    }
  enable_blending(blending);
  // Texture shaders
//#define USECOLOR
#ifdef USECOLOR
  glUseProgram(m_sh.program_color);

  color2openGl(0xFFEDADDE);
  glUniformMatrix4fv(m_sh.uniform_mvp, 1, GL_FALSE, m_projection_matrix);
  glUniform4fv(m_sh.uniform_color, 1, m_color);

  //
  // Copy the 4 points
  for (i = 0; i < 4; i++)
    {
      m_vertex_buffer_data[i * 3 + 0] = points[i].x;
      m_vertex_buffer_data[i * 3 + 1] = points[i].y;
      m_vertex_buffer_data[i * 3 + 2] = 0;//points[i].z;
    }
  //
  // vertex and texture coorinates
  glEnableVertexAttribArray(m_sh.attribute_vertpos);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), m_vertex_buffer_data, GL_STATIC_DRAW);
  //  
  glVertexAttribPointer(m_sh.attribute_vertpos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  // Set the triangle indexes
  m_triangles[0] = 0;
  m_triangles[1] = 1;
  m_triangles[2] = 2;
  m_triangles[3] = 0;
  m_triangles[4] = 2;
  m_triangles[5] = 3;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_triangles);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), m_triangles, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(m_sh.attribute_vertpos);
#else
  glUseProgram(m_sh.program_texture);
  glActiveTexture(GL_TEXTURE0); // Use texture unit 0
  glUniform1i(m_sh.uniform_text_unit, /*GL_TEXTURE*/0);
  glBindTexture(GL_TEXTURE_2D, tidentifier);
  //
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //
  // Projection matrix 
  // Set the matrice uniform for the vertex shader
  glUniformMatrix4fv(m_sh.uniform_text_mvp, 1, GL_FALSE, m_projection_matrix);
  //
  // Copy the 4 points
  for (i = 0; i < 4; i++)
    {
      m_vertex_buffer_data[i * 5 + 0] = points[i].x;
      m_vertex_buffer_data[i * 5 + 1] = points[i].y;
      m_vertex_buffer_data[i * 5 + 2] = points[i].z;
      m_vertex_buffer_data[i * 5 + 3] = ptcoord[i].x;
      m_vertex_buffer_data[i * 5 + 4] = ptcoord[i].y;
    }
  //
  // vertex and texture coorinates
  glEnableVertexAttribArray(m_sh.attribute_text_vertpos);
  glEnableVertexAttribArray(m_sh.attribute_text_coord0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(GLfloat), m_vertex_buffer_data, GL_STATIC_DRAW);
  //  
  glVertexAttribPointer(m_sh.attribute_text_vertpos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
  glVertexAttribPointer(m_sh.attribute_text_coord0,  2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  // Set the triangle indexes
  m_triangles[0] = 0;
  m_triangles[1] = 1;
  m_triangles[2] = 2;
  m_triangles[3] = 0;
  m_triangles[4] = 2;
  m_triangles[5] = 3;  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_triangles);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), m_triangles, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  //
  glDisableVertexAttribArray(m_sh.attribute_text_vertpos);
  glDisableVertexAttribArray(m_sh.attribute_text_coord0);
  glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

void CGL2Dprimitives::texture_rectangle(t_fcoord pos, t_fcoord dim, GLuint tidentifier, t_fcoord *ptcoord, bool blending)
{
  t_fcoord vcoord[4];

  //
  // Create the 4 points around the box
  vcoord[0].x = pos.x;
  vcoord[0].y = pos.y;
  vcoord[0].z = 0;
  //
  vcoord[1].x = pos.x;
  vcoord[1].y = pos.y + dim.y;
  vcoord[1].z = 0;
  //
  vcoord[2].x = pos.x + dim.x;
  vcoord[2].y = pos.y + dim.y;
  vcoord[2].z = 0;
  //
  vcoord[3].x = pos.x + dim.x;
  vcoord[3].y = pos.y;
  vcoord[3].z = 0;
  texture_quad(vcoord, tidentifier, ptcoord, blending);
}

void CGL2Dprimitives::draw_key(string texturename, t_fcoord pos, t_fcoord dim, bool blending)
{
  Cpicture *pp;
  t_fcoord  quad_texture_coord[4];

  pp = get_picture(texturename);
  if (pp != NULL)
    {
#ifndef __ANDROID__
      glEnable(GL_MULTISAMPLE);
#endif
      quad_texture_coord[0].x = 1;
      quad_texture_coord[0].y = 0;
      quad_texture_coord[0].z = 0;
      //
      quad_texture_coord[1].x = 0;
      quad_texture_coord[1].y = 0;
      quad_texture_coord[1].z = 0;
      //
      quad_texture_coord[2].x = 0;
      quad_texture_coord[2].y = 1;
      quad_texture_coord[2].z = 0;
      //
      quad_texture_coord[3].x = 1;
      quad_texture_coord[3].y = 1;
      quad_texture_coord[3].z = 0;
      //
      texture_rectangle(pos, dim, pp->m_OpenGL_texture_id, quad_texture_coord, blending);
#ifndef __ANDROID__
      glDisable(GL_MULTISAMPLE);
#endif
      error_handling(__LINE__);
    }
}

void CGL2Dprimitives::draw_texture(std::string name, t_fcoord pos, t_fcoord dim, bool filtering, bool blend, t_fcoord *psubdim, t_fcoord *poffset)
{
  Cpicture *pp;
  t_fcoord  quad_texture_coord[4];
  t_fcoord  subdim, offset;

  pp = get_picture(name);
  if (pp != NULL)
    {
      // Translate to 0-1 values
      if (psubdim != NULL)
	{
	  subdim.x = psubdim->x / (float)pp->m_dim.x;
	  subdim.y = psubdim->y / (float)pp->m_dim.y;
	}
      else
	subdim.x = subdim.y = 1.;
      if (poffset != NULL)
	{
	  offset.x = poffset->x / (float)pp->m_dim.x;
	  offset.y = poffset->y / (float)pp->m_dim.y;
	}
      else
	offset.x = offset.y = 0;
      //
      quad_texture_coord[0].x = offset.x;
      quad_texture_coord[0].y = offset.y;
      quad_texture_coord[0].z = 0;
      //
      quad_texture_coord[1].x = offset.x;
      quad_texture_coord[1].y = subdim.y;
      quad_texture_coord[1].z = 0;
      //
      quad_texture_coord[2].x = subdim.x;
      quad_texture_coord[2].y = subdim.y;
      quad_texture_coord[2].z = 0;
      //
      quad_texture_coord[3].x = subdim.x;
      quad_texture_coord[3].y = offset.y;
      quad_texture_coord[3].z = 0;
      //
      texture_rectangle(pos, dim, pp->m_OpenGL_texture_id, quad_texture_coord, blend);
    }
  else
    printf("Error: \"%s\" texture not found.\n", name.c_str());
}

void CGL2Dprimitives::draw_texture_quad(std::string name, t_fcoord *points, bool filtering, bool blend, t_fcoord *psubdim, t_fcoord *poffset)
{
  Cpicture *pp;
  t_fcoord  quad_texture_coord[4];
  t_fcoord  subdim, offset;

  pp = get_picture(name);
  if (pp != NULL)
    {
      // Translate to 0-1 values
      if (psubdim != NULL)
	{
	  subdim.x = psubdim->x / (float)pp->m_dim.x;
	  subdim.y = psubdim->y / (float)pp->m_dim.y;
	}
      else
	{
	  subdim.x = subdim.y = 1.;
	}
      if (poffset != NULL)
	{
	  offset.x = poffset->x / (float)pp->m_dim.x;
	  offset.y = poffset->y / (float)pp->m_dim.y;
	}
      else
	offset.x = offset.y = 0;
      //
      quad_texture_coord[0].x = offset.x;
      quad_texture_coord[0].y = offset.y;
      quad_texture_coord[0].z = 0;
      //
      quad_texture_coord[1].x = offset.x;
      quad_texture_coord[1].y = subdim.y;
      quad_texture_coord[1].z = 0;
      //
      quad_texture_coord[2].x = subdim.x;
      quad_texture_coord[2].y = subdim.y;
      quad_texture_coord[2].z = 0;
      //
      quad_texture_coord[3].x = subdim.x;
      quad_texture_coord[3].y = offset.y;
      quad_texture_coord[3].z = 0;
      //
      texture_quad(points, pp->m_OpenGL_texture_id, quad_texture_coord, blend);
    }
  else
    printf("Error: \"%s\" texture not found.\n", name.c_str());
}

void CGL2Dprimitives::surface_to_texture(Cpicture *dsttext, int width, int height, void *pmap, t_fcoord *subdim)
{
  glBindTexture(GL_TEXTURE_2D, dsttext->m_OpenGL_texture_id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
		  GL_UNSIGNED_BYTE, pmap);
  glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);  
  subdim->x = (float)width / (float)dsttext->m_dim.x;
  subdim->y = (float)height / (float)dsttext->m_dim.y;
  subdim->z = 0;
  error_handling(__LINE__);
}

void CGL2Dprimitives::update_texture_texels(std::string name, int width, int height, void *pmap)
{
  Cpicture *pp;
  t_fcoord  subdim;

  pp = get_picture(name);
  if (pp != NULL)
    {
      surface_to_texture(pp, width, height, pmap, &subdim);
    }
}

void CGL2Dprimitives::surface_to_screen2(t_fcoord pos, t_fcoord dim, int width, int height, void *pmap, bool blending)
{
  t_fcoord subdim;
  t_fcoord  quad_texture_coord[4];

  surface_to_texture(m_transfert_texture, width, height, pmap, &subdim);
  //
  quad_texture_coord[0].x = 0;
  quad_texture_coord[0].y = 0;
  quad_texture_coord[0].z = 0;
  //
  quad_texture_coord[1].x = 0;
  quad_texture_coord[1].y = subdim.y;
  quad_texture_coord[1].z = 0;
  //
  quad_texture_coord[2].x = subdim.x;
  quad_texture_coord[2].y = subdim.y;
  quad_texture_coord[2].z = 0;
  //
  quad_texture_coord[3].x = subdim.x;
  quad_texture_coord[3].y = 0;
  quad_texture_coord[3].z = 0;
  //
  texture_rectangle(pos, dim, m_transfert_texture->m_OpenGL_texture_id, quad_texture_coord, blending);
}

void CGL2Dprimitives::surface_to_screen2(t_fcoord pos, t_fcoord dim, SDL_Surface *surface, bool blending)
{
  surface_to_screen2(pos, dim, surface->w, surface->h, surface->pixels, blending);
}

void CGL2Dprimitives::pixdata_to_screen_area(Cgfxarea *pw, int width, int height, void *pmap)
{
  t_fcoord pos, dim;

  pw->get_posf(&pos);
  pw->get_dimf(&dim);
  surface_to_screen2(pos, dim, width, height, pmap);
}

bool CGL2Dprimitives::get_textSize(char *str, TTF_Font *pfont, t_fcoord *pfdim)
{
  int w, h;

  if (TTF_SizeText(pfont, str, &w, &h) == 0)
    {
      pfdim->x = w;
      pfdim->y = h;
      return true;
    }
  return false;
}

// Text
void CGL2Dprimitives::print(char *str, TTF_Font *pfont, t_fcoord pos, int colorin, bool blended, float outline, int colorout)
{
  t_fcoord tdim;

  if (get_textSize(str, pfont, &tdim))
    {
      print(str, pfont, pos, tdim, colorin, blended, outline, colorout);
    }
}

void CGL2Dprimitives::print(char *str, TTF_Font *pfont, t_fcoord pos, t_fcoord dim, int colorin, bool blended, float outline, int colorout)
{
  SDL_Color   c;
  SDL_Color   cbg;
  SDL_Surface *surface;
  t_fcoord    outpos;

  if (outline > 0)
    {
      cbg.r = (colorout >> 16) & 0xFF;
      cbg.g = (colorout >> 8)  & 0xFF;
      cbg.b = colorout & 0xFF;
      //cbg.a = (colorout >> 24) & 0xFF;
      cbg.a = 0xFF; 
      TTF_SetFontOutline(pfont, outline);
      if (blended)
	surface = TTF_RenderText_Blended(pfont, str, cbg);
      else
	surface = TTF_RenderText_Solid(pfont, str, cbg);
      SDL_Surface *img = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
      SDL_FreeSurface(surface);
      outpos = pos;
      //outpos.x += (dim.x - surface->w) / 2;
      surface_to_screen2(outpos, dim, img->w, img->h, img->pixels, blended);
      SDL_FreeSurface(img);
      TTF_SetFontOutline(pfont, 0);

      pos.x += outline;
      pos.y += 2 * outline;
      dim.x -= 2 * outline;
      dim.y -= 2 * outline;
    }
  c.r = (colorin >> 16) & 0xFF;
  c.g = (colorin >> 8)  & 0xFF;
  c.b = colorin & 0xFF;
  //c.a = (colorin >> 24) & 0xFF;
  c.a = 0xFF;
  if (blended)
    surface = TTF_RenderText_Blended(pfont, str, c);
  else
    surface = TTF_RenderText_Solid(pfont, str, c);
  SDL_Surface *img = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
  SDL_FreeSurface(surface);  
  //pos.x += (dim.x - surface->w) / 2;
  surface_to_screen2(pos, dim, img->w, img->h, img->pixels, blended);
  SDL_FreeSurface(img);
}

#define I256TOFLOAT10(C) (((float)C) / 255.)

void CGL2Dprimitives::color2openGl(int color)
{
  unsigned char R, G, B, A;

  R = color & 0xFF;
  G = (color >> 8) & 0xFF;
  B = (color >> 16) & 0xFF;
  A = (color >> 24) & 0xFF;
  m_color[0] = I256TOFLOAT10(R);
  m_color[1] = I256TOFLOAT10(G);
  m_color[2] = I256TOFLOAT10(B);
  m_color[3] = I256TOFLOAT10(A);
}

void CGL2Dprimitives::hline(float x, float y, float width, int color)
{
  if (width == 0)
    return;
  box(x, y, x + width, y + 1, color, false);
  //line(x, y, x + width, y, color);
}

void CGL2Dprimitives::vline(float x, float y, float height, int color)
{
  if (height == 0)
    return;
  box(x, y, x + 1, y + height, color, false);
  //line(x, y, x , y + 10/*height*/, color);
}

// called only twice in Scoreview
void CGL2Dprimitives::line(float xsta, float ysta, float xsto, float ysto, int color)
{
  int i;

  // FIXME replace with a polygon?

  enable_blending(false);
  color2openGl(color);
  // Shader program selection
  glUseProgram(m_sh.program_color);
  // Set the matrix uniform for the vertex shader
  glUniformMatrix4fv(m_sh.uniform_mvp, 1, GL_FALSE, m_projection_matrix);
  // Set the color uniform for the fragment shader
  glUniform4fv(m_sh.uniform_color, 1, m_color);
  //
  // Enable the array
  glEnableVertexAttribArray(m_sh.attribute_vertpos);
  // Copy the 2 points
  i = 0;
  m_vertex_buffer_data[i++] = xsta;
  m_vertex_buffer_data[i++] = ysta;
  m_vertex_buffer_data[i++] = 0;
  m_vertex_buffer_data[i++] = xsto;
  m_vertex_buffer_data[i++] = ysto;
  m_vertex_buffer_data[i++] = 0;
  // Set the position attribute
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, i * sizeof(GLfloat), m_vertex_buffer_data, GL_STATIC_DRAW);
  glVertexAttribPointer(m_sh.attribute_vertpos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glDrawArrays(GL_LINES, 0, i / 2); // Starting from vertex 0; i vertices
  glDisableVertexAttribArray(m_sh.attribute_vertpos);
  //
  color2openGl(IDENTCOLOR);
  //error_handling(__LINE__);
}

// Set of triangles only
void CGL2Dprimitives::polygon(t_fcoord *ppointarr, int points, int color, bool bantialiased)
{
  int i, p;

  enable_blending(true);
  color2openGl(color);
#ifndef __ANDROID__
  if (bantialiased)
    glEnable(GL_MULTISAMPLE);
#endif
  //
  glUseProgram(m_sh.program_color);
  //
  // Set the matrice uniform for the vertex shader
  glUniformMatrix4fv(m_sh.uniform_mvp, 1, GL_FALSE, m_projection_matrix);
  // Set the color uniform for the fragment shader
  glUniform4fv(m_sh.uniform_color, 1, m_color);
  //
  i = p = 0;
  while (i < MAX_GL_VERTEX_ARRAY_SIZE && p < points)
    {
      // Copy the point
      m_vertex_buffer_data[i++] = ppointarr[p].x;
      m_vertex_buffer_data[i++] = ppointarr[p].y;
      m_vertex_buffer_data[i++] = 0;
      p++;
    }
  //
  // Enable the array
  glEnableVertexAttribArray(m_sh.attribute_vertpos);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, i * sizeof(GLfloat), m_vertex_buffer_data, GL_STATIC_DRAW);
  // Set the position attribute
  glVertexAttribPointer(m_sh.attribute_vertpos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glDrawArrays(GL_TRIANGLES, 0, i / 3); // Starting from vertex 0; i vertices
  // Disable the array
  glDisableVertexAttribArray(m_sh.attribute_vertpos);
  //
  color2openGl(IDENTCOLOR);
  enable_blending(false);
#ifndef __ANDROID__
  if (bantialiased)
    glDisable(GL_MULTISAMPLE);
#endif
}

void CGL2Dprimitives::triangle_mesh2D(t_fcoord pos, t_fcoord dim, CMesh *pmesh, int color, bool bantialiased)
{
  t_triangle *ptriangle;
  t_fcoord    p1, p2, p3;
  t_fcoord    meshdim;
  float       xratio, yratio;
  bool        bret;
  int         i;
  const int   components = 3;

  // FIXME-

  meshdim = pmesh->get_size();
  xratio = dim.x / meshdim.x;  // Scaling from object dimensions to drawing box dimensions
  yratio = dim.y / meshdim.y;
  enable_blending(true);
  color2openGl(color);

//   // Project the object inside the box
//   glMatrixMode(GL_PROJECTION);
//   glPushMatrix();
//   glTranslatef(pos.x, pos.y, 0);
//   glScaled(xratio, -yratio, 1.);
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();

  // Shader program selection
  glUseProgram(m_sh.program_color);
  //
  // Project in the current window dimension
  push_projection_matrix();
  glm::mat4 transform = glm::mat4(1.0f);
  memcpy(glm::value_ptr(transform), m_projection_matrix, sizeof(m_projection_matrix));
  transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0));
  transform = glm::scale(transform, glm::vec3(xratio, -yratio, 1.));
  memcpy(m_projection_matrix, glm::value_ptr(transform), sizeof(m_projection_matrix));
  //
  // Set the matrice uniform for the vertex shader
  glUniformMatrix4fv(m_sh.uniform_mvp, 1, GL_FALSE, m_projection_matrix);
  // Set the color uniform for the fragment shader
  glUniform4fv(m_sh.uniform_color, 1, m_color);
  //
#ifndef __ANDROID__
  glEnable(GL_MULTISAMPLE);
#endif
  //
  ptriangle = pmesh->get_first_triangle();
  i = 0;
  while (ptriangle != NULL && i < MAX_GL_VERTEX_ARRAY_SIZE)
    {
      bret = pmesh->get_point(ptriangle->v1, &p1);
      bret = bret && pmesh->get_point(ptriangle->v2, &p2);
      bret = bret && pmesh->get_point(ptriangle->v3, &p3);
      if (!bret)
	{
	  printf("Error in 2D primitives: get_point failed\n.");
	  return ;
	}
      // Copy the points
      m_vertex_buffer_data[i++] = p1.x;
      m_vertex_buffer_data[i++] = p1.y;
      m_vertex_buffer_data[i++] = 0;
      m_vertex_buffer_data[i++] = p2.x;
      m_vertex_buffer_data[i++] = p2.y;
      m_vertex_buffer_data[i++] = 0;
      m_vertex_buffer_data[i++] = p3.x;
      m_vertex_buffer_data[i++] = p3.y;
      m_vertex_buffer_data[i++] = 0;
      //
      ptriangle = pmesh->get_next_triangle();
    }
  // Enable the array
  glEnableVertexAttribArray(m_sh.attribute_vertpos);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, i * sizeof(GLfloat), m_vertex_buffer_data, GL_STATIC_DRAW);
  // Set the position attribute
  glVertexAttribPointer(m_sh.attribute_vertpos, components, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glDrawArrays(GL_TRIANGLES, 0, i / 3); // Starting from vertex 0; i vertices
  // Disable the array
  glDisableVertexAttribArray(m_sh.attribute_vertpos);
  //
#ifndef __ANDROID__
  glDisable(GL_MULTISAMPLE);
#endif
  // Return to the current projection
  pop_projection_matrix();
  //
  //color2openGl(IDENTCOLOR);
  enable_blending(false);
  error_handling(__LINE__);
}

void CGL2Dprimitives::disc(t_fcoord center, float radius, int color, bool bantialiased)
{
  const int cnpts = 32;
  t_fcoord  points[cnpts];
  int       i, p;
  float     angle;
  const int cntripts = 3 * (cnpts - 2);
  t_fcoord  points_tri[cntripts];

  for (i = 0, angle = (2. * M_PI) / (float)cnpts; i < cnpts; i++)
    {      
      points[i].x = center.x + radius * cos(2. * M_PI - angle * (float)i);
      points[i].y = center.y + radius * sin(2. * M_PI - angle * (float)i);
      points[i].z = 0;
    }
  // Create triangles
  for (i = 0, p = 1; i < cntripts; i += 3, p++)
    {
      points_tri[i] = points[0];
      points_tri[i + 1] = points[p + 1];
      points_tri[i + 2] = points[p + 0];
    }
  polygon(points_tri, cntripts, color, bantialiased);
}

void CGL2Dprimitives::rectangle(t_fcoord pos, t_fcoord dim, int color, bool bantialiased)
{
  enable_blending(true);
  vline(pos.x, pos.y, dim.y, color);
  vline(pos.x + dim.x, pos.y, dim.y, color);
  hline(pos.x, pos.y, dim.x, color);
  hline(pos.x, pos.y + dim.y, dim.x, color);
  enable_blending(false);
}

void CGL2Dprimitives::rectangle(float xstart, float ystart, float xstop, float ystop, int color, bool bantialiased)
{
  t_fcoord pos, dim;

  pos.x = xstart;
  pos.y = ystart;
  dim.x = xstop - xstart;
  dim.y = ystop - ystart;
  rectangle(pos, dim, color, bantialiased);
}

void CGL2Dprimitives::box(float xstart, float ystart, float xstop, float ystop, int color, bool bantialiased)
{
  t_fcoord pos, dim;

  pos.x = xstart;
  pos.y = ystart;
  dim.x = xstop - xstart;
  dim.y = ystop - ystart;
  box(pos, dim, color, bantialiased);
}

void CGL2Dprimitives::box(t_fcoord pos, t_fcoord dim, int color, bool bantialiased)
{
  const int cnpts = 6;
  t_fcoord  points[cnpts];
  t_fcoord  p1, p2, p3, p4;

  p1 = p2 = p3 = p4 = pos;
  p2.x += dim.x;
  p3.x += dim.x;
  p3.y += dim.y;
  p4.y += dim.y;
  points[0] = p1;
  points[1] = p3;
  points[2] = p2;
  points[3] = p1;
  points[4] = p4;
  points[5] = p3;
  polygon(points, cnpts, color, bantialiased);
}

void CGL2Dprimitives::add_circular_contour(int *pindex, t_fcoord center, float angle_start, float angle_stop, float radius)
{
  const int csegments = 8;
  int       i;
  float     da, angle;
  t_fcoord  point;

  da = (angle_stop - angle_start) / (float)csegments;
  for (i = 1; i < csegments; i++)
    {
      angle = (float)i * da;
      point.x = center.x + radius * cos(angle_start + angle);
      point.y = center.y + radius * sin(angle_start + angle);
      m_reallocMesh.add_vertex(point, (*pindex));
      (*pindex)++;
    }
}

void CGL2Dprimitives::rounded_box(t_fcoord pos, t_fcoord dim, int color, bool bantialiased, float radius)
{
  t_fcoord centerdim, center, pivot;
  t_fcoord point;
  int      i, nbpoints;
  int      indexes[12];

  //box(pos, dim, color, bantialiased);
  //return;
  centerdim.x = dim.x - 2. * radius;
  centerdim.y = dim.y - 2. * radius;  
  center = dim.x / 2.;
  center = dim.y / 2.;
  m_reallocMesh.clear();
  // A list of border points
  i = 1;
  // Center
  m_reallocMesh.add_vertex(center, i++);
  // Top
  point.x = radius;
  point.y = 0;
  m_reallocMesh.add_vertex(point, i++);
  point.x = radius + centerdim.x;
  point.y = 0;
  m_reallocMesh.add_vertex(point, i++);
  pivot.x = radius + centerdim.x;
  pivot.y = -radius;
  add_circular_contour(&i, pivot, M_PI / 2., 0, radius);
  // Right
  point.x = dim.x;
  point.y = -radius;
  m_reallocMesh.add_vertex(point, i++);
  point.x = dim.x;
  point.y = -(radius + centerdim.y);
  m_reallocMesh.add_vertex(point, i++);
  pivot.x = radius + centerdim.x;
  pivot.y = -(radius + centerdim.y);
  add_circular_contour(&i, pivot, 0, -M_PI / 2., radius);
  // Bottom
  point.x = radius + centerdim.x;
  point.y = -(dim.y);
  m_reallocMesh.add_vertex(point, i++);
  point.x = radius;
  pivot.y = -(dim.y);
  m_reallocMesh.add_vertex(point, i++);
  pivot.x = radius;
  pivot.y = -(radius + centerdim.y);
  add_circular_contour(&i, pivot, -M_PI / 2., -M_PI, radius);
  // Left
  point.x = 0;
  point.y = -(radius + centerdim.y);
  m_reallocMesh.add_vertex(point, i++);
  point.x = 0;
  pivot.y = -(radius);
  m_reallocMesh.add_vertex(point, i++);
  pivot.x = radius;
  pivot.y = -(radius);
  add_circular_contour(&i, pivot, M_PI, M_PI / 2., radius);
  // Add the triangles
  nbpoints = i;
  indexes[0] = 1;
  for (i = 2; i < nbpoints; i++)
    {
      indexes[3] = (i + 1) < nbpoints? i + 1 : 2;
      indexes[6] = i;
      m_reallocMesh.add_face(indexes, 9);
    }
  // Draw it
  triangle_mesh2D(pos, dim, &m_reallocMesh, color, bantialiased);
}

void CGL2Dprimitives::circle(t_fcoord center, float radius, float thickness, int color, bool bantialiased)
{
  const int cnpts = 32;
  t_fcoord  point, pivot, pos, dim;
  int       i, v, last, current;
  float     angle;
  int       indexes[12];

  m_reallocMesh.clear();
  angle = (2. * M_PI) / (float)cnpts;
  pivot.x = radius;
  pivot.y = -radius;  
  point.z = pivot.z = 0;
  for (i = 0, v = 1; i < cnpts; i++)
    {
      point.x = pivot.x + radius * cos(2. * M_PI - angle * (float)i);
      point.y = pivot.y + radius * sin(2. * M_PI - angle * (float)i);
      m_reallocMesh.add_vertex(point, v);
      point.x = pivot.x + (radius - thickness) * cos(2. * M_PI - angle * (float)i);
      point.y = pivot.y + (radius - thickness) * sin(2. * M_PI - angle * (float)i);
      m_reallocMesh.add_vertex(point, v + 1);
      v += 2;
    }
  last = v - 2;
  current = 1;
  for (i = 0; i < cnpts; i++)
    {
      indexes[0] = last;
      indexes[3] = current;
      indexes[6] = current + 1;
      indexes[9] = last + 1;
      m_reallocMesh.add_face(indexes, 12);
      last = current;
      current += 2;
    }
  //
  pos.x = center.x - radius;
  pos.y = center.y - radius;
  dim.x = dim.y = 2. * radius;
  triangle_mesh2D(pos, dim, &m_reallocMesh, color, bantialiased);
}

