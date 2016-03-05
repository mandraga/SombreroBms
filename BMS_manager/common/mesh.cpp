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
#include <assert.h>
#include <math.h>
#include <list>
#include <vector>
#include <iterator>

#include "gfxareas.h"
#include "mesh.h"
#include "meshloader.h"

using namespace std;

CMesh::CMesh():
  m_smooth(false)
{
  t_fcoord point;

  // vertex 0 at 0
  point.x = point.y = point.z = 0;
  m_points.push_back(point);
}

CMesh::~CMesh()
{
}

string CMesh::get_name()
{
  return (m_objname);
}

void CMesh::add_object(std::string objectName)
{
  // Only one suported object
  m_objname = objectName;
}

void CMesh::add_group(std::string groupName)
{
}

void CMesh::add_vertex(t_fcoord point, int index)
{
  t_fcoord empty;

  empty.x = empty.y = empty.z = 0;
  assert(index < MAX_VERTICES);
  if (m_points.size() <= (unsigned int)index)
    m_points.resize(index + 8, empty);
  m_points[index] = point;
}

void CMesh::add_face(int *pindexes, int num)
{
  t_triangle t;

  t.v1 = pindexes[0];
  t.v2 = pindexes[3];
  t.v3 = pindexes[6];
  m_triangles.push_back(t);
  // Quad, add one triangle
  if (num / 3 == 4)
    {
      t.v1 = pindexes[0];
      t.v2 = pindexes[6];
      t.v3 = pindexes[9];
      m_triangles.push_back(t);
    }
}

void CMesh::add_face(int *pindexes, int num, int facenum)
{
  add_face(pindexes, num);
}

void CMesh::setsmooth(bool enabled, int group)
{
}

t_triangle* CMesh::get_first_triangle()
{
  m_titer = m_triangles.begin();
  if (m_titer == m_triangles.end())
    return NULL;
  return &(*m_titer);
}

t_triangle* CMesh::get_next_triangle()
{
  m_titer++;
  if (m_titer == m_triangles.end())
    return NULL;
  return &(*m_titer);  
}

bool CMesh::get_point(unsigned int index, t_fcoord *ppoint)
{
  if (index >= m_points.size())
    return false;
  *ppoint = m_points[index];
  ppoint->z = 0;
  return true;
}

t_fcoord CMesh::get_size()
{
  t_fcoord lsz, rsz, ret;
  std::vector<t_fcoord>::iterator iter;

  lsz.x = lsz.y = lsz.z = 0;
  rsz.x = rsz.y = rsz.z = 0;
  iter = m_points.begin();
  while (iter != m_points.end())
    {
      lsz.x = (*iter).x < lsz.x? (*iter).x : lsz.x;
      lsz.y = (*iter).y < lsz.y? (*iter).y : lsz.y;
      rsz.x = (*iter).x > rsz.x? (*iter).x : rsz.x;
      rsz.y = (*iter).y > rsz.y? (*iter).y : rsz.y;
      //sz.z = (*iter).z > sz.z? (*iter).z : sz.z;
      iter++;
    }
  ret.x = fabs(rsz.x - lsz.x);
  ret.y = fabs(rsz.y - lsz.y);
  return ret;
}

void CMesh::set_name(std::string name)
{
  m_objname = name;
}

void CMesh::clear()
{
  t_fcoord point;

  m_points.clear();
  m_triangles.clear();
  // vertex 0 at 0
  point.x = point.y = point.z = 0;
  m_points.push_back(point);
}

// ---------------------------------------------------------------------------


CMeshList::CMeshList()
{
}

CMeshList::~CMeshList()
{
  std::list<CMesh*>::iterator iter;

  iter = m_mesh_list.begin();
  while (iter != m_mesh_list.end())
    {
      delete (*iter);
      iter = m_mesh_list.erase(iter);
    }
}

bool CMeshList::Load_meshes_from(char *path, char *file_name)
{
  bool   ret;

  ret = load_meshes(path, file_name, &m_mesh_list);
  return ret;
}

CMesh* CMeshList::get_mesh(std::string name)
{
  std::list<CMesh*>::iterator iter;

  iter = m_mesh_list.begin();
  while (iter != m_mesh_list.end())
    {
      if ((*iter)->get_name() == name)
	return (*iter);
      iter++;
    }
  return NULL;
}

