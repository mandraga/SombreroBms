//
// Description: Part of this file is taken from the linux driver source code from 
// Analog Devices and changed to fit the atmel AVR.
//
// Original Analog Deviecs code:
//
// MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
// MODULE_DESCRIPTION("Analog Devices AD7280A");
// MODULE_LICENSE("GPL v2");
//
// AD7280A Lithium Ion Battery Monitoring System
//
// Copyright 2011 Analog Devices Inc.
//
// Licensed under the GPL-2.
//
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"
#include "spi.h"
#include "init.h"
#include "eeprom.h"
#include "thermistor.h"
#include "AD7280A.h"
//#include "balancing.h"
//#include "state_machine.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];
extern t_ad7280_state       g_ad7280;

#define DEBUG_AD7280

#include <avr/io.h>
#ifdef DEBUG_AD7280
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "serial.h"
extern t_serialport g_serial;
#endif

//#define USE_RAW_WRITES

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

// This is a conversion read
int ad7280_read_channel(t_ad7280_state *st, unsigned devaddr, unsigned addr)
{
/*
  unsigned long tmp;

  ad7280_write(devaddr, AD7280A_READ, 0, addr << 2);
  // turns off the read operation on all parts
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_NO |
	       st->ctrl_hb);
  // turns on the read operation on the addressed part
  ad7280_write(devaddr, AD7280A_CONTROL_HB, 0,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_ALL |
	       AD7280A_CTRL_HB_CONV_START_CS |
	       st->ctrl_hb);
  // Wait for the conversion
  ad7280_delay();
  // Read the channels
  __ad7280_read32(&tmp);
  // Check the CRC
  if (ad7280_check_crc(tmp))
    return 1;
  // Check the returned command addresses
  if (((tmp >> 27) != devaddr) || (((tmp >> 23) & 0xF) != addr))
    return 1;
  // Return the conversion value
  return (tmp >> 11) & 0xFFF;
*/
  return 0;
}

// This is a conversion read
int ad7280_read_ADC_selftest(t_ad7280_state *st, unsigned int *pvalues, int modules)
{
  unsigned long tmp;
  int i;

#ifdef USE_RAW_WRITES
  ad7280_write_raw(0x01B81092L);
  ad7280_write_raw(0x01C2B6E2L);
  ad7280_write_raw(0x038617CAL);
  ad7280_write_raw(0x03A0546AL);
  // CNVST pin to 0
  CBI(PORTC, CNVSTART);
  // Wait for the conversion
  ad7280_delay();
  // CNVST pin to 0
  SBI(PORTC, CNVSTART);
  ad7280_write_raw(0x03A0340AL);
#else
  // Control
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_SELF_TEST);
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_MUST_SET |
	       st->ctrl_lb);
  // Read
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_READ, 1, AD7280A_SELF_TEST << 2);
  // Enable CNVST on falling edge
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CNVST_CONTROL, 1, 0x02);
  // CNVST pin to 0
  CBI(PORTC, CNVSTART);
  // Wait for the conversion
  ad7280_delay();
  // CNVST pin to 0
  SBI(PORTC, CNVSTART);
  // Disable CNVST on falling edge
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CNVST_CONTROL, 1, 0x01);
#endif
  for (i = 0; i < modules; i++)
    {
      // Read the channels
      __ad7280_read32(&tmp);
      // Check the CRC
      if (ad7280_check_crc(tmp))
	return 1;
      // Return the conversion value
      pvalues[i] = (tmp >> 11) & 0xFFF;
    }
  return 0;
}

int channel_count(t_ad7280_state *st)
{
  int i, count;

  for (i = 0, count = 0; i < MAXMODULES; i++)
    {
      count += st->chan_cnt[i];
    }
  return count;
}

