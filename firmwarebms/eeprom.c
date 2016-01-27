#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>

#include "env.h"
#include "main.h"
#include "AD7280A.h"
#include "eeprom.h"

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

void set_serial_number_EEPROM(char *str)
{
  int  i;
  char end;

  for (i = 0, end = 0; i < 9; i++)
    {
      end = end || (str[i] == 0);
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
  while (i < 32 && str[i] != 0 && str[i] != '"')
    g_edat.client[i] = str[i];
  write_cfg_to_EEPROM(); // Only writes what's different
}

void inc_charge_cylces_EEPROM(void)
{
  g_edat.charge_cycles++;
  write_cfg_to_EEPROM();
}

void update_charge_time_minutes_EEPROM(unsigned long additional_charge_time_minutes)
{
  g_edat.charge_time_minutes += additional_charge_time_minutes;
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

//---------------------------------------
// Read all, Write all
//---------------------------------------

void read_cfg_from_EEPROM(void)
{
  unsigned char c;
  
  // Read the first byte,
  // if it is 0xFF then the EEPROM is unprogramed: set to default values and return
  // else read the data
  c = eeprom_read_byte((uint8_t*)0x00);
  if (c == 0xFF)
    {
      // Default values to make the buzer scream
      memset(&g_edat, 0, sizeof(g_edat));
      return ;
    }
  // Read a block @ 0x01
  eeprom_read_block(&g_edat, (uint8_t*)0x01, sizeof(g_edat));
}

void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements)
{
  unsigned char c;
  int           offset;

  // Read the first byte,
  // if it is 0xFF then the EEPROM is unprogramed: set to default values and return
  // else read the data
  c = eeprom_read_byte((uint8_t*)0x00);
  if (c == 0xFF)
    {
      return ;
    }
  offset = 0x01 + sizeof(g_edat) + 4;
  eeprom_read_block(pbats, (uint8_t*)offset, elements * sizeof(t_eeprom_battery));  
}

void write_cfg_to_EEPROM(void)
{
  int           i;
  unsigned char c;
  unsigned char *ptr;
  
  ptr = (unsigned char*)&g_edat;
#if 1
  for (i = 0; i < sizeof(g_edat); i++)
    {
      c = eeprom_read_byte((uint8_t*)0x01 + i);
      if (c != ptr[i])
	eeprom_write_byte((uint8_t*)0x01 + i, ptr[i]);
    }
#else
  eeprom_write_block(&g_edat, (uint8_t*)0x01, sizeof(g_edat));
#endif
  eeprom_write_byte((uint8_t*)0x00, 0x00); // indicates the first write
}

void write_bat_values_to_EEPROM(t_eeprom_battery *pbats, int elements)
{
  int           i, j;
  int           offset;
  unsigned char c;
  unsigned char *ptr;
  
  //#if ((sizeof (g_edat) + 5 + (sizeof(t_eeprom_battery) * MAXBATTERY)) > 510)
  // Not enough EEPROM memory
  //#endif
#if 1
  for (i = 0; i < elements; i++)
    {
      // Pass the g_edat structure and add 4 bytes to be sure.
      offset = 0x01 + sizeof(g_edat) + 4 + i * sizeof(t_eeprom_battery);
      //
      for (j = 0; j < sizeof(t_eeprom_battery); j++)
	{
	  ptr = (unsigned char*)&pbats[i];
	  c = eeprom_read_byte((uint8_t*)offset + j);
	  if (c != ptr[j])
	    eeprom_write_byte((uint8_t*)offset + j, ptr[j]);
	}
    }
#else
  eeprom_write_block(&pbats, (uint8_t*)0x01 + sizeof(g_edat) + 4, elements * sizeof(t_eeprom_battery));
#endif
}

// Adds an error code to a circular memory at the end of the EEPROM.
// The first byte is the index of the value that will be written or rewritten.
void add_error_log_EEPROM(char code)
{
  char next_index;

  next_index = eeprom_read_byte((uint8_t*)ERROR_LOG_START);
  next_index = next_index < 0? 0 : next_index;
  next_index = next_index >= ERROR_LOG_SIZE ? 0 : next_index;
  eeprom_write_byte((uint8_t*)(ERROR_LOG_START + 1 + next_index), code);
  next_index++;
  next_index = next_index >= ERROR_LOG_SIZE ? 0 : next_index;
  eeprom_write_byte((uint8_t*)(ERROR_LOG_START), next_index);
}
