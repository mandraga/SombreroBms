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

void INT0_init(void)
{
  // INT0 low -> interruption
  //CBI(MCUCR, ISC01);
  //CBI(MCUCR, ISC00);
  //SBI(GIFR, INTF0);
  //SBI(GICR, INT0);
}

// INT0 interrupt, for SPDIF change/error handling
ISR(INT0_vect)
{
  // Interrupt disabled
  //CBI(GIFR, INTF0);
  //CBI(GICR, INT0);
  // Do nothing, it just wakes up from idle mode
}

// Enable the timer interrupt and the INT0/PD2 input interrupt
void configure_Interrupts(void)
{
  //set_sleep_mode(SLEEP_MODE_IDLE);  
}

void configure_Timer(void)
{
}

void enter_idle_mode(void)
{
  // reset the timer
  
  // cpu in idle mode, (IO clock and timers are activated)
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();
}

void test_uart(char *str)
{
  uart_puts(str);
}

// Called at start, sets the IO port direction, defaults the variables, 
// Initialises the SPI devices and configures the interrupts
void init(t_vbat *pvbat)
{
  int i;

  // IO ports
  // Port B:
  DDRB  = 0x00 | (1 << CSBAT) | (1 << BUZZER) | (1 << PD) | (1 << MOSI) | (1 << SCLK);
  PORTB = 0x00 | (1 << CSBAT) | (0 << BUZZER) | (1 << PD) | (0 << MOSI) | (0 << SCLK);
  // Port C:
  DDRC  = 0x00 | (1 << CNVSTART) | (1 << CTS) | (1 << RELON);
  PORTC = 0x00 | (1 << CNVSTART) | (0 << CTS) | (0 << RELON);
  // Port D:
  DDRD  = 0x00 | (1 << TXD) | (1 << CSDAC) | (1 << STOP_CHARGER) | (1 << LED_BALANCING) | (1 << LED_ERROR);
  PORTD = 0x00 | (1 << TXD) | (1 << CSDAC) | (1 << STOP_CHARGER) | (1 << LED_BALANCING) | (1 << LED_ERROR);

  // Defaults
  for (i = 0; i < MAXBATTERY; i++)
    pvbat->vbat = 0;

  // Wait 50ms second
  _delay_ms(50);

  // Init UART
  uart_init(UART_BAUD_SELECT(BAUDRATE, F_CPU));
  test_uart("startig the shit\n");

  // Init SPI
  init_spi_master();

  // Init the SPI device AD7820A
  if (!init_AD7820A())
    {
      uart_puts("AD7820A failed\n");
      while (1);
    }

  // Interrupt
  // One interrupt routine must be installed to handle the UART input commands
  

}
