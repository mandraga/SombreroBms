/******************************************************************************
 Title:    Sombrero BMS
 Author:   Patrick Areny arenyp@yahoo.fr
 Date:     2016
 Software: AVR-GCC 3.3 
 Hardware: ATMEGA168PA

Description: Part of it is taken from the linux driver source code from Analog Devices and changed to fit the atmel AVR.

*******************************************************************************/
// Original Analog Deviecs code:
//
// MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
// MODULE_DESCRIPTION("Analog Devices AD7280A");
// MODULE_LICENSE("GPL v2");
//
//
// AD7280A Lithium Ion Battery Monitoring System
//
// Copyright 2011 Analog Devices Inc.
//
// Licensed under the GPL-2.
//

#include "spi.h"
#include "AD7280A.h"

// CRC section
// The CRC for a write  is calculated on the 21 bits 11 to 31 of
// a command:
// device address, register address, and data, addess all bit
// and bit D11 (reserved, 0).
// On a read it's 22bits
unsigned char ad7280_crc8_table(unsigned char index)
{
  unsigned char bit, crc;
  int i;

  crc = index;
  for (i = 0; i < 8; i++)
    {
      bit = crc & HIGHBIT;
      crc <<= 1;
      if (bit)
	crc ^= POLYNOM;
    }
  return (crc);
}

// Val is the 32bit command
unsigned char ad7280_calc_crc8(unsigned char *crc_tab, unsigned long val)
{
  unsigned char crc;
  unsigned char index;

  index = (val >> 16) & 0xFF;
  crc = ad7280_crc8_table(index);
  index = crc ^ ((val >> 8) & 0xFF);
  crc = ad7280_crc8_table(index);
  return  crc ^ (val & 0xFF);
}

// Note that the first 10bits are omited
char ad7280_check_crc(t_ad7280_state *st, unsigned long val)
{
  unsigned char crc;

  car = ad7280_calc_crc8(st->crc_tab, val >> 10);
  if (crc != ((val >> 2) & 0xFF))
    return 1;
  return 0;
}

// After initiating a conversion sequence we need to wait until the
// conversion is done. The delay is typically in the range of 15..30 us
// however depending an the number of devices in the daisy chain and the
// number of averages taken, conversion delays and acquisition time options
// it may take up to 250us, in this case we better sleep instead of busy wait.
//
// But on the AVR at 3Mhz, we just wait for 2000us
void ad7280_delay()
{
  _delay_ms(2);
}

// SPI read a command from the device 32bits
int __ad7280_read32(t_ad7280_state *st, unsigned long *val)
{
  unsigned long ret;

  select_device(CS7280A);
  ret = SPI_Master_read();
  ret = (ret << 8) | SPI_Master_read();
  ret = (ret << 8) | SPI_Master_read();
  ret = (ret << 8) | SPI_Master_read();
  select_device(NOSEL);
  *val = ret
  return 0;
}

// SPI write a command 32bits
int ad7280_write(t_ad7280_state *st,
		 unsigned char  devaddr,
		 unsigned char  addr,
		 unsigned char  all,
		 unsigned char  val)
{
  unsigned long reg;

  // device address
  reg = ((unsigned long)devaddr) << 27;
  // register address field
  reg = reg | ((unsigned long)addr) << 21;
  // transmited value
  reg = reg | ((unsigned long)val & 0xFF) << 13;
  // All bit
  reg = reg | ((unsigned long)all) << 12;
  // CRC from all this + b010
  reg |= ad7280_calc_crc8(st->crc_tab, reg >> 11) << 3 | 0x2;
  // Send the 32bits through SPI, MSB first
  select_device(CS7280A);
  SPI_Master_write(reg >> 24);
  SPI_Master_write((reg >> 16) & 0xFF);
  SPI_Master_write((reg >>  8) & 0xFF);
  SPI_Master_write(reg & 0xFF);
  select_device(NOSEL);
}

// This is a register data read, not a conversion read
int ad7280_read(t_ad7280_state *st, unsigned char devaddr, unsigned char addr)
{
  unsigned long tmp;

  // turns off the read operation on all parts
  ad7280_write(st, AD7280A_DEVADDR_MASTER, 
	       AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_NO |
	       st->ctrl_hb);
  // turns on the read operation on the addressed part
  ad7280_write(st, devaddr,
	       AD7280A_CONTROL_HB, 0,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_ALL |
	       st->ctrl_hb);
  // Set register address on the part to be read from
  ad7280_write(st, devaddr, AD7280A_READ, 0, addr << 2);
  // Read the shit
  __ad7280_read32(st, &tmp);
  // Check the transfer CRC
  if (ad7280_check_crc(st, tmp))
    return 1;
  // Check return command register and address
  if (((tmp >> 27) != devaddr) || (((tmp >> 21) & 0x3F) != addr))
    return 1;
  // Return D20 to D13, the register data 8bits
  return (tmp >> 13) & 0xFF;
}

// This is a conversion read
int ad7280_read_channel(t_ad7280_state *st, unsigned devaddr, unsigned addr)
{
  unsigned long tmp;

  ad7280_write(st, devaddr, AD7280A_READ, 0, addr << 2);
  // turns off the read operation on all parts
  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_NO |
	       st->ctrl_hb);
  // turns on the read operation on the addressed part
  ad7280_write(st, devaddr, AD7280A_CONTROL_HB, 0,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_ALL |
	       AD7280A_CTRL_HB_CONV_START_CS |
	       st->ctrl_hb);
  // Wait for the conversion
  ad7280_delay();
  // Read the channels
  __ad7280_read32(st, &tmp);
  // Check the CRC
  if (ad7280_check_crc(st, tmp))
    return 1;
  // Check the returned command addresses
  if (((tmp >> 27) != devaddr) || (((tmp >> 23) & 0xF) != addr))
    return 1;
  // Return the conversion value
  return (tmp >> 11) & 0xFFF;
}

