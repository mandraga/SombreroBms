
#define SBI(port, bit) ((port) |= (1 << (bit)))
#define CBI(port, bit) ((port) &= ~(1 << (bit)))

// PORTB
#define CSBAT  PB0
#define BUZZER PB1
#define PD     PB2
#define MOSI   PB3
#define MISO   PB4
#define SCLK   PB5
#define OSC1   PB6
#define OSC2   PB7

// PORTC
#define CNVSTART  PC0
#define NC1       PC1
#define NC2       PC2
#define RTS       PC3
#define CTS       PC4
#define RELON     PC5
#define RESET     PC6

// PORTD
#define RXD           PD0
#define TXD           PD1
#define ALERT         PD2
#define CHARGER_ON    PD3
#define CSDAC         PD4
#define STOP_CHARGER  PD5
#define LED_BALANCING PD6
#define LED_ERROR     PD7

// Main state machine
#define STATE_START    0
#define STATE_CHARGEON 1  // The charger is on
#define STATE_CHARGING 2  // same plus charging
#define STATE_IDLE     3  // Idle, does nothing
#define STATE_RUN      4  // Run, the current is beingused and state of charge updated
#define STATE_RELAPSE  5  // After run, wait for the battery voltage to spatbilise (lithium behaviour)
#define STATE_SECURITY 6  // Undervoltage or overvoltage or error like trying to start when charging

#define BAUDRATE       9600
#indef F_CPU
#define F_CPU       3686000
#endif

void _delay_ms_S(int millisecond);

#define MAXBATTERY 18

typedef struct s_vbat
{
  int          vbat[MAXBATTERY];
  int          batnum;
}              t_vbat;

typedef struct s_ibat
{
  int          ibat;
  int          timeslice_ms;
}              t_ibat;