// Read all the channels
char ad7280_read_all_channels(t_ad7280_state *st, unsigned int *array, int channel_count)
{
  int           i;
  unsigned long tmp;

#ifdef USE_RAW_WRITES
  ad7280_write_raw(0x038011CAL);
  ad7280_write_raw(0x01AA1062L);
  ad7280_write_raw(0x03A0546AL);
#else
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_READ, 1, AD7280A_CELL_VOLTAGE_1 << 2);
  // Read all and start conversion
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_6CELL_AUX1_3_4 |
	       AD7280A_CTRL_HB_CONV_RES_READ_6CELL_AUX1_3_4);
  // Enable CNVST on falling edge
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CNVST_CONTROL, 1, 0x02);
#endif
  // CNVST pin to 0
  CBI(PORTC, CNVSTART);
  // Wait for the conversion
  ad7280_delay();
  // CNVST pin to 0
  SBI(PORTC, CNVSTART);
  // Read all the channels
  for (i = 0; i < channel_count; i++)
    {
      __ad7280_read32(&tmp);
      // Check the message CRC
      if (ad7280_check_crc(tmp))
	return 1;
      // Extract the conversion values
      tmp = ((tmp >> 11) & 0xFFFL);
      array[i] = tmp;
    }
  // Disable CNVST on falling edge
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CNVST_CONTROL, 1, 0x01);
  return 0;
}

void ad7280_force_PW_DOWN(void)
{
  PORTD = PORTD & ~(PD4 | PD3);
  CBI(PORTC, PC5);
  _delay_ms(10);
  SBI(PORTD, PD3);
  SBI(PORTD, PD4);
  SBI(PORTC, PC5);
  _delay_ms(10);
}

void ad7280_force_default_control(void)
{
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1, 0);
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1, 0x13);  
}

int ad7280_chain_setup(t_ad7280_state *st)
{
  unsigned long val;
  int           n;

#ifdef DEBUG_AD7280
  uart_puts_P(PSTR("AD7280A before 1rst write\n"));
  _delay_ms(1000);
#endif
  // Software Reset + Initialise the addresses on the chain and lock them
#ifdef USE_RAW_WRITES
  // Reset
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_MUST_SET |
	       AD7280A_CTRL_LB_SWRST);
  // Init
  ad7280_write_raw(0x01C2B6E2L);
  ad7280_write_raw(0x038716CAL);
#else
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_MUST_SET |
	       AD7280A_CTRL_LB_SWRST |
	       st->ctrl_lb);
  _delay_ms(1);
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_MUST_SET |
	       st->ctrl_lb);
  // Control read of all the devices in the chain
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_READ, 1, AD7280A_CONTROL_LB << 2);
#endif
  // Read once to check if it works
  n = 0;
  while (n < CFGAD728AMODULES)
    {
      __ad7280_read32(&val);
#ifdef DEBUG_AD7280
      uart_puts_P(PSTR("- module\n"));
      _delay_ms(500);
#endif
      if (val == 0) // Error a module should be here
	{
#ifdef DEBUG_AD7280
	  uart_puts_P(PSTR("AD7280A module error\n"));
	  _delay_ms(500);
#endif
	  return ERR_MODULE_ALL_ZERO;
	}
      if (ad7280_check_crc(val)) // Error
	{
#ifdef DEBUG_AD7280
	  uart_puts_P(PSTR("crc error\n"));
	  _delay_ms(500);
#endif
	  return ERR_MODULE_CRC;
	}
      if (n != AD7280A_DEVADDR(val >> 27))
	{
#ifdef DEBUG_AD7280
	  uart_puts_P(PSTR("AD7280 invalid address\n"));
	  _delay_ms(500);
#endif
	  return ERR_MODULE_COUNT;
	}
      n++;
    }
  // Last read should be Zero
  __ad7280_read32(&val);
  if (val != 0)
    {
#ifdef DEBUG_AD7280
      uart_puts_P(PSTR("AD7280A last read error\n"));
      _delay_ms(500);
#endif
      return ERR_MODULE_ALL_ZERO;
    }
#ifdef DEBUG_AD7280
  uart_puts_P(PSTR("AD7280A setup ok\n"));
  _delay_ms(500);
#endif
  return CFGAD728AMODULES;
}

unsigned long AD7280Adac_AUX_2_mv(unsigned long cnv)
{
  cnv = ((cnv * 1220L) / 1000L); // 1220,A5(BV / LSB
  return cnv;
}

