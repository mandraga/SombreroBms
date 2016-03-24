
#include "env.h"
#include "AD7280A.h"

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

int get_highter_temperature(int *ptemperature, char modules)
{
  int  max;

  max = 10;
  return max;
}

char ad7280_get_balance(t_ad7280_state *st, char channel)
{
  return 0;
}
