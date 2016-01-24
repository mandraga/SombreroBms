#include <avr/eeprom.h>

#include "eeprom.h"
#include "main.h"
#include "env.h"

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

  pstr = str;
  while (*pstr != '/' && *pstr != '\\' && *pstr != 0)
    pstr++;
  if (*pstr == 0)
    return;
  install_date_day = (int)strtol(str, pstr, 10);
  pstr++;
  str = pstr;
  while (*pstr != '/' && *pstr != '\\' && *pstr != 0)
    pstr++;
  if (*pstr == 0)
    return;
  install_date_month = (int)strtol(str, pstr, 10);
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
  char i;

  for (i = 0; i < 9; i++)
    g_edat.serial_number[i] = 0;
  while (i < 8 && str[i] != 0)
    g_edat.serial_number[i] = str[i];
  write_cfg_to_EEPROM();
}

void set_client_name_EEPROM(char *str)
{
  char i;

  if (*str == '"')
    str++;
  for (i = 0; i < 33; i++)
    g_edat.client[i] = 0;
  while (i < 32 && str[i] != 0 && str[i] != '"')
    g_edat.client[i] = str[i];
  write_cfg_to_EEPROM(); // Only writes what's different
}

void set_charge_cycles_count_EEPROM(int charge_cycles)
{
  g_edat.charge_cycles = charge_cycles;
  write_cfg_to_EEPROM();
}

void set_charge_time_minutes_EEPROM(unsigned long charge_time_minutes)
{
  g_edat.charge_time_minutes = charge_time_minutes;
  write_cfg_to_EEPROM();
}

void read_cfg_from_EEPROM(void)
{
  unsigned char c;
  int           alarm_number;
  
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
  
#if ((sizeof(g_edat) + 5 + sizeof(t_eeprom_battery) * MAXBATTERY) > 510)
  Not enough EEPROM memory
#endif
#if 1
  for (i = 0; i < elements; i++)
    {
      // Pass the g_edat structure and add 4 bytes to be sure.
      offset = 0x01 + sizeof(g_edat) + 4 + i * sizeof(t_eeprom_battery);
      //
      for (j = 0; j < sizeof(t_eeprom_battery); j++)
	{
	  ptr = (unsigned char*)&pbats[i];
	  c = eeprom_read_byte(offset + j);
	  if (c != ptr[j])
	    eeprom_write_byte(offset + j, ptr[j]);
	}
    }
#else
  eeprom_write_block(&pbats, (uint8_t*)0x01 + sizeof(g_edat) + 4, elements * sizeof(t_eeprom_battery));
#endif
}
