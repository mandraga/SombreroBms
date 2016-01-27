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
}

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

void set_serial_number_EEPROM(char *str)
{
}

void set_client_name_EEPROM(char *str)
{
}

void inc_charge_cylces_EEPROM(void)
{
}

void update_charge_time_minutes_EEPROM(unsigned long additional_charge_time_minutes)
{
}

void update_temperature_extremes_EEPROM(int *ptemperatures)
{
}

//---------------------------------------
// Battery data
//---------------------------------------
void update_battery_low_events_EEPROM(void)
{
}

void update_battery_low_values_EEPROM(void)
{
}

void update_battery_charge_values_EEPROM(void)
{
}

//---------------------------------------
// Read all, Write all
//---------------------------------------

void read_cfg_from_EEPROM(void)
{
}

void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements)
{
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
