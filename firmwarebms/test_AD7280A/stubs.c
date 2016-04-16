
#include "env.h"
#include "main.h"
#include "eeprom.h"

void set_install_date_EEPROM(char *str)
{
}

void set_min_Vbat_EEPROM(char *str)
{
}
 
void set_max_Vbat_EEPROM(char *str)
{
}

void set_min_temperature_EEPROM(char *str)
{
}	  

void set_max_temperature_EEPROM(char *str)
{
}

void set_full_charge_value_mAH_EEPROM(char *str)
{
}

void set_full_charge_value_mV_EEPROM(char *str)
{
}

void set_serial_number_EEPROM(char *str)
{
}

void set_client_name_EEPROM(char *str)
{
}

void read_cfg_from_EEPROM(void)
{
}

void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements)
{
}

int get_highter_temperature(int *ptemperature, char modules)
{
  int  max;

  max = 4;
  return max;
}
