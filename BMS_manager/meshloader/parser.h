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

#define TRANSFERTSTRSZ  4096
#define MAX_COORD       4
#define MAX_FACEINDEXES (4 * 3)

class Cparser
{
enum
  {
    get_new_element,
    state_vertex,
    state_vertextext,
    state_vertexnormal,
    state_freeform,
    state_mtllibname,
    state_mtlfilename,
    state_smooth,
    state_objectname,
    state_groupname,
    state_face
  };
public:
  Cparser();
  ~Cparser(); 
  int  open_data(const char *file_name);
  bool build_meshes(std::list<CMesh*> *pmeshlist);
  int  close_data();
private:
  // A function for each parsing state
  void get_new(int res);
  void dump_line(int res);
  void get_objectname(int res);
  void get_groupname(int res);
  void get_smooth_group(int res);
  void reset_coord();
  void get_vertex(int res);
  void get_vertextext(int res);
  void get_normal(int res);
  void reset_face();
  void get_face(int res);

public:
  char   m_error_str[64];
 private:
  int    m_state;
  CMesh                 *m_curpmesh;
  std::list<CMesh*>      m_meshlist;
  std::vector<t_fcoord>  m_vertexes;
  char   m_str[TRANSFERTSTRSZ];
  int    m_numvertex;
  int    m_numvtext;
  int    m_numfnormal;
  int    m_numface;
  int    m_ccordctr;
  float  m_sign;
  float  m_coord[MAX_COORD];
  int    m_faces[MAX_FACEINDEXES];
};

