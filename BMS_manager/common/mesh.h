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
#define MAX_VERTICES 4096

typedef struct s_triangle
{
  int v1;
  int v2;
  int v3;
}              t_triangle;

class CMesh
{
 public:
  CMesh();
  ~CMesh();

  std::string get_name();

  void add_object(std::string objectName);
  void add_group(std::string groupName);
  void add_vertex(t_fcoord point, int index);
  void add_face(int *pindexes, int num);
  void add_face(int *pindexes, int num, int facenum);
  void setsmooth(bool enabled, int group);
  t_triangle* get_first_triangle();
  t_triangle* get_next_triangle();
  bool        get_point(unsigned int index, t_fcoord *ppoint);
  t_fcoord    get_size();

  void set_name(std::string name);
  void clear();

 private:
  std::string                       m_objname;
  std::vector<t_fcoord>             m_points;
  std::vector<t_triangle>           m_triangles;
  std::vector<t_triangle>::iterator m_titer;
  bool                              m_smooth;
};

class CMeshList
{
 public:
  CMeshList();
  ~CMeshList();

  bool   Load_meshes_from(char *path, char *file_name);
  CMesh* get_mesh(std::string name);

 private:
  std::list<CMesh*> m_mesh_list;
};

