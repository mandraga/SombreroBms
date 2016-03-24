
#include <avr/io.h>

#include "env.h"
#include "main.h"
#include "AD7280A.h"
#include "adc.h"
#include "inout.h"

void setled_balancing(char state)
{
  if (state)
    CBI(PORTD, LED_BALANCING); // on
  else
    SBI(PORTD, LED_BALANCING);
}

void setled_error(char state)
{
  if (state)
    CBI(PORTD, LED_ERROR);
  else
    SBI(PORTD, LED_ERROR);
}

void set_buzer(char buzer)
{
  if (buzer)
    SBI(PORTB, BUZZER);
  else
    CBI(PORTB, BUZZER);
}

void set_main_relay(char cutmains)
{
  if (cutmains)
    SBI(PORTC, RELON);
  else
    CBI(PORTC, RELON);
}

void set_charger_disabled(char charger_off)
{
  if (charger_off)
    SBI(PORTD, STOP_CHARGER);
  else
    CBI(PORTD, STOP_CHARGER);  
}

// Charger input, returns 1 if the charger is on 0 if the charger isnot powered.
// Just an IO port.
int get_charger_ON(void)
{
  char result;

  result = PORTD;
  result = (result & (1 << CHARGER_ON)) != 0? 1 : 0;
  return result;
}

// Get the volts on each battery
// Gets the value through SPI and the AD8280A
void get_VBAT(t_ad7280_state *p_ad7280, t_pack_variable_data *pappdata)
{
  ad7280_get_VBAT(p_ad7280, pappdata->vbat, pappdata->temperature);
}

// Get the SHUNT amplified tension and calculate the curent.
void get_IBAT(t_pack_variable_data *pappdata)
{
  unsigned long conversion;
  unsigned long volts;
  unsigned long shunt_uV;

  // Get the value from the ADC channel 7
  conversion = get_adc7();
  // Find the conversion value in millivolts
  // 10bits = DACREF (like 5V or 1.1V)
  // value in mV = (REF / 1024) * 1000 * conversion
  volts = (DACREFV * 1000L * conversion) >> 10; // >> 10 <=> / 1024
  // We have a shunt of some milli homs, the measured volts of the battery pack and
  // the ADC value to be coverted in volts.
  // the current is shunt_mV / SHUNT_VAL
  //
  // Convert the measured value to the real µV value on the shunt
  shunt_uV = (volts * 1000L) / INA197GAIN;
  // Convert it to milli amps (U = R x I for the newbies reading this anyways).
  // (1000 * µV / µHom) for milliAmps
  pappdata->c_discharge = (1000L * shunt_uV / SHUNT_VAL); // mA for the interval SAMPLING_INTERVAL_MS;
}

// Sets an IO port to 0, the duration of the LOW interval gives tha value to be displayed on the custom gauge.
// States transmited:
// 1 to 11: bar graph values from blinking 1 to 10
// 12:      security state, blink fast
// 13:      charging the bars fill from bottom to top
// 14:      failure the 10 bars blink simultaneouslty
//
// It is transmited with the duration of the pin low state.
// 1 cycle hight, then 1 to 12 cycles low.
// Because it is called continuously it must change states depending on the input
// only once per 14 calls.
// Note that the output goes to an optocoupler so 1 is 0 on the VBAT outpout.
void set_gauge_out(unsigned long SOC, unsigned long fullcharge, char bat_low, char charging, char failure)
{
  static char   gauge_out_state = GAUGE_OUT_STATE_UPDATE;
  static char   gauge_out_value;
  unsigned long scale;

  if (gauge_out_state == GAUGE_OUT_STATE_UPDATE)
    {
      scale = (10L * SOC) / fullcharge;
      gauge_out_value = scale + 1;
      if (bat_low)
	gauge_out_value = 12;
      if (charging)
	gauge_out_value = 13;
      if (failure)
	gauge_out_value = 14;
      gauge_out_state = GAUGE_OUT_STATE_TRANSMIT;
    }
  if (gauge_out_state == GAUGE_OUT_STATE_TRANSMIT)
    {
      if (gauge_out_value == 1 || gauge_out_value <= 0)
	CBI(PORTD, GAUGE);
      else
	CBI(PORTD, GAUGE);
      gauge_out_value--;
      if (gauge_out_value <= 0)
	gauge_out_state = GAUGE_OUT_STATE_UPDATE;
    }
}

