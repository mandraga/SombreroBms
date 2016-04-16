/******************************************************************************
 Title:    VBAT SOC led indicator
 Author:   Patrick Areny arenyp@yahoo.fr
 Date:     Mars 2016
 Software: AVR-GCC 3.3 
 Hardware: ATTINY84A using the internal 8Mhz RC clock
 Fuses:    Lo fuse SUT 10; CKSEL 0010 -> 8Mhz internal, slow rising power, CKDIV8 programmed b011000010 = 0x62
           Thus 1Mhz clock

           Hi fuse BOD at 2.5V b101   Lo = 0xDF - 2 = 0xDD

           Command line: avrdude -pt84 -P /dev/parport0 -c stk200  -Uhfuse:w:0xDD:m


 Description:
	Initialises the IO ports
        Setup a timer for 1/10 second
	Gets his display state from the delay on an input pin
        Displays on 10 led bars

*******************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

#include "vbat.h"
#include "main.h"

static char g_bargraph[10];

int read_adc(void)
{
  int v;

  v = 1;
  return v;
}

void State_Of_Charge_interrupt_routine(long vbat)
{
  long translated_vbat;

  translated_vbat = vbat * (1000. * CALIBRATION_V / CALIBRATION_CONV);
  fill_vbat_arr(g_bargraph, translated_vbat);
  setleds(g_bargraph);
}

void start_conversion(void)
{
  SBI(ADCSRA, ADSC);
}

void set_portA(char pin, char state)
{
  if (state == 0)
    SBI(PORTA, pin);
  else
    CBI(PORTA, pin);
}

void set_portB(char pin, char state)
{
  if (state == 0)
    SBI(PORTB, pin);
  else
    CBI(PORTB, pin);
}

void setled(char led, char state)
{
  switch (led)
    {
    case 0:
      set_portA(LED1,  state);
      break;
    case 1:
      set_portA(LED2,  state);
      break;
    case 2:
      set_portA(LED3,  state);
      break;
    case 3:
      set_portA(LED4,  state);
      break;
    case 4:
      set_portA(LED5,  state);
      break;
    case 5:
      set_portB(LED6,  state);
      break;
    case 6:
      set_portA(LED7,  state);
      break;
    case 7:
      set_portA(LED8,  state);
      break;
    case 8:
      set_portB(LED9,  state);
      break;
    case 9:
      set_portB(LED10, state);
      break;
    default:
      break;
    };
}

void setleds(char *pstates)
{
  int i;

  for (i = 0; i < 10; i++)
    setled(i, pstates[i]);
}

void toggle_leds(void)
{
  int i;

  if (g_bargraph[0] == 0)
    {
      for (i = 0; i < 10; i++)
	g_bargraph[i] = 1;
    }
  else
    {
      for (i = 0; i < 10; i++)
	g_bargraph[i] = 0;
    }
  setleds(g_bargraph);
}

// TIMER1 interrupt
ISR(TIM1_COMPA_vect)
{
  // Start aconversion, it will be completed on interrupts
  start_conversion();
  //toggle_leds();
}

// ADC interrupt
ISR(ADC_vect)
{
  unsigned long   vbat;

  //cli();
  vbat = ADCW;
  //  sei();

  //#define TESTADCINT
#ifdef  TESTADCINT
  static int e = 0;

  setled(e, 0);
  e = (e + 1) % 10;
  setled(e, 1);
#endif
  //#define SHOW_ADC_VALUE
#ifdef SHOW_ADC_VALUE
  for (int i = 0; i < 10; i++)
    setled(i, (vbat >> i) & 1);
#endif
  State_Of_Charge_interrupt_routine(vbat);
}

void configure_Timer(void)
{
  // interrupts must be disabled

  TCNT1  = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  // Start timer 1 in CTC channel A mode Table 12-2
  // WGM = 0100
  TCCR1A |= (0 << WGM11) | (0 << WGM10);
  TCCR1B |= (0 << WGM13) | (1 << WGM12) | (1 << CS12);   // Prescaler Table 12-6 clkIO/256 1000000/256=3906 ticks per second

  // 16 bit counter and register
  OCR1A = 390; // CTC Compare value, interrupt triggered every 390 counts -> 10 times per second
  TIMSK1 = (1 << OCIE1A); // TIMER1 channel A interrupt enabled
  //TIFR1  = (1 << OCF1A);  // no need automaticly set and clear when the interrupt routine is called
}

/*
void enter_idle_mode(void)
{
  // cpu in idle mode, (IO clock and timers are activated)
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();
}
*/

void ADC_setup(void)
{
  // Power the ADC
  CBI(PRR, PRADC);
  // Digital input disabled on PA1
  SBI(DIDR0, ADC1D);
  // Free runing mode, unipolar
  ADCSRB = 0x00;
  //
  ADMUX  = (1 << MUX5) || (1 << MUX0);
  ADCSRA = (1 << ADEN);
  _delay_ms(50);
  ADCSRA = (1 << ADEN) | (0 << ADIF) | (1 << ADIE) | 0x03;
}

void mysleep(void)
{
  for (int i = 0; i < 2; i++)
    _delay_ms(50);
}

int main(void)
{
  int  m;

  cli();

  // Everything in hight impedance
  // port A:
  DDRA  = 0xFD;
  //DDRA  = (1 << LED4) | (1 << LED1);
  PORTA = 0x00;

  // port B:
  DDRB  = 0x07;
  //DDRB  = 0x00;
  PORTB = 0x00;

  // Power down
  SBI(PRR, PRTIM0); // Timer0 disabled
  SBI(PRR, PRUSI);  // Serial interface disabled

  for (m = 0; m < 10; m++)
    setled(m, 1);

  // Wait 50ms second
  _delay_ms(50);

  // ADC
  ADC_setup();

  // Timer
  configure_Timer();

  sei();                         // enable interrupts

  for (m = 0;; m++)              // loop forever
    {
//#define GNULE
#ifdef GNULE
      if (m > 9)
	m = 0;

      mysleep();

      //set_portA(LED1,  1);
      //set_portA(LED4,  1);

      //toggle_leds();

      setled(m, 1);

      mysleep();

      //toggle_leds();

      //set_portA(LED1,  0);
      //set_portA(LED4,  0);

      setled(m, 0);

      // Sleep
#endif      
    }
}

