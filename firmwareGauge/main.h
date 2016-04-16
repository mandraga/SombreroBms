
#define SBI(port, bit) ((port) |= (1 << (bit)))
#define CBI(port, bit) ((port) &= ~(1 << (bit)))

// PORTs

#define LED10 PB0
#define LED9  PB2
#define LED8  PA6
#define LED7  PA7
#define LED6  PB1
#define LED5  PA0
#define LED4  PA2
#define LED3  PA5
#define LED2  PA4
#define LED1  PA3

#define ADCPIN PA1

void _delay_ms_S(int millisecond);

void setleds(char *pstates);

