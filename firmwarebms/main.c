
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

#include "uart.h"
#include "spi.h"
#include "init.h"
#include "AD7280A.h"
#include "main.h"
#include "env.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;

void setled_balancing(char state)
{
  if (state)
    SBI(PORTD, LED_BALANCING);
  else
    CBI(PORTD, LED_BALANCING);
}

void setled_error(char state)
{
  if (state)
    SBI(PORTD, LED_ERROR);
  else
    CBI(PORTD, LED_ERROR);
}

void uart1_send_bytes(unsigned char *pdat, int len)
{
  int i;
  
  for (i = 0; i < len; i++)
    {
      uart_putc(pdat[i]);
    }
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
    g_appdata.tempereature[i] = 0;
  // Start point
  g_appdata.app_state = STATE_START;
  g_appdata.charging_started = 0;
  setled_balancing(0);
  led_toggle = 0;
  // Enter here and stay in int for many years
  while (1)
    {
      State_machine(&vbat);
      // Sleep 100ms
      _delay_ms(1000 / SAMPLING_PER_SECOND);
      setled_error(led_toggle);
      led_toggle = led_toggle ^ 1;
    }
}
