
void set_install_date_EEPROM(char *str);
void set_min_Vbat_EEPROM(char *str);	  
void set_max_Vbat_EEPROM(char *str);
void set_min_temperature_EEPROM(char *str);	  
void set_max_temperature_EEPROM(char *str);
void set_full_charge_value_mAH_EEPROM(char *str);	  
void set_serial_number_EEPROM(char *str);
void set_client_name_EEPROM(char *str);

void set_charge_cycles_count_EEPROM(int charge_cycles);
void set_charge_time_minutes_EEPROM(unsigned long charge_time_minutes);

void read_cfg_from_EEPROM(void);

void write_cfg_to_EEPROM(void);
void write_bat_values_to_EEPROM(t_eeprom_battery *pbats, int elements);
