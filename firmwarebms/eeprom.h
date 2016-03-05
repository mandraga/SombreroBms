
// Log the last 64 errors.
#define ERROR_CODE_BALANCING_EMPTY   0
#define ERROR_CODE_BALANCING_FAILED  1
#define ERROR_CODE_VBAT_LOW_CRITICAL 2
#define ERROR_CODE_SHORT_CIRCUIT     3
#define ERROR_CODE_HIGHT_CURRENT     4

// EEPROM size
#ifndef E2SIZE
#define E2SIZE 512
#endif

#define ERROR_LOG_SIZE              16
#define ERROR_LOG_START        (E2SIZE - ERROR_LOG_SIZE - 1)

void add_error_log_EEPROM(char code);

void set_install_date_EEPROM(char *str);
void set_min_Vbat_EEPROM(char *str);	  
void set_max_Vbat_EEPROM(char *str);
void set_min_temperature_EEPROM(char *str);	  
void set_max_temperature_EEPROM(char *str);
void set_full_charge_value_mAH_EEPROM(char *str);
void set_full_charge_value_mV_EEPROM(char *str);
void set_serial_number_EEPROM(char *str);
void set_client_name_EEPROM(char *str);

void inc_charge_cylces_EEPROM(void);
void update_charge_time_minutes_EEPROM(unsigned long additional_charge_time_minutes);
void update_temperature_extremes_EEPROM(int *ptempereatures);

void update_battery_low_events_EEPROM(void);
void update_battery_low_values_EEPROM(void);
void update_battery_charge_values_EEPROM(void);

void read_cfg_from_EEPROM(void);
void read_bat_values_from_EEPROM(t_eeprom_battery *pbats, int elements);

void write_cfg_to_EEPROM(void);
void write_bat_values_to_EEPROM(t_eeprom_battery *pbats, int elements);
