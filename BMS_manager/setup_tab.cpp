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
#include <string>
#include <list>

#include <time.h>
#include <tinyxml.h>
#include <pthread.h>

#include <event2/dns.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>

#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>

#include "audioapi.h"
#include "score.h"
#include "messages.h"
#include "message_decoder.h"
#include "tcpclient.h"
#include "main.h"

using namespace std;

void cancel_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;

  close_function(papp_data);
}

using namespace scmsg;

void ok_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;
  const int       cmsgsize = 4096;
  char            msg[cmsgsize];
  int             size;
  Cmessage_coding coder;
  t_appconfig     cfg;
  string          command;
  
  
  cfg.recordAtStart = papp_data->bstartrecord;
  cfg.doNotChangeOpenedFiles = papp_data->bnotappendtoopen;
  command = coder.create_config_message(&cfg);
  if (coder.build_wire_message(msg, cmsgsize, &size, command))
    {
      papp_data->ptcpclient->Send(msg, size);
    }
  close_function(pdata);
}

void check_box_recordnow_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;

  if (papp_data->pcheckbox_record->value())
    papp_data->bstartrecord = true;
  else
    papp_data->bstartrecord = false;
}

void check_box_recordover_callback(Fl_Widget *pwi, void *pdata)
{
  t_app_data *papp_data = (t_app_data*)pdata;

  if (papp_data->pcheckbox_do_not->value())
    papp_data->bnotappendtoopen = true;
  else
    papp_data->bnotappendtoopen = false;
}

void create_config_tab(int x, int y, int w, int h, void *pdata)
{
  Fl_Group   *group1;
  t_app_data *papp_data = (t_app_data*)pdata;
  int         border;
  int         buttonw;
  int         buttonh;

  group1 = new Fl_Group(x, y, w, h, "conf");
  {
    border = 10;
    papp_data->pcheckbox_record = new Fl_Check_Button(x + border, y + border, w - 3 * border, 3 * border, "Record right when starting");
    papp_data->pcheckbox_record->value(papp_data->bstartrecord?  1 : 0);
    papp_data->pcheckbox_record->user_data(papp_data);
    papp_data->pcheckbox_record->callback(check_box_recordnow_callback);
    //
    papp_data->pcheckbox_do_not = new Fl_Check_Button(x + border, y + 5 * border, w - 3 * border, 3 * border, "Do not record over opened projects");
    papp_data->pcheckbox_do_not->value(papp_data->bnotappendtoopen? 1 : 0);
    papp_data->pcheckbox_do_not->user_data(papp_data);
    papp_data->pcheckbox_do_not->callback(check_box_recordover_callback);
    //
    buttonw = 90;
    buttonh = 50;
    papp_data->but_ok = new Fl_Button(x + w - buttonw - border, y + h - buttonh - 2 * border, buttonw, buttonh, "Ok");
    set_button(papp_data->but_ok, papp_data);
    papp_data->but_ok->callback(ok_callback);
    //
    papp_data->but_cancel = new Fl_Button(x + w - 2 * (buttonw + border), y + h - buttonh - 2 * border, buttonw, buttonh, "Cancel");
    set_button(papp_data->but_cancel, papp_data);
    papp_data->but_cancel->callback(cancel_callback);
  }
  group1->end();
}

