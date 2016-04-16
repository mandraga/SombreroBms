
#define SBI(port, bit) ((port) |= (1 << (bit)))
#define CBI(port, bit) ((port) &= ~(1 << (bit)))

// PORTB
#define STOP_CHARGER  PB0
#define CHARGER_ON    PB1
#define RELON         PB2
#define MOSI          PB3
#define MISO          PB4
#define SCLK          PB5
#define OSC1          PB6
#define OSC2          PB7

// PORTC
#define NC0           PC0
#define GAUGE         PC1
#define RTS           PC2
#define CTS           PC3
#define NC4           PC4
#define CNVSTART      PC5
#define RESET         PC6

// PORTD
#define RXD           PD0
#define TXD           PD1
#define ALERT         PD2
#define CSBAT         PD3
#define PDOWN         PD4
#define LED_BALANCING PD5
#define LED_ERROR     PD6
#define BUZZER        PD7

// Main state machine
#define STATE_START    0
#define STATE_CHARGEON 1  // The charger is on
#define STATE_CHARGING 2  // same plus charging
#define STATE_IDLE     3  // Idle, does nothing
#define STATE_RUN      4  // Run, the current is beingused and state of charge updated
#define STATE_RELAX    5  // After run, wait for the battery voltage to spatbilise (lithium behaviour)
#define STATE_SECURITY 6  // Undervoltage or overvoltage or error like trying to start when charging
#define STATE_CURRENT_SECURITY 7  // Over current or too much current during charge
#define STATE_CRITICAL_FAILURE 8  // Stops here, no way to get out but a reset

void _delay_ms_S(int millisecond);

typedef struct  s_ibat
{
  unsigned long ibat; // milliAmps
  int           timeslice_ms;
}               t_ibat;

// EEPROM data
typedef struct  s_eeprom_data
{
  int           install_date_year;
  char          install_date_month;
  char          install_date_day;
  unsigned long bat_maxv;            // mV
  unsigned long bat_minv;
  int           bat_tmax;            // temperature °C
  int           bat_tmin;
  char          bat_elements;
  int           full_charge;         // AH
  unsigned long full_chargeV;        // mV when the pack voltage reaches this value, the charger is stoped and the charge is set at 100%
  int           max_current;         // Amp
  char          serial_number[10];   //  8 bytes             plus 0 end of string
  char          client[33];          // 32 bytes Client name plus 0 end of string
  // These are often programmed
  int           charge_cycles;
  unsigned long charge_time_minutes; // Total charging time
  int           min_temperature;     // Extreme temperatures °C
  int           max_temperature;
}               t_eeprom_data;

// Per battery eeprom information
typedef struct  s_eeprom_battery
{
  unsigned long lowestV;
  int           lowVevents; // times vbat whent under the treshold before a charge
  unsigned long cap;        // More or less the capacity, or charge speed...
}               t_eeprom_battery;

// Ram data
typedef struct  s_pack_variable_data
{
  unsigned int  uptime_days;
  unsigned int  uptime_minutes;
  unsigned int  tseconds;                 // Tenth of seconds
  int           max_discharge;            // Discharge current in Amperes
  unsigned long c_discharge;              // Discharge, last value mA
  unsigned int  state_of_charge;          // SOC mili Ampere Hour
  unsigned long vbat[MAXBATTERY];         // Batterey milivolts, last value
  unsigned long total_vbat;               // Pack voltage in mv
  int           temperature[MAXMODULES];  // °C
  char          app_state;                // State machine variable
  // Internal variables
  unsigned long c_discharge_accumulator;  // Accumulates mA per hour
  char          charging_started;         // Once started it must finish
  int           charge_time_count_tenth;  // 1/10 second
  int           charge_time_count;        // Minutes
  int           idle_counter;             // Used to get out of the relapse state
  char          prev_idle;                // Used to count the charge cycles
}               t_pack_variable_data;

