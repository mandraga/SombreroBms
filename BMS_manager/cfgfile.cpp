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
#include <assert.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <iterator>
#include <list>
#ifdef __LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#else
#include <windows.h>
#include "Shlobj.h"
#include <direct.h>
#endif

#include <tinyxml.h>

#include "audioapi.h"
#include "cfgfile.h"

using namespace std;

Cxml_cfg_file_decoder::Cxml_cfg_file_decoder():
  m_pdoc(NULL)
{
}

Cxml_cfg_file_decoder::~Cxml_cfg_file_decoder()
{
  delete m_pdoc;
}

string Cxml_cfg_file_decoder::get_user_home()
{
  string ret;
#ifdef __LINUX
  struct passwd *pw;
 
  pw = getpwuid(getuid());
  ret = string(pw->pw_dir);
#else
#ifdef __WINDOWS
  TCHAR szFolderPath[MAX_PATH];
  
  if (!SHGetSpecialFolderPath(NULL, szFolderPath, CSIDL_APPDATA, FALSE))
    {
      printf("The user path was not found.\n");
    }
  else
    {
      ret = string(szFolderPath);
    }
#else
 adazdad
#endif
#endif
  return ret;
}

#ifdef __WINDOWS
// Taken from stack overflow, it helped many times.
bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!
  return false;    // this is not a directory!
}
#endif

