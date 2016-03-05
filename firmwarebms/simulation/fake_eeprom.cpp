#include <stdlib.h>
#include <string.h>

#include "../env.h"
#include "../main.h"
#include "../AD7280A.h"
#include "../eeprom.h"

#include "fake_battery.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];

unsigned long string_to_long(char *str)
{
  return strtol(str, NULL, 10L);
}

void set_install_date_EEPROM(char *str)
{
  int   install_date_year;
  int   install_date_month;
  int   install_date_day;
  char *pstr;

  install_date_day = (int)strtol(str, &pstr, 10);
  pstr++;
  str = pstr;
  install_date_month = (int)strtol(str, &pstr, 10);
  pstr++;
  install_date_year = (int)strtol(pstr, NULL, 10);
  //
  g_edat.install_date_day   = install_date_day;
  g_edat.install_date_month = install_date_month;
  g_edat.install_date_year  = install_date_year;
  write_cfg_to_EEPROM();
}

void set_min_Vbat_EEPROM(char *str)
{
  g_edat.bat_minv = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_max_Vbat_EEPROM(char *str)
{
  g_edat.bat_maxv = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_min_temperature_EEPROM(char *str)
{
  g_edat.bat_tmin = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_max_temperature_EEPROM(char *str)
{
  g_edat.bat_tmax = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_full_charge_value_mAH_EEPROM(char *str)
{
  g_edat.full_charge = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_full_charge_value_mV_EEPROM(char *str)
{
  g_edat.full_chargeV = string_to_long(str);
  write_cfg_to_EEPROM();
}

void set_serial_number_EEPROM(char *str)
{
  int  i;
  char end;

  if (*str == '"')
    str++;
  for (i = 0, end = 0; i < 9; i++)
    {
      end = end || (str[i] == 0) || (str[i] != '"');
      g_edat.serial_number[i] = end? 0 : str[i];
    }
  write_cfg_to_EEPROM();
}

void set_client_name_EEPROM(char *str)
{
  int i;

  if (*str == '"')
    str++;
  for (i = 0; i < 33; i++)
    g_edat.client[i] = 0;
  for (i = 0; (i < 32 && str[i] != 0 && str[i] != '"'); i++)
    g_edat.client[i] = str[i];
  write_cfg_to_EEPROM(); // Only writes what's different
}

void inc_charge_cylces_EEPROM(void)
{
  g_edat.charge_cycles++;
  write_cfg_to_EEPROM();
}

void update_temperature_extremes_EEPROM(int *ptemperatures)
{
  int  tavg, i;

  for (i = 0, tavg = 0; i < CFGAD728AMODULES; i++)
    {
      tavg += ptemperatures[i];
    }
  tavg = tavg / CFGAD728AMODULES;
  g_edat.min_temperature = g_edat.min_temperature < tavg? g_edat.min_temperature : tavg;
  g_edat.max_temperature = g_edat.max_temperature > tavg? g_edat.max_temperature : tavg;
  write_cfg_to_EEPROM();
}

//---------------------------------------
// Battery data
//---------------------------------------
void update_battery_low_events_EEPROM(void)
{
  int i, chg;
  
  for (i = 0, chg = 0; i < g_edat.bat_elements; i++)
    {
      if (g_appdata.vbat[i] < g_edat.bat_minv)
	{
	  g_bat[i].lowVevents++;
	  chg = 1;
	}
    }
  if (chg)
    write_bat_values_to_EEPROM(g_bat, g_edat.bat_elements);
}

void update_battery_low_values_EEPROM(void)
{
  int i, chg;

  for (i = 0, chg = 0; i < g_edat.bat_elements; i++)
    {
      if (g_bat[i].lowestV > g_appdata.vbat[i])
	{
	  g_bat[i].lowestV = g_appdata.vbat[i];
	  chg = 1;
	}
    }
  if (chg)
    write_bat_values_to_EEPROM(g_bat, g_edat.bat_elements);
}

void update_battery_charge_values_EEPROM(void)
{
  int i, chg;
  
  for (i = 0, chg = 0; i < g_edat.bat_elements; i++)
    {
      // Test if charged
      if (g_appdata.vbat[i] > g_edat.bat_maxv + 10L
	  /*fixme a state must be used*/) // 10mV threshold
	{
	  // Charging time for the battery
	  g_bat[i].cap = g_appdata.charge_time_count * 60L + g_appdata.charge_time_count_tenth / 10L;
	  chg = 1;
	}
    }
  if (chg)
    write_bat_values_to_EEPROM(g_bat, g_edat.bat_elements);
}

// Total charging time since the install of the pack
void update_charge_time_minutes_EEPROM(unsigned long additional_charge_time_minutes)
{
  g_edat.charge_time_minutes += additional_charge_time_minutes;
  write_cfg_to_EEPROM();
}

//---------------------------------------
// Read all, Write all
//---------------------------------------

void read_cfg_from_EEPROM(void)
{
  unsigned int i;

  // Default values to make the buzer scream
  g_edat.install_date_year  = 2016;
  g_edat.install_date_month = 1;
  g_edat.install_date_day   = 1;
  g_edat.bat_maxv = BAT_MAXV  - 300;   // mili volts
  g_edat.bat_minv = BAT_MINMV + 200;
  g_edat.bat_tmax =  50;               // temperature
  g_edat.bat_tmin = -20;
  g_edat.bat_elements = CFGBATNUMBER;
  g_edat.full_charge  = DEFAULT_CHARGE; // AH
  g_edat.max_current  = DEFAULT_MAX_CURRENT_A;
  g_edat.serial_number[0] = '1';
  for (i = 1; i < sizeof(g_edat.serial_number) / sizeof(char); i++)
    g_edat.serial_number[i] = 0;       //  8 bytes             plus 0 end of string
  g_edat.client[0] = 0;                // 32 bytes Client name plus 0 end of string
  // These are often programmed
  g_edat.charge_cycles = 0;
  g_edat.charge_time_minutes = 0;      // Total charging time
  g_edat.min_temperature = 25;         // Extreme temperatures
  g_edat.max_temperature = 25;
}

void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements)
{
  int i;

  for (i = 0; i < elements; i++)
    {
      pbats[i].lowestV = g_edat.bat_maxv;
      pbats[i].lowVevents = 0;
      pbats[i].cap = 40000;
    }
}

void write_cfg_to_EEPROM(void)
{
}

void write_bat_values_to_EEPROM(t_eeprom_battery *pbats, int elements)
{
}

// Adds an error code to a circular memory at the end of the EEPROM.
// The first byte is the index of the value that will be written or rewritten.
void add_error_log_EEPROM(char code)
{
}

