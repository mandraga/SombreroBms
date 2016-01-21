
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "main.h"
#include "spi.h"


#define NOSEL    0
#define CS7280A  1
#define CSDAC    2
#define CSMEM    3

#define CS7280APOL 11
#define CS7280APOL 00

void set_SPI_pol(char pol)
{
  if (pol == CS7280APOL)
    {
      // Mode 1 CPOL = 0, SPHA = 2
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
  if (device == CS7280A)
    {
      set_SPI_pol(CS7280APOL);
    }
  else
    {
      set_SPI_pol(CS7280APOL);
    }
  // CHIP select low
  if (device == CS7280A)
    CBI(PORTB, CSBAT);
  else
    SBI(PORTB, CSBAT);
  //_delay_ms(1);
}

int init_spi_master()
{
  // Nothing selected at start
  select_device(NOSEL);
  // SPIE 0 interrupt enable
  // SPE  1 spi enable
  // DORD 0 data order MSB first
  // MSTR 1 Enable SPI as a master
  // CPOL Low when idle
  // CPHA 1
  // Prescaler F/16 (0 f/16)
  SPCR = (0 << SPIE) | (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (1 << SPR0);
  //SPSR = (1 << SPI2X);
  return 1;
}

void SPI_Master_write(unsigned char cData)
{
  // Start transmission
  SPDR = cData;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)));
}

unsigned char SPI_Master_read()
{
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)));
  return (SPDR);
}