string Cxml_cfg_file_decoder::get_user_config_path()
{
  string      home;
  string      cfgdir;
  string      cfgsubdir;
  bool        bisdir;
#ifdef __LINUX
  struct stat dir_stat;
  mode_t      mode;

  home = get_user_home();
  cfgdir = home + string("/.scoreview/");
  bisdir = false;
  if (stat(cfgdir.c_str(), &dir_stat) == 0)
    {
      bisdir = S_ISDIR(dir_stat.st_mode);
    }
  if (!bisdir)
    {
      mode = 0700;
      if (mkdir(cfgdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/");
      if (mkdir(cfgsubdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/addinstrumentFLTK/");
      if (mkdir(cfgsubdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/configFLTK/");
      if (mkdir(cfgsubdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/save_openFLTK/");
      if (mkdir(cfgsubdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/practiceSDL/");
      if (mkdir(cfgsubdir.c_str(), mode) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
    }
#else
#ifdef __WINDOWS
  home = get_user_home(); 
  if (!dirExists(home))
    {
      printf("Warning: the user directory \"%s\" does not exist.\n", home.c_str());
    }
  cfgdir = home + string("/scoreview/");
  bisdir = dirExists(cfgdir);
  if (!bisdir)
    {
      printf("Creating the user directory \"%s\".\n", cfgdir.c_str());
      if (mkdir(cfgdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/");
      if (mkdir(cfgsubdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/addinstrumentFLTK/");
      if (mkdir(cfgsubdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/configFLTK/");
      if (mkdir(cfgsubdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/save_openFLTK/");
      if (mkdir(cfgsubdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
      cfgsubdir = cfgdir + string("dialogs/practiceSDL/");
      if (mkdir(cfgsubdir.c_str()) != 0)
	{
	  printf("Error creating directory \"%s\" failed.\n", cfgsubdir.c_str());
	  exit(EXIT_FAILURE);
	}
    }
#else
  cfgdir = string("");
#endif
#endif 
  return cfgdir;
}

string Cxml_cfg_file_decoder::get_user_documents_path()
{
  std::string home;

#ifdef __LINUX
  home = get_user_home();
  return home;
#else
#ifdef __WINDOWS
  home = get_user_home();
  return home;
#else
  return string("");
#endif
#endif
}

bool Cxml_cfg_file_decoder::open_for_reading(std::string file_name)
{
  bool bload;

  if (m_pdoc != NULL)
    delete m_pdoc;
  m_pdoc = new TiXmlDocument(file_name);
  bload = m_pdoc->LoadFile();
  if (!bload)
    {
      delete m_pdoc;
      m_pdoc = NULL;
      return false;
    }
  return true;
}

bool Cxml_cfg_file_decoder::close()
{
  if (m_pdoc != NULL)
    {
      delete m_pdoc;
      m_pdoc = NULL;
    }
  return true;
}

bool Cxml_cfg_file_decoder::get_int_attribute(TiXmlNode* pnode, string name, int *pvalue)
{
  TiXmlAttribute *pAttrib;

  pAttrib = pnode->ToElement()->FirstAttribute();
  while (pAttrib != NULL)
    {
      if (string(pAttrib->Name()) == name)
	{
	  *pvalue = pAttrib->IntValue();
          return true;
	}
      pAttrib = pAttrib->Next();
    }
  return false;
}

bool Cxml_cfg_file_decoder::get_string_attribute(TiXmlNode* pnode, string name, string *pvalue)
{
  TiXmlAttribute *pAttrib;

  pAttrib = pnode->ToElement()->FirstAttribute();
  while (pAttrib != NULL)
    {
      if (string(pAttrib->Name()) == name)
	{
	  *pvalue = pAttrib->ValueStr();
          return true;
	}
      pAttrib = pAttrib->Next();
    }
  return false;
}

bool Cxml_cfg_file_decoder::read_window_position(int *x, int *y)
{
  string             ValueStr;
  TiXmlNode*         pnode;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("window_pos"))
	    {
	      if (get_int_attribute(pnode, string("x"), x))
		if (get_int_attribute(pnode, string("y"), y))
		  return true;
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
  return false;
}

bool Cxml_cfg_file_decoder::read_window_size(int *w, int *h)
{
  string             ValueStr;
  TiXmlNode*         pnode;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("window_size"))
	    {
	      if (get_int_attribute(pnode, string("w"), w))
		if (get_int_attribute(pnode, string("h"), h))
		  return true;
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
  return false;
}

void Cxml_cfg_file_decoder::read_freq_view_mode(bool &bpiano)
{
  string             ValueStr;
  TiXmlNode*         pnode;
  int                v;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("frequency_view"))
	    {
	      if (get_int_attribute(pnode, string("piano"), &v))
		{
		  bpiano = (v == 1);
		}
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
}

void Cxml_cfg_file_decoder::read_record_params(bool *pbrecord_at_start, bool *pbnotappend_to_saved)
{
  string             ValueStr;
  TiXmlNode*         pnode;
  int                v;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("Record_config"))
	    {
	      if (get_int_attribute(pnode, string("record_at_start"), &v))
		{
		  *pbrecord_at_start = (v == 1);
		}
	      if (get_int_attribute(pnode, string("do_not_append_on_opened"), &v))
		{
		  *pbnotappend_to_saved = (v == 1);
		}
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
}

void Cxml_cfg_file_decoder::read_sound_io_params(t_channel_select_strings *chs)
{
  string             ValueStr;
  TiXmlNode*         pnode;
  string             v;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("Sound_io_config"))
	    {
	      if (get_string_attribute(pnode, string("audio_api"), &v))
		{
		  chs->api_name = v;
		}
	      if (get_string_attribute(pnode, string("input_device"), &v))
		{
		  chs->in_device_name = v;
		}
	      if (get_string_attribute(pnode, string("output_device"), &v))
		{
		  chs->out_device_name = v;
		}
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
}

bool Cxml_cfg_file_decoder::read_music_path(std::string *pmusic_path)
{
  string             ValueStr;
  TiXmlNode*         pnode;
  string             v;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("MusicDirectory"))
	    {
	      if (get_string_attribute(pnode, string("music_dir"), &v))
		{
		  *pmusic_path = v;
		  return true;
		}
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
  return false;
}

void Cxml_cfg_file_decoder::read_edit_params(bool *pautobeam, bool *pchordfuse)
{
  string             ValueStr;
  TiXmlNode*         pnode;
  int                v;

  pnode = m_pdoc->RootElement();
  //m_pdoc->Print(); 
  while (pnode != NULL)
    {
      if (pnode->Type() == TiXmlNode::TINYXML_ELEMENT)
	{
	  ValueStr = pnode->ValueStr();
	  if (ValueStr == string("EditParams"))
	    {
	      if (get_int_attribute(pnode, string("autobeam"), &v))
		{
		  *pautobeam = (v == 1);
		}
	      if (get_int_attribute(pnode, string("chordfuse"), &v))
		{
		  *pchordfuse = (v == 1);
		}
	    }
	}
      pnode = pnode->NextSiblingElement();
    }
}

bool Cxml_cfg_file_decoder::open_for_writing()
{
  TiXmlDeclaration *pdecl;

  if (m_pdoc != NULL)
    delete m_pdoc;
  m_pdoc = new TiXmlDocument();
  pdecl = new TiXmlDeclaration("1.0", "", "");
  m_pdoc->LinkEndChild(pdecl);
  return true;
}

bool Cxml_cfg_file_decoder::write(std::string file_name)
{
  bool  bret;

  //if (remove(file_name.c_str()) != 0)
  //  perror("Error deleting file");
  bret = m_pdoc->SaveFile(file_name);
  if (!bret)
    printf("Could not save the configuration faile in %s\n", file_name.c_str());
  return bret;
}

void Cxml_cfg_file_decoder::write_window_position(int x, int y)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("window_pos");
   pnew_element->SetAttribute("x", x);
   pnew_element->SetAttribute("y", y);
   m_pdoc->LinkEndChild(pnew_element);
}

void Cxml_cfg_file_decoder::write_window_size(int w, int h)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("window_size");
   pnew_element->SetAttribute("w", w);
   pnew_element->SetAttribute("h", h);
   m_pdoc->LinkEndChild(pnew_element);  
}

void Cxml_cfg_file_decoder::write_freq_view_mode(bool piano)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("frequency_view");
   pnew_element->SetAttribute("piano", (piano? 1 : 0));
   m_pdoc->LinkEndChild(pnew_element);
}

void Cxml_cfg_file_decoder::write_record_params(bool brecord_at_start, bool bnotappend_to_saved)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("Record_config");
   pnew_element->SetAttribute("record_at_start", (brecord_at_start? 1 : 0));
   pnew_element->SetAttribute("do_not_append_on_opened", (bnotappend_to_saved? 1 : 0));
   m_pdoc->LinkEndChild(pnew_element);
}

void Cxml_cfg_file_decoder::write_sound_io_params(t_channel_select_strings *chs)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("Sound_io_config");
   pnew_element->SetAttribute("audio_api", chs->api_name);
   pnew_element->SetAttribute("input_device", chs->in_device_name);
   pnew_element->SetAttribute("output_device", chs->out_device_name);
   m_pdoc->LinkEndChild(pnew_element);
}

void Cxml_cfg_file_decoder::write_music_path(std::string music_path)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("MusicDirectory");
   pnew_element->SetAttribute("music_dir", music_path);
   m_pdoc->LinkEndChild(pnew_element);
}

void Cxml_cfg_file_decoder::write_edit_params(bool bautobeam, bool bchordfuse)
{
   TiXmlElement *pnew_element;

   pnew_element = new TiXmlElement("EditParams");
   pnew_element->SetAttribute("autobeam", (bautobeam? 1 : 0));
   pnew_element->SetAttribute("chordfuse", (bchordfuse? 1 : 0));
   m_pdoc->LinkEndChild(pnew_element);
}
