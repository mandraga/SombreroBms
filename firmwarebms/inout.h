
#define GAUGE_OUT_STATE_UPDATE   0
#define GAUGE_OUT_STATE_TRANSMIT 1

void set_buzer(char buzer);
void set_main_relay(char cutmains);
void set_charger_disabled(char charger_off);

void get_VBAT(t_ad7280_state *p_ad7280, t_pack_variable_data *pappdata);
void get_IBAT(t_pack_variable_data *pappdata);
int  get_charger_ON(void);

void set_gauge_out(unsigned long SOC, unsigned long fullcharge, char bat_low, char charging);
