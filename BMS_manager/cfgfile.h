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

#define CONFIG_FILE_NAME "cfg.xml"

class Cxml_cfg_file_decoder
{
 public:
  Cxml_cfg_file_decoder();
  ~Cxml_cfg_file_decoder();

 public:
  // Path
  std::string get_user_config_path();
  std::string get_user_documents_path();

  // Reading
  bool open_for_reading(std::string file_name);
  bool read_window_position(int *x, int *y);
  bool read_window_size(int *x, int *y);
  void read_freq_view_mode(bool &bpiano);
  void read_record_params(bool *pbrecord_at_start, bool *pbnotappend_to_saved);
  void read_sound_io_params(t_channel_select_strings *chs);
  bool read_music_path(std::string *pmusic_path);
  void read_edit_params(bool *pautobeam, bool *pchordfuse);
  bool close();

  // Writing
  bool open_for_writing();
  bool write(std::string file_name);

  void write_window_position(int x, int y);
  void write_window_size(int x, int y);
  void write_freq_view_mode(bool piano);
  void write_record_params(bool brecord_at_start, bool bnotappend_to_saved);
  void write_sound_io_params(t_channel_select_strings *chs);
  void write_music_path(std::string music_path);
  void write_edit_params(bool autobeam, bool chordfuse);

 private:
  bool get_int_attribute(TiXmlNode* pnode, std::string name, int *pvalue);
  bool get_string_attribute(TiXmlNode* pnode, std::string name, std::string *pvalue);
  std::string get_user_home();

 private:
  TiXmlDocument *m_pdoc;
};