unsigned long AD7280Adac_CELL_2_mv(unsigned long cnv)
{
  cnv = ((cnv * 976L) / 1000L) + 1000L; // 976,A5(BV / LSB + 1V offset
  return cnv;
}

int ad7280_get_VBAT(t_ad7280_state *st, unsigned long *pvbat, int *ptemp)
{
  int           i, ret;
  unsigned int  varray[MAX_CHANNELS];
  int           chan, module, Vchan;
  unsigned long cnv;

  ret = ad7280_read_all_channels(st, varray, channel_count(st));
  if (!ret)
    {
      chan = Vchan = 0;
      for (module = 0; module < MAXMODULES; module++)
	{
	  if (st->chan_cnt[module] > 0)
	    {
	      for (i = 0; i < st->chan_cnt[module]; i++)
		{
		  // In 5 battery configuration, pass the 5th element Vin value.
		  if (!(i == 4 && st->chan_cnt[module] == 5))
		    {
		      cnv = varray[chan];
		      pvbat[Vchan] = AD7280Adac_CELL_2_mv(cnv); // mV
		    }
		  Vchan++;
		  chan++;
		}
	      // Temperature
	      cnv = varray[chan];
	      cnv = ((cnv * 122) / 100); // 1,22mV / LSB, 0V offset
	      // Convert it to ,A0(BC
	      ptemp[module] = mv_to_temperature(cnv);
	      chan += 3; // Hardcoded shit, it retuns only 3 aux temperatures 1, 3 and 5
	    }
	}
    }
  return ret;
}

char ad7280_set_balance(t_ad7280_state *st, unsigned long balancing)
{
  int           i, bitnumber;
  unsigned char reg;
  int           rb;

  // Set the values per module and send them
  for (i = 0; i < CFGAD728AMODULES; i++)
    {
      reg = 0;
      for (bitnumber = 0; bitnumber < st->chan_cnt[i]; i++)
	{
	  reg = balancing & (1 << bitnumber);
	}
      st->cb_mask[i] = reg;
      ad7280_write(i, AD7280A_CELL_BALANCE, 0, st->cb_mask[i]);
      // Next module
      balancing = (balancing >> st->chan_cnt[i]);
    }  
  // Check the values
  for (i = 0; i < CFGAD728AMODULES; i++)
    {
      rb = ad7280_read(st, i, AD7280A_CELL_BALANCE);
      if (rb != st->cb_mask[i])
	{
	  //add_error_log_EEPROM(ERROR_CODE_BALANCING_FAILED);
	  // Clear the balancing register
	  ad7280_write(i, AD7280A_CELL_BALANCE, 0, 0);
	  return 1;
	}
    }
  return 0;
}

char ad7280_get_balance(t_ad7280_state *st, char element)
{
  int           i, channel_cnt;
  unsigned long mask;

  channel_cnt = 0;
  mask = 0;
  for (i = 0; i < CFGAD728AMODULES; i++)
    {
      mask = mask | ((unsigned long)st->cb_mask[i] << channel_cnt);
      channel_cnt += st->chan_cnt[i];
    }
  return ((mask & (1L << element)) != 0);
}

