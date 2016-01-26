//
// The BMS can receive commands:
//
// Each command finishes with a return char '\n'.
// And will result in the interrupt callback to send a string back.
//
//-------------------------------------------------------------------------------------
// "ping\n"
//
// Returns something like:
//
// Sombrero BMS C2015-2016 Vreemdelabs.com
// Firmware Version: 1.0
// Elements: 11
//
//-------------------------------------------------------------------------------------
// "get_params\n"
//
// Returns all the configuration information in the following format:
//
// setup date: dd/mm/yyyy
// charge: 80%
// total capacity: 60AH
// Vbat: 36,23V
// Bat elements: 11
// charge cycles: 123
// total charging time: 56h
// Vmin: 2,9V
// Vmax: 3,6V
// undervoltage events: 0
// maxdischarge: 252A
// average discharge: 50A
// serial number: 12345678
// client: "golf des paquerettes"       // 32 chars maximum
// uptime: 0years 3days 5h
// mintemperature: -20°C
// maxtemperature: 60°C
// temperature: 23°C
//
//-------------------------------------------------------------------------------------
// "set_param -valuename value\n"
//
// valuename:
//   date      dd/mm/yyyy
//   batVmin         2800              // In mV
//   batVmax         3600
//   tmin             -20
//   tmax              75              // Use someting low, like 60°C
//   fullcharge     60000              // In mAH
//   serial      12345678
//   client      "golf des paquerettes"
//
// Example: set_param -client  "golf des paquerettes"
//          set_param -date    10/02/2017
//          set_param -batVmin 2900
//
// Note that bat elements comes from the firmware version 
//
//-------------------------------------------------------------------------------------
//
// "get_report\n"
//
// Returns each battery information + current + totalvbat + machine_state + charge
// Not too verbose in order to send it fast and often
//
// bmsReportBegin
// Vb: 35,56                     //  V
// chrg: 80
// chrgmAH: 40000
// ImA: 56323
// state: RUN
// Elts: 11
// temp: 23                      // °C this is the maximum temperature over the board
// batbegin  1
// Vb: 3,230                     // V
// Lowest: 2,83                  // V
// evt: 2                        // undervoltage events
// avgchgt: 4567                 // charge time average in seconds more = more capacity
// balan: 1
// batbegin 2
// ....
// batbegin 11
// Vb: 3,210
// Lowest: 2,92                   // V
// evt: 0
// avgchgt: 4643
// balan: 0
// bmsReportEnd
//
//

#define RCVSTRINGSZ 60
#define TRSTRINGSZ  42

// With 1K of memory, we will send line by line with a 64bytes buffer
// and change states when sending the '\n' at the end of the line.
#define SER_STATE_IDLE                   0
#define SER_STATE_SEND_PING1             1
#define SER_STATE_SEND_PING2             2
#define SER_STATE_SEND_PING3             3
#define SER_STATE_SEND_DATE              4
#define SER_STATE_SEND_CHARGE            5
#define SER_STATE_SEND_TOTALCAP          6
#define SER_STATE_SEND_CHARGECYCLES      7
#define SER_STATE_SEND_TOTALCHRGTIME     8
#define SER_STATE_SEND_VMIN              9
#define SER_STATE_SEND_VMAX             10
#define SER_STATE_SEND_UNDERVOLTAGEEVTS 11
#define SER_STATE_SEND_MAXDISCHARGE     12
#define SER_STATE_SEND_SERIAL           13
#define SER_STATE_SEND_CLIENT           14
#define SER_STATE_SEND_UPTIME           15
#define SER_STATE_SEND_MINT             16
#define SER_STATE_SEND_MAXT             17
#define SER_STATE_SEND_HITEMP           18

// A battery element counter is also used on top of the state
#define SER_STATE_SEND_REPORT_START     50
#define SER_STATE_SEND_REPORT_VB        51
#define SER_STATE_SEND_REPORT_CHRG      52
#define SER_STATE_SEND_REPORT_CHRGMA    53
#define SER_STATE_SEND_REPORT_IMA       54
#define SER_STATE_SEND_REPORT_STATE     55
#define SER_STATE_SEND_REPORT_ELTS      56
#define SER_STATE_SEND_REPORT_TEMP      57
#define SER_STATE_SEND_REPORT_BATBEGIN  58
#define SER_STATE_SEND_REPORT_BATVB     59
#define SER_STATE_SEND_REPORT_BATVLOW   60
#define SER_STATE_SEND_REPORT_BATEVT    61
#define SER_STATE_SEND_REPORT_BATAVGT   62
#define SER_STATE_SEND_REPORT_BATBALANC 63
#define SER_STATE_SEND_REPORT_END       64
#define SER_STATE_SEND_REPORT_FINISHED  65

#define SER_STATE_SEND_DEBUG            99
#define SER_STATE_RECEIVE              100

/*  Taken from Peter Fleury code, (it was too tigh here to use his library)
 *  @brief  UART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)

typedef struct s_serialport
{
  char         state;
  char         inbuffer[RCVSTRINGSZ];
  char         insize;
  char         inindex;
  char         outbuffer[TRSTRINGSZ];
  char         outsize;
  char         outindex;
  char         batcounter;
}              t_serialport;

void uart_puts(char *str);
void uart_init(unsigned int baudrate);

