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

//---------------------------------------
// Read all, Write all
//---------------------------------------

void read_cfg_from_EEPROM(void)
{
  int           i;
  unsigned char c;

  // Read the first byte,
  // if it is 0xFF then the EEPROM is unprogramed: set to default values and return
  // else read the data
  c = eeprom_read_byte((uint8_t*)0x00);
  if (c == 0xFF)
    {
      // Default values to make the buzer scream
      g_edat.install_date_year = 2016;
      g_edat.install_date_month = 1;
      g_edat.install_date_day = 1;
      g_edat.bat_maxv = 3600;               // mili volts
      g_edat.bat_minv = 3000;
      g_edat.bat_tmax =  50;                // temperature
      g_edat.bat_tmin = -20;
      g_edat.bat_elements = CFGBATNUMBER;
      g_edat.full_charge  = DEFAULT_CHARGE; // AH
      g_edat.max_current  = DEFAULT_MAX_CURRENT_A; // A
      g_edat.serial_number[0] = '1';
      for (i = 1; i < sizeof(g_edat.serial_number) / sizeof(char); i++)
	g_edat.serial_number[i] = 0;       //  8 bytes             plus 0 end of string
      g_edat.client[0] = 0;                // 32 bytes Client name plus 0 end of string
      // These are often programmed
      g_edat.charge_cycles = 0;
      g_edat.charge_time_minutes = 0;      // Total charging time
      g_edat.min_temperature = 25;         // Extreme temperatures
      g_edat.max_temperature = 25;
      return ;
    }
  // Read a block @ 0x01
  eeprom_read_block(&g_edat, (uint8_t*)0x01, sizeof(g_edat));
}

void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements)
{
  unsigned char c;
  int           offset;
  int           i;

  // Read the first byte,
  // if it is 0xFF then the EEPROM is unprogramed: set to default values and return
  // else read the data
  c = eeprom_read_byte((uint8_t*)0x00);
  if (c == 0xFF)
    {
      for (i = 0; i < elements; i++)
	{
	  pbats[i].lowestV = g_edat.bat_maxv;
	  pbats[i].lowVevents = 0;
	  pbats[i].cap = 40000;
	}
      return ;
    }
  offset = 0x01 + sizeof(g_edat) + 4;
  eeprom_read_block(pbats, (uint8_t*)offset, elements * sizeof(t_eeprom_battery));  
}

void write_cfg_to_EEPROM(void)
{
#if 0
  int           i;
  unsigned char c;
  unsigned char *ptr;
  
  ptr = (unsigned char*)&g_edat;
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
  //#if ((sizeof (g_edat) + 5 + (sizeof(t_eeprom_battery) * MAXBATTERY)) > 510)
  // Not enough EEPROM memory
  //#endif
#if 0
  int           i, j;
  int           offset;
  unsigned char c;
  unsigned char *ptr;

sxsx
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
  /*
  char next_index;

  next_index = eeprom_read_byte((uint8_t*)ERROR_LOG_START);
  next_index = next_index < 0? 0 : next_index;
  next_index = next_index >= ERROR_LOG_SIZE ? 0 : next_index;
  eeprom_write_byte((uint8_t*)(ERROR_LOG_START + 1 + next_index), code);
  next_index++;
  next_index = next_index >= ERROR_LOG_SIZE ? 0 : next_index;
  eeprom_write_byte((uint8_t*)(ERROR_LOG_START), next_index);
  */
}

