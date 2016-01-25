
void set_install_date_EEPROM(char *str);
void set_min_Vbat_EEPROM(char *str);	  
void set_max_Vbat_EEPROM(char *str);
void set_min_temperature_EEPROM(char *str);	  
void set_max_temperature_EEPROM(char *str);
void set_full_charge_value_mAH_EEPROM(char *str);	  
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
