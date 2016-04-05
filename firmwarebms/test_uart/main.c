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
#include "init.h"
#include "uart.h"
#include "serial.h"
#include "AD7280A.h"
#include "inout.h"

t_pack_variable_data g_appdata;
t_eeprom_data        g_edat;
t_eeprom_battery     g_bat[MAXBATTERY];
t_ad7280_state       g_ad7280;
//t_balancing          g_balancing;

extern t_serialport g_serial;


// Called at start, sets the IO port direction, defaults the variables, 
// Initialises the SPI devices and configures the interrupts
void init()
{
  // IO ports
  // Port B:
  DDRB  = 0x00 | (1 << STOP_CHARGER) | (1 << RELON) | (1 << MOSI) | (1 << SCLK);
  PORTB = 0x00 | (0 << STOP_CHARGER) | (0 << RELON) | (0 << MOSI) | (0 << SCLK);
  // Port C:
  DDRC  = 0x00 | (1 << GAUGE) | (1 << CTS) | (1 << CNVSTART);
  PORTC = 0x00 | (1 << GAUGE) | (0 << CTS) | (1 << CNVSTART);
  // Port D:
  DDRD  = 0x00 | (1 << TXD) | (1 << CSBAT) | (1 << PDOWN) | (1 << LED_ERROR) | (1 << LED_BALANCING) | (1 << BUZZER);
  PORTD = 0x00 | (1 << TXD) | (1 << CSBAT) | (0 << PDOWN) | (1 << LED_ERROR) | (1 << LED_BALANCING) | (0 << BUZZER);

  // Wait 50ms
  _delay_ms(50);

  // Default values
  memset(&g_appdata, 0, sizeof(g_appdata));
  // Start point
  g_appdata.app_state = STATE_START;
  //g_appdata.charging_started = 0;

  // Init UART
  init_serial_vars();
  uart_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
  uart_puts("startig the shit\n");

  // Interrupts enables
  sei();
}

int main(void)
{
  int    led_toggle;
  int    e;

  init();
  //
  led_toggle = 0;
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

