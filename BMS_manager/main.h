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

void close_function(void *pdata);
void set_button(Fl_Button *pb, void* puser_data);
void cancel_callback(Fl_Widget *pwi, void *pdata);

typedef struct                  s_app_data
{
  CTCP_Client                  *ptcpclient;
  std::string                   path;
  int                           xpos;
  int                           ypos;
  Fl_Window                    *pwindow;
  Fl_Check_Button              *pcheckbox_record;
  Fl_Check_Button              *pcheckbox_do_not;
  Fl_Button                    *but_cancel;
  Fl_Button                    *but_ok;
  Fl_Choice                    *audio_api_choice;
  Fl_Choice                    *input_device_choice;
  Fl_Choice                    *output_device_choice;
  pthread_t                     threadclient;
  pthread_mutex_t               cond_devlistmutex;
  pthread_cond_t                cond_devlist;
  bool                          bconnected;
  bool                          bstartrecord;
  bool                          bnotappendtoopen;
  int                           portaudio_devices;
  int                           current_device;
  std::list<t_portaudio_api>    pa_dev_list;
  t_channel_select_strings      pa_chan_sel;
}                               t_app_data;

