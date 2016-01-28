
#include <unistd.h>
#include <iterator>
#include <list>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../env.h"
#include "../main.h"
#include "../AD7280A.h"
#include "../inout.h"
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
//
#include "gfxareas.h"
#include "keypress.h"
#include "app.h"

extern Cappdata *g_papp;

void setled_balancing(char state)
{
  g_papp->set_balancing_led(state);
}

void setled_error(char state)
{
  g_papp->set_error_led(state);
}

void set_buzer(char buzer)
{
  g_papp->set_buzer(buzer != 0);
}

void set_main_relay(char cutmains)
{
  g_papp->set_cutmains(cutmains != 0);
}

void set_charger_disabled(char charger_off)
{
  Cfake_charger *fch = g_papp->get_fake_charger();

  fch->stop_charger(charger_off);
}

// Charger input, returns 1 if the charger is on 0 if the charger isnot powered.
// Just an IO port.
int get_charger_ON(void)
{
  Cfake_charger *fch = g_papp->get_fake_charger();

  return fch->m_bpluged;
}

// Get the volts on each battery
// Gets the value through SPI and the AD8280A
void get_VBAT(t_ad7280_state *p_ad7280, t_pack_variable_data *pappdata)
{
  int i;

  Cfake_battery *fkbat = g_papp->get_fake_bats();
  Cfake_AD7280A *fkAD7280 = g_papp->get_fake_AD7280();

  for (i = 0; i < CFGBATNUMBER; i++)
    {
      pappdata->vbat[i] = fkbat[i].get_mV();
    }
  for (i = 0; i < CFGAD728AMODULES; i++)
    {
      pappdata->temperature[i] = fkAD7280->get_temp();
    }
}

// Get the SHUNT amplified tension and calculate the curent.
void get_IBAT(t_pack_variable_data *pappdata)
{
  pappdata->c_discharge = g_papp->get_run_current();
}

void set_gauge_out(unsigned long SOC, unsigned long fullcharge, char bat_low, char charging)
{
}