// Read all the channels
void ad7280_read_all_channels(t_ad7280_state *st, unsigned int cnt, unsigned int *array, char *ptype)
{
  int           i;
  unsigned long tmp;

  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_READ, 1, AD7280A_CELL_VOLTAGE_1 << 2);
  // Read all and start conversion
  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_ALL |
	       AD7280A_CTRL_HB_CONV_START_CS |
	       st->ctrl_hb);
  // Wait for conversion
  ad7280_delay();
  // Read all the channels
  sum = 0;
  for (i = 0; i < cnt; i++)
    {
      __ad7280_read32(st, &tmp);
      // Check the message CRC
      if (ad7280_check_crc(st, tmp))
	return 1;
      // Save the value
      ptype[i] = (((tmp >> 23) & 0xF) <= AD7280A_CELL_VOLTAGE_6)? 1 : 0; 
      // Extract the conversion values
      tmp = ((tmp >> 11) & 0xFFF);
      array[i] = tmp;
    }
  return sum;
}

int ad7280_chain_setup(t_ad7280_state *st)
{
  unsigned val, n;
  int      ret;

  // Initialise the addresses on the chain and lock them
  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_MUST_SET |
	       AD7280A_CTRL_LB_SWRST |    // Software Reset
	       st->ctrl_lb);
  // Lock
  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_CONTROL_LB, 1,
	       AD7280A_CTRL_LB_DAISY_CHAIN_RB_EN |
	       AD7280A_CTRL_LB_LOCK_DEV_ADDR |
	       AD7280A_CTRL_LB_MUST_SET |
	       st->ctrl_lb);
  // Control read of all the devices in the chain
  ad7280_write(st, AD7280A_DEVADDR_MASTER, AD7280A_READ, 1, AD7280A_CONTROL_LB << 2);
  // Read once to check if it works
  for (n = 0; n <= AD7280A_MAX_CHAIN; n++)
    {
      __ad7280_read32(st, &val);
      if (val == 0) // Finished, return the number of devices in the dasy chain
	return n - 1;
      if (ad7280_check_crc(st, val)) // Error
	return -1;
      if (n != AD7280A_DEVADDR(val >> 27))
	return -1;
    }
  return -1;
}

int ad7280_initialise(t_ad7280_state *st)
{
  int  ret;
  char acquisition_time     = AD7280A_ACQ_TIME_1200ns;
  char conversion_averaging = AD7280A_CONV_AVG_DIS;
  char thermistor_term_en   = 1;

  // Control register values:
  // - Thermistor res enabled
  // - 6 cell + 3 AUX
  st->ctrl_hb = AD7280A_CTRL_LB_ACQ_TIME(acquisition_time & 0x3) | AD7280A_CTRL_HB_CONV_RES_READ_6CELL_AUX1_3_4;
  st->ctrl_lb = AD7280A_CTRL_HB_CONV_AVG(conversion_averaging & 0x3) | (thermistor_term_en != 0 ? AD7280A_CTRL_LB_THERMISTOR_EN : 0);

  // Initialise the daisy chain addresses and check if it transmits
  ret = ad7280_chain_setup(st);
  if (ret < 0)
    return ret;
  st->slave_num = ret; // If not < 0 this is the number of devices
  st->cell_threshhigh = 0xFF;
  st->aux_threshhigh  = 0xFF;
  // Initialise the channels
  ret = ad7280_channel_init(st);
  if (ret < 0)
    return ret;
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
/*    val = ((val - 1000) * 100) / 1568; /* LSB 15.68mV */ */
/*    break; */
/*  case AD7280A_AUX_ADC_OVERVOLTAGE: */
/*  case AD7280A_AUX_ADC_UNDERVOLTAGE: */
/*    val = (val * 10) / 196; /* LSB 19.6mV */ */
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

  // Ready for use
  return 0;
}

int ad7280_get_VBAT(t_ad7280_state *st, unsigned long *pvbat, unsigned long *ptemp)
{
  int           i, ret;
  unsigned int  varray[MAX_CHANNELS];
  char          type[MAX_CHANNELS];
  unsigned int  cnt;
  int           chan, module;
  unsigned long cnv;

  cnt = (6 + 3) * CFGAD728AMODULES;
  ret = ad7280_read_all_channels(st, cnt, varray, type);
  if (!ret)
    {
      for (i = 0; i < cnt; i++)
	{
	  chan = i % 9;
	  module = (i / 9);
	  if (chan < 6)
	    {
	      if (type[i]) // Check the type
		ret = 1;
	      cnv = varray[i];
	      cnv = ((cnv * 976) / 1000) + 1000; // 976,A5(BV / LSB, 1V offset
	      pvbat[module * 6 + i] = cnv; // mV
	    }
	  else
	    {
	      if (chan == 7)
		{
		  cnv = varray[i];
		  cnv = ((cnv * 122) / 100);  // 1,22mV / LSB, 0V offset
		  ptemp[module] = cnv;
		}
	    }
	}
    }
  return ret;
}

int ad7280_set_balance(t_ad7280_state *st, unsigned char *pbal)
{
  // Set the values per module
  
}

