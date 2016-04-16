/******************************************************************************
 Title:    Reads the AD7280A registers and prints them on the serial port
 Author:   Patrick Areny arenyp@yahoo.fr
 Date:     2016
 Software: AVR-GCC 3.3 
 Hardware: ATMEGA168PA

 Description: Reads the Vbat registers and prints them (raw value)
              Reads the Temperature registers and prints them (raw value)
*******************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"
#include "init.h"
#include "uart.h"
#include "serial.h"
#include "thermistor.h"

#include "AD7280A.h"
#include "inout.h"

extern t_serialport g_serial;

void print_AD7280A_ADC_self_test(t_ad7280_state *st)
{
  int           i, ret;
  unsigned int  varray[MAX_CHANNELS];
  int           module_cnt = 2;

  // Should print the reference 1,2mv od 1200mV
  uart_puts("Self test\n");
  _delay_ms(500);
  ret = ad7280_read_ADC_selftest(st, varray, module_cnt);
  if (!ret)
    {
      for (i = 0; i < module_cnt; i++)
	{
	  sprintf(g_serial.outbuffer, "chan %d %d ", i, varray[i]);
	  uart_puts(g_serial.outbuffer);
	  _delay_ms(400);
	  sprintf(g_serial.outbuffer, "%ldmv\n", AD7280Adac_AUX_2_mv(varray[i]));
	  uart_puts(g_serial.outbuffer);
	  _delay_ms(400);
	}
    }
  uart_puts("\n");
  _delay_ms(400);
}

#define PRINTALL

// 6 Cells + 3 temperatures (AUX)
#define CHANNELS 9

void print_AD7280A_VBAT_n_temp(t_ad7280_state *st)
{
  int           i, mod, index, ret;
  unsigned int  varray[MAX_CHANNELS];
  unsigned long temp_mv;
  unsigned long Cell_mv;

  uart_puts("Read all channels:\n");
  _delay_ms(400);
  ret = ad7280_read_all_channels(st, varray, CFGAD728AMODULES * 9);
  _delay_ms(1000);
  if (!ret)
    {
      for (mod = 0; mod < CFGAD728AMODULES; mod++)
	{
	  for (i = 0; i < CHANNELS; i++)
	    {
	      index = mod * 9 + i;
	      if (i < 6)
		sprintf(g_serial.outbuffer, "cell %d %d", i, varray[index]);
	      else
		sprintf(g_serial.outbuffer, "temp %d %d", i, varray[index]);
	      uart_puts(g_serial.outbuffer);
	      _delay_ms(800);
	      //
	      if (i < 6)
		{
		  Cell_mv = AD7280Adac_CELL_2_mv(varray[index]);
		  sprintf(g_serial.outbuffer, " %ldmv\n",  Cell_mv);
		}
	      else
		{
		  temp_mv = AD7280Adac_AUX_2_mv(varray[index]);
		  sprintf(g_serial.outbuffer, " %ldmv %ddC\n", temp_mv, mv_to_temperature(temp_mv));
		}
	      uart_puts(g_serial.outbuffer);
	      _delay_ms(800);
	    }
	  uart_puts("\n");
	  _delay_ms(800);
	}
    }
  else
    {
      uart_puts("reading the AD7280A channels failed\n");
      _delay_ms(200);
    }
}

