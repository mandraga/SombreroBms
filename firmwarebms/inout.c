
// Get the volts on each battery
// Gets the value through SPI and the AD8280A
void get_VBAT(t_vbat *pbat, int size)
{

}

// Get the SHUNT amplified tension and calculate the curent.
void get_IBAT(t_ibat *pibat)
{

}

// Charger input, returns 1 if the charger is on 0 if the charger isnot powered.
// Just an IO port.
int get_charger_ON()
{
}

// Closes the relay connected to a power relay to avoir discharge during charging
void set_cut_mains(int cut)
{
}

// Sets an IO port to 0, the duration of the 0 interval gives tha value to be displayed on the custom gauge.
void set_gauge_out(int SOC)
{
}

