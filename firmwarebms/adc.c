
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "env.h"
#include "main.h"

void init_adc(void)
{
  // Free runing mode, not timer, no comparator
  ADCSRB = 0;
  // Prescaler to F_CPU =  7372800 / 32 = 230400
  ADCSRA |= (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
  // Datasheet page 225, ref selection + ADC7 on the mux
  ADMUX = (0 << REFS1) | (1 << REFS0) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  // Left adjust for 8bit only, the result is in ADCH register
  //ADMUX |= (1 << ADLAR);
  // Enable
  ADCSRA |= (1 << ADEN);
  // Start a conversion
  ADCSRA |= (1 << ADSC);
}

unsigned int get_val(void)
{
  unsigned int res;

  // Start the free running conversion
  SBI(ADCSRA, ADSC);
  // Wait for conversion to finish (ADIF == interrupt flag == 1)
  while ((ADCSRA & (1 << ADIF)) == 0);
  //res = ADCH;
  //res = res << 8;
  //res = res | ADCL;
  res = 0L | ADCW;
  // Clear the interrupt flag
  SBI(ADCSRA, ADIF);
  return res;
}

unsigned int get_adc7(void)
{
  unsigned int ret;

  // Enable the adc
  SBI(ADCSRA, ADEN);
  // Set mux to adc 7
  ADMUX = (0 << REFS1) | (1 << REFS0) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
  // Convert
  ret = get_val();
  // Disable ADC
  CBI(ADCSRA, ADEN);
  return ret;
}

// Returns the AVR temperature in °C
int get_uc_internal_temperature(void)
{
  return 0;
}
