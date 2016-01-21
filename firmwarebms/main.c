
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

#include "uart.h"
#include "spi.h"
#include "init.h"
#include "main.h"
#include "env.h"

void setled(char state)
{
  if (state)
    SBI(PORTD, LED1);
  else
    CBI(PORTD, LED1);
}

void uart1_send_bytes(unsigned char *pdat, int len)
{
  int i;
  
  for (i = 0; i < len; i++)
    {
      uart_putc(pdat[i]);
    }
}

int main(void)
{
  int  i;
  t_vbat vbat;
  t_ibat ibat;

  init(&vbat);

//#define ACC_MMA
#ifdef ACC_MMA
  // Configure MMA accelerometer
  Configure_MMA7455L();
#endif
  #define ACC_LTC
#ifdef ACC_LTC
  // Configure LT accelerometer
  Configure_LT_ACCEL();
#endif
  #define GYRO
#ifdef GYRO
  // Configure LT gyroscope
  Configure_LT_GYRO();
#endif
  // Init I2C
  i2c_init();
  #define TEMPSENSOR
#ifdef TEMPSENSOR
  // Configure the Barometer
  Configure_BARO();
#endif
#define ADC_M
#ifdef ADC_M
  init_adc();
#endif
  sei();                         // enable interrupts

  // Power led to on
  for (m = 0; m < 4; m++)                       // loop forever
    {
      setled(1);
      _delay_ms(50);
      //_delay_ms(50);
      setled(0);
      _delay_ms(50);
    }
  setled(0);

  for (m = 0;; m++)                       // loop forever
    {
      //_delay_ms_S(125);
      setled((m >> 2) & 1);
#ifdef TEMPSENSOR
      // Pression
      temperature = BARO_read_TEMP();
      pressure = BARO_read_PRESSURE();
#endif
#ifdef ACC_MMA
      // Accelerometres
      MMA[0] = MMA7455L_read_X();
      MMA[1] = MMA7455L_read_Y();
      MMA[2] = MMA7455L_read_Z();
#endif
#ifdef ACC_LTC
      LTA[0] = LTA_read_X();
      LTA[1] = LTA_read_Y();
      LTA[2] = LTA_read_Z();
#endif
#ifdef GYRO
      // Gyro
      LTG[0] = LTG_read_X();
      LTG[1] = LTG_read_Y();
      LTG[2] = LTG_read_Z();
#endif

      //get_3values(MIC);

      // Wait the end of the transmission here
      //_delay_ms(50);

      // BARxxxxxxxx
      // TEMxxxxxxxx
      // A1Xxxxxxxxx
      // A1Yxxxxxxxx
      // ...
      // G1zxxxxxxxx
/*
      select_device(CSMMA);
      _delay_ms(50);
      select_device(CSLTG);
      _delay_ms(50);
      select_device(CSLTA);
      _delay_ms(50);
*/
      //sprintf(sout, "BRR%lu BR%X TEM%lu AX%8d AY%8d AZ%8d GX%8d GY%8d GZ%8d\n\r", pressure, pressure, temperature, LTA[0], LTA[1], LTA[2], LTG[0], LTG[1], LTG[2]);
      //test_uart(sout);
      //_delay_ms(20);
      //sprintf(sout, "test test testage du truc là\n");
      // Pack data & crc
      char vp[3];

      vp[2] = (pressure >> 16) & 0xFF;
      vp[1] = (pressure >> 8) & 0xFF;
      vp[0] = (pressure >> 0) & 0xFF;
      sprintf(sout, "BAR%04X%04X%04X%04X%04X%04X%02X%02X%02X", LTA[0], LTA[1], LTA[2], LTG[0], LTG[1], LTG[2], vp[2], vp[1], vp[0]);
      //sprintf(sout, "BAR%04X", LTA[0]);
      int g = strlen(sout);
      for (i = 0, CRC = 0; i < g; i++)
	{
	  CRC ^= sout[i];
	}
      sprintf(&sout[g], " %02X\n\r", CRC & 0xFF);
      test_uart(sout);
      _delay_ms(4);
    }
}