// initialises the ALERT thresholds
void ad7280_channel_init(t_ad7280_state *st)
{
  // However the default value 1V min - 5V max are enough given the fact that
  // the alert is not used and checked in the ,A5(BC.
  // The threshold registers are 8bits and not 12bits.
  //st->slave_num = ret; // If not < 0 this is the number of devices
  //st->cell_threshhigh = 0xFF;
  //st->aux_threshhigh  = 0xFF;
  // Initialise the channels
  // Disable ALERT output, the ,A5(BC will check everything. In this applicaiton,
  // the modules are just a hightech spi dac and output buffer.
  // The ALERT is in defaultmode: no ALERT signal
/*   ret = ad7280_write(st, AD7280A_DEVADDR_MASTER, */
/* 		     AD7280A_ALERT, 1, */
/* 		     AD7280A_ALERT_RELAY_SIG_CHAIN_DOWN); */
/*   if (ret) */
/*     return 1; */
  // Disable the ALERT output, default
/*   ret = ad7280_write(st, AD7280A_DEVADDR(st->slave_num), */
/* 		     AD7280A_ALERT, 0, */
/* 		     AD7280A_ALERT_GEN_STATIC_HIGH | */
/* 		     (pdata->chain_last_alert_ignore & 0xF)); */
// Max and min alert values tresholds conversions in mv
/*  case AD7280A_CELL_OVERVOLTAGE: */
/*  case AD7280A_CELL_UNDERVOLTAGE: */
/*    val = ((val - 1000) * 100) / 1568; // LSB 15.68mV */
/*    break; */
/*  case AD7280A_AUX_ADC_OVERVOLTAGE: */
/*  case AD7280A_AUX_ADC_UNDERVOLTAGE: */
/*    val = (val * 10) / 196; // LSB 19.6mV */
/*  case AD7280A_CELL_OVERVOLTAGE: */
/*    val = 1000 + (st->cell_threshhigh * 1568) / 100; */
/*    break; */
/*  case AD7280A_CELL_UNDERVOLTAGE: */
/*    val = 1000 + (st->cell_threshlow * 1568) / 100; */
/*    break; */
/*  case AD7280A_AUX_ADC_OVERVOLTAGE: */
/*    val = (st->aux_threshhigh * 196) / 10; */
/*    break; */
/*  case AD7280A_AUX_ADC_UNDERVOLTAGE: */
/*    val = (st->aux_threshlow * 196) / 10; */
}

/*
void ad7280_software_reset(t_ad7280_state *st)
{
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1, 0x00);
  ad7280_write(AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_MUST_SET |
	       AD7280A_CTRL_LB_SWRST |    // Software Reset
	       st->ctrl_lb);
}*/

int init_AD7820A(t_ad7280_state *st)
{
  char success = 0;
  int  ret;
  char conversion_averaging = AD7280A_CTRL_HB_CONV_AVG_DIS;

  //
  // Setup the channels per module count (from env.h)
  //
  st->chan_cnt[0] = CFGAD728AMODULE_0_CHAN;
  st->chan_cnt[1] = CFGAD728AMODULE_1_CHAN;
  st->chan_cnt[2] = CFGAD728AMODULE_2_CHAN;
  // Cell balance masks to all MosFets disabled.
  st->cb_mask[0] = st->cb_mask[1] = st->cb_mask[2] = 0;
  //
  // Control register values:
  // - Thermistor res enabled
  // - 6 cell + 3 AUX
  st->ctrl_hb = conversion_averaging | AD7280A_CTRL_HB_CONV_INPUT_6CELL_AUX1_3_4 | AD7280A_CTRL_HB_CONV_RES_READ_6CELL_AUX1_3_4;
  // Do not change the conversion time, 1600ns required for the termination thermistor.
  st->ctrl_lb = AD7280A_CTRL_LB_ACQ_TIME_1600ns | AD7280A_CTRL_LB_THERMISTOR_EN;
  //
  // Reset
  //ad7280_software_reset(st);
  // Initialise the daisy chain addresses and check if it transmits
  while (success == 0)
    {
      ret = ad7280_chain_setup(st);
      if (ret > 0)
	{
	  success = 1;
	}
      else
	{
	  switch (ret)
	    {
	    case ERR_MODULE_ALL_ZERO:
	    case ERR_MODULE_COUNT:
	      {
		ad7280_force_PW_DOWN();
	      }
	      break;
	    case ERR_MODULE_CONTROL:
	    case ERR_MODULE_CRC:
	      {
		ad7280_force_PW_DOWN();
		ad7280_force_default_control();
	      }
	      break;
	    default:
	      break;
	    };
	}
#ifdef DEBUG_AD7280
      _delay_ms(1000);
      if (success)
	uart_puts_P(PSTR("AD7280A chain setup ok.\n"));
      else
	uart_puts_P(PSTR("AD7280A chain setup fail.\n"));
      _delay_ms(1000);
#endif
    }
  st->slave_num = ret; // If not < 0 this is the number of devices
  st->cell_threshhigh = 0xFF;
  st->aux_threshhigh  = 0xFF;
  // Initialise the channels
  //ad7280_channel_init(st);
  // Ready for use
  return 0;
}

