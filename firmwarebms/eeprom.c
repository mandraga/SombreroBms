#include <avr/eeprom.h>

#include "eeprom.h"
#include "main.h"
#include "env.h"

void set_install_date_EEPROM(char *str)
{
  char          install_date_year;
  char          install_date_month;
  char          install_date_day;

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
  while (i < 32 && str[i] != 0)
    g_edat.client[i] = str[i];
  write_cfg_to_EEPROM(); // Only writes what's different
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

