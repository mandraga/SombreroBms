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
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

#include <stdio.h>
#include <string.h>
#include <string>
#include <list>
#include <vector>

#include "gfxareas.h"
#include "mesh.h"
#include "parse_codes.h"
#include "parser.h"

#define CMP_MEMUSE_STR(s) (strlen(yytext) == strlen(s) && strncmp(yytext, s, strlen(s)) == 0)
//#define VERBOSAL

// Uses only lex
extern FILE *yyin;
extern char *yytext;
extern int  yyleng;
extern int  num_line;

extern "C" {
int yylex(void);
}

using namespace std;

Cparser::Cparser():
  m_state(get_new_element),
  m_curpmesh(NULL),
  m_numvertex(0),
  m_numvtext(0),
  m_numfnormal(0),
  m_numface(0)
{
  t_fcoord point;

  m_vertexes.push_back(point); // First one not used, set to (0, 0, 0)
  strcpy(m_error_str, "");
}

Cparser::~Cparser()
{
}

int Cparser::open_data(const char *file_name)
{
  yyin = fopen(file_name, "r");
  if (yyin == NULL)
    {
      snprintf(m_error_str, sizeof(m_error_str), "open file %s", file_name);
      return 1;
    }
  return 0;
}

int Cparser::close_data()
{
  if (yyin == NULL)
    {
      snprintf(m_error_str, sizeof(m_error_str), "file already closed");
      return 1;
    }
  if (fclose(yyin) != 0)
    {
      snprintf(m_error_str, sizeof(m_error_str), "could not close file");
      return 1;
    }
  yyin = NULL;
  return 0;
}

void Cparser::reset_coord()
{
  int i;

  m_ccordctr = 0;
  for (i = 0; i < MAX_COORD; i++)
    m_coord[i] = 0;
  m_sign = 1;
}

void Cparser::get_new(int res)
{
  switch (res)
    {
    case COMMENT:
    case EOL:
      break;
    case VERTEX:
      m_numvertex++;
      reset_coord();
      m_state = state_vertex;
      break;
    case TEXTURE_COORD:
      m_numvtext++;
      reset_coord();
      m_state = state_vertextext;
      break;
    case NORMAL:
      m_numfnormal++;
      reset_coord();
      m_state = state_vertexnormal;
      break;
    case FREEFORM:
      m_state = state_freeform;
      break;
    case FACE:
      m_numface++;
      reset_face();
      m_state = state_face;
      break;
    case MATERIAL_FILE:
      m_state = state_mtllibname;
      break;
    case MATERIAL_NAME:
      m_state = state_mtlfilename;
      break;
    case OBJECT_NAME:
      m_state = state_objectname;
      break;
    case OBJECT_GROUP_NAME:
      m_state = state_groupname;
      break;
    case SMOOTH_SHADING:
      m_state = state_smooth;
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d, no op type", num_line);
      throw (1);
    };
}

