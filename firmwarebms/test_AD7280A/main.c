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
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"
#include "init.h"
#include "uart.h"
#include "serial.h"
#include "AD7280A.h"
#include "inout.h"
#include "test_AD7280A.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;
//t_balancing          g_balancing;

extern t_serialport g_serial;

int main(void)
{
  int    led_toggle;
  int    e;

  init();
  //
  led_toggle = 0;

  
  _delay_ms(1000);
  uart_puts_P(PSTR("Init ok.\n"));
  _delay_ms(1000);

  print_AD7280A_ADC_self_test(&g_ad7280);
  _delay_ms(1000);
  print_AD7280A_VBAT_n_temp(&g_ad7280);
  _delay_ms(2000);

  // Enter here and stay in int for many years
  e = 0;
  while (1)
    {
      // Sleep 1000ms
      _delay_ms(10000 / SAMPLING_PER_SECOND);

      switch (e)
	{
	case 0:
	  strncpy(g_serial.inbuffer, "get_report\n", RCVSTRINGSZ);
	  break;
	case 1:
	  strncpy(g_serial.inbuffer, "ping\n", RCVSTRINGSZ);
	  break;
	case 2:
	  strncpy(g_serial.inbuffer, "get_params\n", RCVSTRINGSZ);
	  break;
	};
      g_serial.inindex = strlen(g_serial.inbuffer);
      g_serial.RXstate = SER_STATE_IDLE;
      g_serial.TXstate = SER_STATE_IDLE;
      process_serial_command();
      e++;
      e = e >= 3? 0 : e;

      //
      led_toggle = led_toggle ^ 1;
      setled_balancing(led_toggle);
    }
}

