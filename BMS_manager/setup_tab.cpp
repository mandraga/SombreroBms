
#include <string>
#include <list>

#include <time.h>
#include <tinyxml.h>
#include <pthread.h>


#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>

#include "main.h"

using namespace std;


/*
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
}*/

void create_setup_tab(int x, int y, int w, int h, void *pdata)
{
  Fl_Group   *group1;
  t_app_data *papp_data = (t_app_data*)pdata;
  int         border;
  int         buttonw;
  int         buttonh;

  group1 = new Fl_Group(x, y, w, h, "conf");
  {
    border = 10;
/*
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
*/
  }
  group1->end();
}