void Cparser::get_objectname(int res)
{
  switch (res)
    {
    case EOL:
      m_state = get_new_element;
      break;
    case OBJECT_NAME:
      strncpy(m_str, yytext, sizeof(m_str));
#ifdef VERBOSAL
      printf("adding object name: \"%s\"\n", m_str);
#endif
      m_curpmesh = new CMesh();
      m_curpmesh->add_object(string(m_str));
      m_meshlist.push_back(m_curpmesh);
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::get_groupname(int res)
{
  switch (res)
    {
    case EOL:
      m_state = get_new_element;
      break;
    case OBJECT_NAME:
      strncpy(m_str, yytext, sizeof(m_str));
      m_curpmesh->add_group(string(m_str));
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::get_smooth_group(int res)
{
  int group;

  switch (res)
    {
    case EOL:
      m_state = get_new_element;
      break;
    case SMOOTH_OFF:
      m_curpmesh->setsmooth(false, 0);
      break;
    case SMOOTH_GROUP:
      sscanf(yytext,"%d", &group);
      m_curpmesh->setsmooth(true, group);
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::get_vertex(int res)
{
  float    v;
  t_fcoord point;

  switch (res)
    {
    case EOL:
      point.x = m_coord[0];
      point.y = m_coord[1];
      point.z = m_coord[2];
#ifdef VERBOSAL
      printf("adding vertex %d: %f %f %f\n", m_numvertex, point.x,  point.y,  point.z);
#endif
      m_vertexes.push_back(point);
      //m_pmesh->add_vertex(point, m_numvertex);
      m_state = get_new_element;
      break;
    case SIGN:
      m_sign = -1;
      break;
    case FLOAT:
      if (m_ccordctr >= MAX_COORD)
	{
	  snprintf(m_error_str, sizeof(m_error_str), "parser line %d coordinates overflow", num_line);
	  throw (1);
	}
      sscanf(yytext,"%f", &v);
      m_coord[m_ccordctr] = m_sign * v;
      m_ccordctr++;
      m_sign = 1;
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::get_vertextext(int res)
{
  float    v;
  t_fcoord point;

  switch (res)
    {
    case EOL:
      point.x = m_coord[0];
      point.y = m_coord[1];
      point.z = m_coord[2];
      //m_pmesh->add_vtexture(point, m_numvtext);
      m_state = get_new_element;
      break;
    case SIGN:
      m_sign = -1;
      break;
    case FLOAT:
      if (m_ccordctr >= MAX_COORD)
	{
	  snprintf(m_error_str, sizeof(m_error_str), "parser line %d coordinates overflow", num_line);
	  throw (1);
	}
      sscanf(yytext,"%f", &v);
      m_coord[m_ccordctr] = m_sign * v;
      m_ccordctr++;
      m_sign = 1;
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::get_normal(int res)
{
  float    v;
  t_fcoord point;

  switch (res)
    {
    case EOL:
      point.x = m_coord[0];
      point.y = m_coord[1];
      point.z = m_coord[2];
      //m_pmesh->add_normal(point, m_numfnormal);
      m_state = get_new_element;
      break;
    case SIGN:
      m_sign = -1;
      break;
    case FLOAT:
      if (m_ccordctr >= MAX_COORD)
	{
	  snprintf(m_error_str, sizeof(m_error_str), "parser line %d coordinates overflow", num_line);
	  throw (1);
	}
      sscanf(yytext,"%f", &v);
      m_coord[m_ccordctr] = m_sign * v;
      m_ccordctr++;
      m_sign = 1;
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::reset_face()
{
  int i;
  
  m_ccordctr = -1;
  for (i = 0; i < MAX_FACEINDEXES; i++)
    {
      m_faces[i] = 0;
    }
}

void Cparser::get_face(int res)
{
  if ((res != SPACE) && (m_ccordctr < 0))
    {
      printf("Error when reading face indexes: missign space separator.\n");
      m_ccordctr = 0;
    }
  switch (res)
    {
    case EOL:
#ifdef VERBOSAL
      printf("adding face %d: %d %d %d\n", m_numface, m_faces[0], m_faces[3], m_faces[6]);
#endif
      m_curpmesh->add_face(m_faces, m_ccordctr / 3, m_numface);
      m_state = get_new_element;
      break;
    case FACE_POINT_SEPARATOR:
      {
	m_ccordctr++;
      }
      break;
    case SPACE:
      {
	if (m_ccordctr < 0)
	  m_ccordctr = 0;
	else
	  {
	    if ((m_ccordctr % 3) == 0) // No '/' when only vertexes are stored
	      m_ccordctr += 3;
	    else
	      m_ccordctr++;
	  }
      }
      break;
    case FACE_INDEX:
      {
	if (m_ccordctr > MAX_FACEINDEXES)
	{
	  snprintf(m_error_str, sizeof(m_error_str), "parser line %d faces index overflow polygons instead of triangles or quads", num_line);
	  throw (1);    
	}
	sscanf(yytext,"%d", &m_faces[m_ccordctr]);
      }
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

void Cparser::dump_line(int res)
{
  switch (res)
    {
    case EOL:
      m_state = get_new_element;
      break;
    default:
      snprintf(m_error_str, sizeof(m_error_str), "parser line %d expects something", num_line);
      throw (1);
    };
}

bool Cparser::build_meshes(std::list<CMesh*> *pmeshlist)
{
  std::list<CMesh*>::iterator iter;
  int          res;
  unsigned int i;

  m_meshlist.clear();
  num_line = 1;
  try
    {
      while ((res = yylex()) != 0)
	{
	  switch (m_state)
	    {
	    case get_new_element:
	      get_new(res);
	      break;
	    case state_vertex:
	      get_vertex(res);
	      break;
	    case state_vertextext:
	      get_vertextext(res);
	      break;
	    case state_vertexnormal:
	      get_normal(res);
	      break;
	    case state_freeform:
	    case state_mtllibname:
	    case state_mtlfilename:
	      dump_line(res);
	      break;
	    case state_face:
	      get_face(res);
	      break;
	    case state_objectname:
	      get_objectname(res);
	      break;
	    case state_groupname:
	      get_groupname(res);
	      break;
	    case state_smooth:
	      get_smooth_group(res);
	      break;
	    };
	}
    }
  catch (int e)
    {
      return false;
    }
  // Add every vertex to every object for size purpose, assuming that all the objects are in the same object space
  iter = m_meshlist.begin();
  while (iter != m_meshlist.end())
    {
      m_curpmesh = (*iter);
      for (i = 0; i < m_vertexes.size(); i++)
	{
	  m_curpmesh->add_vertex(m_vertexes[i], i);
	}
      iter++;
    }
  iter = m_meshlist.begin();
  while (iter != m_meshlist.end())
    {
      pmeshlist->push_back(*iter);      
      iter++;
    }
  return true;
}

