//
// Description: Part of this file is taken from the linux driver source code from 
// Analog Devices and changed to fit in the atmel AVR.
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
unsigned char ad7280_calc_crc8(unsigned long val)
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
char ad7280_check_crc(unsigned long val)
{
  unsigned char crc;

  crc = ad7280_calc_crc8(val >> 10);
  if (crc != ((val >> 2) & 0xFF))
    {
      return 1;
    }
  return 0;
}

// After initiating a conversion sequence we need to wait until the
// conversion is done. The delay is typically in the range of 15..30 us
// however depending an the number of devices in the daisy chain and the
// number of averages taken, conversion delays and acquisition time options
// it may take up to 250us, in this case we better sleep instead of busy wait.
//
// But on the AVR at 3Mhz, we just wait for 4ms
void ad7280_delay(void)
{
  _delay_ms(4);
}

// SPI read a command from the device 32bits
char __ad7280_read32(unsigned long *val)
{
  unsigned long ret;

  select_device(SPICS7280A);
  ret = 0L         | SPI_Master_read((AD7280A_READ_TXVAL >> 24) & 0xFFL);
  ret = (ret << 8) | SPI_Master_read((AD7280A_READ_TXVAL >> 16) & 0xFFL);
  ret = (ret << 8) | SPI_Master_read((AD7280A_READ_TXVAL >>  8) & 0xFFL);
  ret = (ret << 8) | SPI_Master_read((AD7280A_READ_TXVAL >>  0) & 0xFFL);
  select_device(SPINOSEL);
  *val = ret;
  return 0;
}

void ad7280_write_raw(unsigned long reg)
{
  // Send the 32bits through SPI, MSB first
  select_device(SPICS7280A);
  SPI_Master_write((reg >> 24) & 0xFFL);
  SPI_Master_write((reg >> 16) & 0xFFL);
  SPI_Master_write((reg >>  8) & 0xFFL);
  SPI_Master_write((reg >>  0) & 0xFFL);
  select_device(SPINOSEL);
}

// SPI write a command 32bits
void ad7280_write(unsigned char  devaddr,
	          unsigned char  regaddr,
		  unsigned char  all,
		  unsigned char  val)
{
  unsigned long reg;

  // Device address
  // Note that address is less significant bit first 1 is 0x10000
  reg = ((unsigned long)devaddr) << 27;
  // Register address field
  reg = reg | ((unsigned long)regaddr) << 21;
  // Transmited value
  reg = reg | ((unsigned long)val & 0xFFL) << 13;
  // All bit
  reg = reg | ((unsigned long)all) << 12;
  // CRC from all this + b010
  reg = reg | ((unsigned long)ad7280_calc_crc8(reg >> 11)) << 3 | 0x02L;
  //
  ad7280_write_raw(reg);
}

// This is a register data read, not a conversion read
int ad7280_read(t_ad7280_state *st, unsigned char devaddr, unsigned char addr)
{
  unsigned long tmp;

  // turns off the read operation on all parts
  ad7280_write(AD7280A_DEVADDR_MASTER, 
	       AD7280A_CONTROL_HB, 1,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_NO |
	       st->ctrl_hb);
  // turns on the read operation on the addressed part
  ad7280_write(devaddr,
	       AD7280A_CONTROL_HB, 0,
	       AD7280A_CTRL_HB_CONV_INPUT_ALL |
	       AD7280A_CTRL_HB_CONV_RES_READ_ALL |
	       st->ctrl_hb);
  // Set register address on the part to be read from
  ad7280_write(devaddr, AD7280A_READ, 0, addr << 2);
  // Read the shit
  __ad7280_read32(&tmp);
  // Check the transfer CRC
  if (ad7280_check_crc(tmp))
    return 1;
  // Check return command register and address
  if (((tmp >> 27) != devaddr) || (((tmp >> 21) & 0x3F) != addr))
    return 1;
  // Return D20 to D13, the register data 8bits
  return (tmp >> 13) & 0xFF;
}

