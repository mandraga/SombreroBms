/******************************************************************************
 Title:    Sombrero BMS
 Author:   Patrick Areny arenyp@yahoo.fr
 Date:     2016
 Software: AVR-GCC 3.3 
 Hardware: ATMEGA168PA
*******************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"
#include "spi.h"
#include "init.h"
#include "AD7280A.h"
#include "balancing.h"
#include "state_machine.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;
t_balancing          g_balancing;

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

int main(void)
{
  int    i;
  int    led_toggle;

  init();

  // Default values
  g_appdata.uptime_days = 0;
  g_appdata.uptime_minutes = 0;
  g_appdata.tseconds = 0;
  g_appdata.average_discharge = 0;
  g_appdata.c_discharge = 0;
  g_appdata.c_discharge_accumulator = 0;
  g_appdata.state_of_charge = 0; // empty
  for (i = 0; i < MAXBATTERY; i++)
    g_appdata.vbat[i] = 0;
  for (i = 0; i < MAXMODULES; i++)
    g_appdata.temperature[i] = 0;
  // Start point
  g_appdata.app_state = STATE_START;
  g_appdata.charging_started = 0;
  setled_balancing(0);
  led_toggle = 0;
  // Enter here and stay in int for many years
  while (1)
    {
      // Sleep 100ms
      _delay_ms(1000 / SAMPLING_PER_SECOND);
      setled_error(led_toggle);
      led_toggle = led_toggle ^ 1;
      // Change state
      State_machine();
    }
}

