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
#include "inout.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;
t_balancing          g_balancing;

int main(void)
{
  int    led_toggle;

  init();
  //
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

