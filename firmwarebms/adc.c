
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "main.h"

void init_adc(void)
{
  // Prescaler to F_CPU =  7372800 / 32 = 230400
  ADCSRA |= (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
  // Reference voltage is the internal 2.56V with an external capacitor
  ADMUX = (0 << REFS1) | (0 << REFS0);
  // 8bit only, the result is in ADCH register
  ADMUX |= (1 << ADLAR);
  // Free runing
  //ADCSRA |= (1 << ADFR);
  // Enable
  ADCSRA |= (1 << ADEN);
  // Start the free running conversion
  ADCSRA |= (1 << ADSC);
}

unsigned char get_val(void)
{
  unsigned char res;

  // Start the free running conversion
  SBI(ADCSRA, ADSC);
  // Wait for conversion to finish
  while ((ADCSRA & (1 << ADIF)) == 0);
  res = ADCH;
  // Clear the interrupt flag
  SBI(ADCSRA, ADIF);
  return res;
}

void get_3values(char *pv)
{
  // Enable the adc
  SBI(ADCSRA, ADEN);
  // Set mux to adc 0
  ADMUX |= (0 << REFS1) | (0 << REFS0) | 0;
  pv[0] = get_val();

  // Set mux to adc 1
  ADMUX |= (0 << REFS1) | (0 << REFS0) | 1;
  pv[1] = get_val();

  // Set mux to adc 2
  ADMUX |= (0 << REFS1) | (0 << REFS0) | 2;
  pv[2] = get_val();
  // Disable ADC
  CBI(ADCSRA, ADEN);
}

