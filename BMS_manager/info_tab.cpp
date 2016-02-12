

#include <FL/Fl_Group.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

#include "env.h"

using namespace std;

void create_information_tab(int x, int y, int w, int h, void *app_data)
{
  t_app_data      *papp_data = (t_app_data*)pdata;
  int              border;
  Fl_Text_Display *ptext;
  Fl_Text_Buffer  *pbuff;
  t_params         BMSparams;
  char             text[4096];
  string           out;

  Fl_Group *group1 = new Fl_Group(x, y, w, h, "info");
  {
    border = TAB_BORDER;
    ptext = new Fl_Text_Display(x + border, y + border, w - 3 * border, h - 3 * border);
    pbuff = new Fl_Text_Buffer();
    ptext->buffer(pbuff);
    // Get the text from the BMS
    papp_data->pBMS->get_params(&BMSparams);
    // Print them
    sprintf(text, "\n\nSetup date: %s\nCharge: %d%c\ntotal capacity: %f\nVbat: %fV\n\n", BMSparams.setupdate, BMSparams.charge_percent, '%', BMSparams.total_capacity, BMSparams.Vbat);
    out =  string(text);
    sprintf(text, "\n\nBat elements: %d\nCharge cycles: %d\nTotal charging time: %dh\nVmin: %fV\nVmax: %fV\n", BMSparams.BatElements, BMSparams.ChargeCycles, BMSparams.ChargeTimeTotal, BMSparams.EltVmin, BMSparams.EltVmax);
    out += string(text);
    sprintf(text, "MaxVbat: %fV\nundervoltage events: %d\nmaxdischarge: %dA\naverage discharge: %dA\nserial number: %s\n", BMSparams.MaxVbat, BMSparams.undervoltageEvts, BMSparams.maxdischarge, BMSparams.avgdischarge, BMSparams.serialnumber);
    out += string(text);
    sprintf(text, "Client name: %s\nuptime: %s\nmintemperature: %d°C\nmaxtemperature: %d°C\ntemperature: %d°C\n", BMSparams.client, BMSparams.uptime, BMSparams.tmin, BMSparams.tmax, BMSparams.temperature);
    out += string(text);
    //
    pbuff->text(out.c_str());
  }
  group1->end();
}

