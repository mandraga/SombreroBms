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

#include <string.h>

#include <iterator>
#include <list>

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>

#include "gfxareas.h"

//-------------------------------------------------------------
//  Stores the dimensions of the a graphic window area
//  used to create custom SDL "widgets"
//-------------------------------------------------------------
Cgfxarea::Cgfxarea(t_fcoord pos, t_fcoord dimensions, t_coord wsize, char *name):
  m_fpos(pos),
  m_fdimensions(dimensions),
  m_update(true),
  m_clicked(false)
{
  m_ratio = (dimensions.x / dimensions.y);
  resize_keep_ratio(wsize);
  //m_ratrio = (float)m_dimensions.x / (float)m_dimensions.y;
  //printf("ratio=%f\n", m_ratio);
  strcpy(m_name, name);
}

Cgfxarea::~Cgfxarea()
{
  //printf("deleting %s\n", m_name);
}

void Cgfxarea::get_pos(t_coord *pos)
{
  *pos = m_pos;
}

void Cgfxarea::get_dim(t_coord *dim)
{
  *dim = m_dimensions;
}

void Cgfxarea::get_posf(t_fcoord *pos)
{
  pos->x = m_pos.x;
  pos->y = m_pos.y;
}

void Cgfxarea::get_dimf(t_fcoord *dim)
{
  dim->x = m_dimensions.x;
  dim->y = m_dimensions.y;
}

bool Cgfxarea::is_in(t_coord pos)
{
  if (m_pos.x <= pos.x && pos.x < m_pos.x + m_dimensions.x)
    if (m_pos.y <= pos.y && pos.y < m_pos.y + m_dimensions.y)
      return true;
  return false;
}

void Cgfxarea::resize(t_coord wsize)
{
  m_pos.x = wsize.x * m_fpos.x / 100.;
  m_pos.y = wsize.y * m_fpos.y / 100.;
  m_pos.z = 0;
  m_dimensions.x = wsize.x * m_fdimensions.x / 100.;
  m_dimensions.y = wsize.y * m_fdimensions.y / 100.;
  m_dimensions.z = 0;
}

void Cgfxarea::set_ratio(float ratio)
{
  m_ratio = ratio;
}

void Cgfxarea::resize_keep_ratio(t_coord wsize)
{
  m_pos.x = wsize.x * m_fpos.x / 100.;
  m_pos.y = wsize.y * m_fpos.y / 100.;
  m_pos.z = 0;
  m_dimensions.y = wsize.y * m_fdimensions.y / 100.;
  m_dimensions.x = m_dimensions.y * m_ratio;
  m_dimensions.z = 0;
}

//-------------------------------------------------------------
// Lists gfx areas
//-------------------------------------------------------------

CgfxareaList::CgfxareaList()
{
  m_Cgfxarea_list.clear();
}

CgfxareaList::~CgfxareaList()
{
  Cgfxarea *pw;

  pw = get_first();
  while (pw != NULL)
    {
      delete pw;
      pw = get_next();
    }
  m_Cgfxarea_list.clear();
}

void CgfxareaList::add(Cgfxarea *pw)
{
  m_Cgfxarea_list.push_front(pw);
}

Cgfxarea* CgfxareaList::get_first()
{
  m_iter = m_Cgfxarea_list.begin();
  if (m_iter == m_Cgfxarea_list.end())
    return NULL;
  return (*m_iter);
}

Cgfxarea* CgfxareaList::get_next()
{
  m_iter++;
  if (m_iter == m_Cgfxarea_list.end())
    return NULL;
  return *m_iter;
}

Cgfxarea* CgfxareaList::get_area(char *pname)
{
  std::list<Cgfxarea*>::iterator iter;
  Cgfxarea *pw;

  iter = m_Cgfxarea_list.begin();
  if (iter == m_Cgfxarea_list.end())
    return NULL;
  do
    {
      pw = *iter;
      if (strcmp(pw->m_name, pname) == 0)
	{
	  return pw;
	}
      iter++;
    }
  while (iter != m_Cgfxarea_list.end());
  return NULL;
}

bool CgfxareaList::is_in(int x, int y, char *pname)
{
  Cgfxarea *pw;
  t_coord  p;

  pw = get_area(pname);
  if (pw == NULL)
    return false;
  p.x = x;
  p.y = y;
  return pw->is_in(p);
}

void CgfxareaList::resize(t_coord window_size)
{
  Cgfxarea* parea;

  parea = get_first();
  while (parea != NULL)
    {
      parea->resize(window_size);
      parea = get_next();
    }
}
