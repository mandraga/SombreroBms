/******************************************************************************
 Title:    Sombrero BMS
 Author:   Patrick Areny arenyp@yahoo.fr
 Date:     2016
 Software: AVR-GCC 3.3 
 Hardware: ATMEGA168PA

*******************************************************************************/
//
// Description: Part of this file is taken from the linux driver source code from 
// Analog Devices and changed to fit the atmel AVR.
//
// Original Analog Deviecs code:
//
// MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
// MODULE_DESCRIPTION("Analog Devices AD7280A");
// MODULE_LICENSE("GPL v2");
//
// AD7280A Lithium Ion Battery Monitoring System
//
// Copyright 2011 Analog Devices Inc.
//
// Licensed under the GPL-2.
//

#include <unistd.h>
#include <iterator>
#include <list>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../env.h"
#include "../main.h"
#include "../init.h"
#include "../eeprom.h"
#include "../thermistor.h"
#include "../AD7280A.h"

#include "gfxareas.h"
#include "keypress.h"
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
//
#include "app.h"


extern Cappdata  *g_papp;

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];
extern t_ad7280_state       g_ad7280;

int init_AD7820A(t_ad7280_state *st)
{
  char thermistor_term_en = 1;
  char acquisition_time = AD7280A_ACQ_TIME_1600ns; // Do not change, 1600ns required for the thermistor reading.
  char conversion_averaging = AD7280A_CONV_AVG_DIS;

  //
  // Setup the channels per module count
  //
  st->chan_cnt[0] = CFGAD728AMODULE_0_CHAN;
  st->chan_cnt[1] = CFGAD728AMODULE_1_CHAN;
  st->chan_cnt[2] = CFGAD728AMODULE_2_CHAN;
  // Cell balance masks to all MosFets disabled.
  st->cb_mask[0] = st->cb_mask[1] = st->cb_mask[2] = 0;
  //
  // Control register values:
  // - Thermistor res enabled
  // - 6 cell + 3 AUX
  st->ctrl_hb = AD7280A_CTRL_LB_ACQ_TIME(acquisition_time & 0x3) | AD7280A_CTRL_HB_CONV_RES_READ_6CELL_AUX1_3_4;
  st->ctrl_lb = AD7280A_CTRL_HB_CONV_AVG(conversion_averaging & 0x3) | (thermistor_term_en != 0 ? AD7280A_CTRL_LB_THERMISTOR_EN : 0);
  //
  st->slave_num = CFGAD728AMODULES;
  st->cell_threshhigh = 0xFF;
  st->aux_threshhigh  = 0xFF;

  Cfake_AD7280A *pfk_AD7280A = g_papp->get_fake_AD7280();
  for (int i = 0; i < CFGAD728AMODULES; i++)
    pfk_AD7280A->set_balancing(i, 0);

  // Ready for use
  return 0;
}

int ad7280_get_VBAT(t_ad7280_state *st, unsigned long *pvbat, int *ptemp)
{
  Cfake_battery *pfkbats = g_papp->get_fake_bats();
  Cfake_AD7280A *pfk_AD7280A = g_papp->get_fake_AD7280();
  int            i;

  for (i = 0; i < CFGBATNUMBER; i++)
    {
      pvbat[i] = pfkbats[i].get_mV();
    }
  for (i = 0; i < CFGAD728AMODULES; i++)
    ptemp[CFGAD728AMODULES] = pfk_AD7280A->get_temp() + i;
  return 0;
}

char ad7280_set_balance(t_ad7280_state *st, unsigned long balancing)
{
  int           i;
  Cfake_AD7280A *pfk_AD7280A = g_papp->get_fake_AD7280();

  // Set the values per module and send them
  for (i = 0; i < CFGBATNUMBER; i++)
    {
      pfk_AD7280A->set_balancing(i, (balancing & (1 << i)) != 0);
    }
  return 0;
}

char ad7280_get_balance(t_ad7280_state *st, char element)
{
  Cfake_AD7280A *pfk_AD7280A = g_papp->get_fake_AD7280();

  return pfk_AD7280A->get_balancing(element);
}

