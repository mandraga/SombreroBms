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
#include "uart.h"
#include "serial.h"
#include "AD7280A.h"
#include "inout.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;

// Called at start, sets the IO port direction, defaults the variables, 
// Initialises the SPI devices and configures the interrupts
void init(void)
{
  // IO ports
  // Port B:
  DDRB  = 0x00 |  (1 << STOP_CHARGER) | (1 << RELON) | (1 << MOSI) | (1 << SCLK);
  PORTB = 0x00 |  (1 << STOP_CHARGER) | (1 << RELON) | (0 << MOSI) | (0 << SCLK);
  // Port C:
  DDRC  = 0x00 | (1 << GAUGE) | (1 << CTS) | (1 << CNVSTART);
  PORTC = 0x00 | (1 << GAUGE) | (0 << CTS) | (1 << CNVSTART);
  // Port D:
  DDRD  = 0x00 | (1 << TXD) | (1 << CSBAT) | (1 << PDOWN) | (1 << LED_ERROR) | (1 << LED_BALANCING) | (1 << BUZZER);
  PORTD = 0x00 | (1 << TXD) | (1 << CSBAT) | (1 << PDOWN) | (1 << LED_ERROR) | (1 << LED_BALANCING) | (0 << BUZZER);

  // Wait 50ms
  _delay_ms(50);

  // Default values
  memset(&g_appdata, 0, sizeof(g_appdata));
  // Start point
  g_appdata.app_state = STATE_START;
  //g_appdata.charging_started = 0;
}

int main(void)
{
  int    led_toggle1;
  int    led_toggle2;

  init();
  //
  led_toggle1 = 1;
  led_toggle2 = 0;
  while (1)
    {
      // Sleep 1000ms
      _delay_ms(10000 / SAMPLING_PER_SECOND);
      //
      led_toggle1 = led_toggle1 ^ 1;
      led_toggle2 = led_toggle2 ^ 1;
      setled_balancing(led_toggle1);
      setled_error(led_toggle2);
    }
}

