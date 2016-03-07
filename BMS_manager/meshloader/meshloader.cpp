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
#include <stdio.h>
#include <string>
#include <list>
#include <vector>

#include "gfxareas.h"
#include "mesh.h"
#include "parser.h"

using namespace std;

bool load_meshes(char *path, char *file_name, std::list<CMesh*> *pmeshlist)
{
  Cparser *pparser;
  bool     ret;
  string   dir;

  ret = false;
  pparser = new Cparser();
  dir = string(path) + string(file_name);
  if (pparser->open_data(dir.c_str()) == 0)
    {
      ret = pparser->build_meshes(pmeshlist);
      pparser->close_data();
    }
  else
    printf("Loading of \"%s\" failed.\n", dir.c_str());
  delete pparser;
  return ret;
}

