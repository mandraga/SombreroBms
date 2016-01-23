//
// The BMS can receive commands:
//
// Each command finishes with a return char '\n'.
// And will result in the interrupt callback to send a string back.
//
//-------------------------------------------------------------------------------------
// ping
//
// Returns something like:
//
// Sombrero BMS C2015-2016 Vreemdelabs.com
// Firmware Version: 1.0
// Elements: 11
//
//-------------------------------------------------------------------------------------
// get_params
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
//
//-------------------------------------------------------------------------------------
// set_param -valuename value
// valuename:
//   date      dd/mm/yyyy
//   batVmin         2800              // In mV
//   batVmax         3600
//   tmin             -20
//   tmax              75
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
// get_report
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
// batbegin  1
// Vb: 3,230                     // V
// evt: 2                        // undervoltage events
// avgchgt: 4567                 // charge time average in seconds more = more capacity
// batbegin 2
// ....
// batbegin 11
// Vb: 3,210
// evt: 0
// avgchgt: 4643
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

#define SER_STATE_RECEIVE              100

// A battery element counter is also used on top of the state
#define SER_STATE_SEND_REPORT_START     50
#define SER_STATE_SEND_REPORT_VB        51
#define SER_STATE_SEND_REPORT_CHRG      52
#define SER_STATE_SEND_REPORT_CHRGMA    53
#define SER_STATE_SEND_REPORT_IMA       54
#define SER_STATE_SEND_REPORT_STATE     55
#define SER_STATE_SEND_REPORT_ELTS      56
#define SER_STATE_SEND_REPORT_BATBEGIN  57
#define SER_STATE_SEND_REPORT_BATVB     58
#define SER_STATE_SEND_REPORT_BATEVT    59
#define SER_STATE_SEND_REPORT_BATAVGT   60
#define SER_STATE_SEND_REPORT_END       61


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
