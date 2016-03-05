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
#include <math.h>
#include <list>
#include <vector>
#include <iterator>
#include <assert.h>

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

#include "gfxareas.h"
#include "mesh.h"
#include "gl2Dprimitives.h"

// From the excellent OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
// Contributor: Sylvain Beucler
// Displays compilation errors from the OpenGL shader compiler
//
void print_log(GLuint object)
{
  GLint log_length = 0;
  char *log;

  if (glIsShader(object))
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  else 
    if (glIsProgram(object))
      glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else
      {
	fprintf(stderr, "printlog: Not a shader or a program\n");
	return;
      }
  log = (char*)malloc(log_length);
  if (glIsShader(object))
    glGetShaderInfoLog(object, log_length, NULL, log);
  else if (glIsProgram(object))
    glGetProgramInfoLog(object, log_length, NULL, log);
  printf("%s", log);
  free(log);
}

// This function is a modified CreateProgram() from the AMD open GLES SDK
bool CGL2Dprimitives::CreateProgram(t_gl_shader_Inouts *pgl_io)
{
  // For colored polygon rendering
  const GLchar* vsColorSource =
    "uniform mat4 mvpMatrix;"
    "attribute vec3 vertPosition;"
    "void main()"
    "{"
    "   gl_Position = mvpMatrix * vec4(vertPosition, 1.0);"
    "}";
  const GLchar* fsColorSource =
    "uniform vec4 color;"
    "void main()"
    "{"
    "  gl_FragColor = color;"
    "}";
  // Texture rendering
  const GLchar* vsTextureSource =
    "uniform mat4 mvpMatrix;"
    "attribute vec3 vertPosition;"
    "attribute vec2 texCoord0;"
    "varying vec2 vTexCoord;"
    "void main()"
    "{"
    "   gl_Position = mvpMatrix * vec4(vertPosition, 1.0);"
    "   vTexCoord   = texCoord0;"
    "}";
  const GLchar* fsTextureSource =
    "varying vec2 vTexCoord;"
    "uniform sampler2D textureUnit0;"
    "void main(void)"
    "{"
    "   gl_FragColor = texture2D(textureUnit0, vTexCoord);"
    "}";
//    " gl_FragColor = texture(textureUnit0, vTexCoord); ?????????"
//    " gl_FragColor = vec4(0.5, 0.5, 0., 1.);"

  GLint  status;
  GLuint vs;
  GLuint fs;
  GLuint po;

  //--------------------------------------------------------
  // One color
  //--------------------------------------------------------
  // create and compile the vertex shader
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vsColorSource, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
  if (status == 0)
    {
      printf("Failed to create the color vertex shader.\n");
      print_log(vs);
      glDeleteShader(vs);
      return false;
    }
  // create and compile the fragment shader
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fsColorSource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
  if (status == 0)
    {
      printf("Failed to create the color fragment shader.\n");
      print_log(fs);
      glDeleteShader(vs);
      glDeleteShader(fs);
      return false;
    }
  // create the program and attach the shaders
  po = glCreateProgram();
  if (po == 0)
    {
      printf("Failed to create a program.\n");
      return false;
    }
  glAttachShader(po, vs);
  glAttachShader(po, fs);
  // link the program
  glLinkProgram(po);
  glGetProgramiv(po, GL_LINK_STATUS, &status);
  if(!status) 
    {
      printf("Failed to link program.\n");
      print_log(po);
      glDeleteProgram(po);
      glDeleteShader(vs);
      glDeleteShader(fs);
      return false;
    }
  // Free the no longer needed shader resources
  glDeleteShader(vs);
  glDeleteShader(fs);

  // Store the identifiers
  pgl_io->program_color = po;
  pgl_io->attribute_vertpos = glGetAttribLocation(po, "vertPosition");
  pgl_io->uniform_mvp   = glGetUniformLocation(po, "mvpMatrix");
  pgl_io->uniform_color = glGetUniformLocation(po, "color");
  assert(pgl_io->attribute_vertpos >= 0);
  assert(pgl_io->uniform_mvp >= 0);
  assert(pgl_io->uniform_color >= 0);
  
  //--------------------------------------------------------
  // Textures
  //--------------------------------------------------------
  // create and compile the vertex shader
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vsTextureSource, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
  if (status == 0)
    {
      printf("Failed to create the texture vertex shader.\n");
      print_log(vs);
      glDeleteShader(vs);
      return false;
    }
  // create and compile the fragment shader
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fsTextureSource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
  if (status == 0)
    {
      printf("Failed to create the texture fragment shader.\n");
      print_log(fs);
      glDeleteShader(vs);
      glDeleteShader(fs);
      return false;
    }
  // create the program and attach the shaders
  po = glCreateProgram();
  if (po == 0)
    {
      printf("Failed to create a program.\n");
      return false;
    }
  glAttachShader(po, vs);
  glAttachShader(po, fs);
  // link the program
  glLinkProgram(po);
  glGetProgramiv(po, GL_LINK_STATUS, &status);
  if(!status) 
    {
      printf("Failed to link program.\n");
      print_log(po);
      glDeleteProgram(po);
      glDeleteShader(vs);
      glDeleteShader(fs);
      return false;
    }
  // Free the no longer needed shader resources
  glDeleteShader(vs);
  glDeleteShader(fs);
  // Store the identifiers
  pgl_io->program_texture = po;
  pgl_io->attribute_text_vertpos = glGetAttribLocation(po, "vertPosition");
  pgl_io->attribute_text_coord0 = glGetAttribLocation(po, "texCoord0");
  pgl_io->uniform_text_mvp  = glGetUniformLocation(po, "mvpMatrix");
  pgl_io->uniform_text_unit = glGetUniformLocation(po, "textureUnit0");
  assert(pgl_io->attribute_text_vertpos >= 0);
  //assert(pgl_io->attribute_text_coord0 >= 0);
  assert(pgl_io->uniform_text_mvp >= 0);
  //assert(pgl_io->uniform_text_unit >= 0);

  return true;
}
