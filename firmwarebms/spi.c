
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"
#include "spi.h"

void set_SPI_pol(char pol)
{
  if (pol == CS7280APOL)
    {
      // Mode 1 CPOL = 0, SPHA = 1
      CBI(SPCR, CPOL);
      SBI(SPCR, CPHA);
    }
  /*
  else
    {
      CBI(SPCR, CPHA);
      CBI(SPCR, CPOL);
    }
  */
}

void select_device(char device)
{
  // SPI polarity
  if (device == SPICS7280A)
    {
      set_SPI_pol(CS7280APOL);
    }
  else
    {
      set_SPI_pol(CS7280APOL);
    }
  // CHIP select low
  if (device == SPICS7280A)
    CBI(PORTD, CSBAT);
  else
    SBI(PORTD, CSBAT);
  _delay_us(40);
}

int init_spi_master()
{
  // Enable SPI in the power reduction register
  CBI(PRR, PRSPI);
  // SS/ must be set as ouput
  //
  // SPIE 0 interrupt enable
  // SPE  1 spi enable
  // DORD 0 data order MSB first
  // MSTR 1 Enable SPI as a master
  // CPOL Low when idle
  // CPHA 1
  // Prescaler F/64 (0 f/64) =  10
  // Prescaler F/16          =  01
  SPCR = (0 << SPIE) | (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (1 << CPHA) | (1 << SPR1) | (0 << SPR0);
  SPSR = (0 << SPI2X); // defaults to 0
  //
  // Nothing selected at start
  select_device(SPINOSEL);
  return 1;
}

void SPI_Master_write(unsigned char cData)
{
  // Start transmission
  SPDR = cData;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)));
  _delay_us(5);
}

unsigned char SPI_Master_read(unsigned char outByte)
{
  unsigned char result;

  // Start transmission
  SPDR = outByte;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)));
  result = SPDR;
  _delay_us(5);
  // Return the slave output
  return (result);
}

