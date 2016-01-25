#include "env.h"
#include "main.h"
#include "eeprom.h"
#include "serial.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];

t_serialport g_serial;

// Only used for debug purposes
void uart_puts(char *str)
{
  if (g_serial.RXstate == SER_STATE_IDLE)
    {
      snprintf(g_serial.outbuffer, TRSTRINGSZ, "%s", str);
      g_serial.TXstate = SER_STATE_SEND_DEBUG;
      g_serial.outsize = strlen(g_serial.outbuffer);
      g_serial.outindex = 0;
      // Start the transmission
      if (g_serial.outindex < g_serial.outsize)
	{
	  UDR0 = g_serial.outbuffer[g_serial.outindex++];
	}
    }
}

void init_serial_vars(void)
{
  g_serial.RXstate = SER_STATE_IDLE;
  g_serial.TXstate = SER_STATE_IDLE;
  g_serial.inbuffer[0] = '\n';
  g_serial.inindex = 0;
  g_serial.insize = 0;
  g_serial.outbuffer[0] = '\n';
  g_serial.outindex = 0;
  g_serial.outsize = 0;
}

void uart_init(unsigned int baudrate)
{
  // Set baud rate
  UBRR0H = (unsigned char)(baudrate >> 8);
  UBRR0L = (unsigned char)baudrate;
  // Enable receiver and transmitter + interrupts
  UCSR0B = (1 << RXCIE0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << TXEN0);
  // Set frame format: asynchronous, 8data, no parity, 1 stop bit
  UCSR0C = (3 << UCSZ00);
  init_serial_vars();
}

void replace_char(char a, char b, char *str)
{
  int len;
  int i;
  
  len = strlen(str);
  for (i = 0; i < len; i++)
    if (str[i] == a)
      str[i] = b;
}

// It starts at the '-' of "set_param -valuename value" and checks if valuename is
// refvalue. Returns 0 if not. And if equal it fills the pointer to the argument in
// *pstr and returns 1.
char check_valuename(char *refvalue, char *command_buffer, char index, char **pstr)
{
  char reflen;
  char remainlen;
  char i;

  reflen = strlen(refvalue);
  remainlen = strlen(command_buffer);
  // Eliminate cases where it cannot fit
  if ((remainlen - index) <= reflen + 2)
    return 0;
  if (strncmp(refvalue, command_buffer, reflen) == 0)
    {
      // Jump over spaces
      i = index + reflen;
      while (command_buffer[i] == ' ' && i < remainlen)
	i++;
      if (i >= remainlen)
	return 0;
      *pstr = &command_buffer[i];
      return 1;
    }
  *pstr = NULL;
  return 0;
}

void execute_config_command(char *command_buffer, char size)
{
  char i, index;
  char found;
  char *str;

  // Find the command type
  i = 0;
  found = 0;
  while ((i < size - 1) && !found)
    {
      if ((command_buffer[i] == '-'))
	{
	  found = 1;
	}
      i++;
    }
  if (found)
    {
      if (check_valuename("date", command_buffer, i, &str))
	{
	  set_install_date_EEPROM(str);
	}
      if (check_valuename("batVmin", command_buffer, i, &str))
	{
	  set_min_Vbat_EEPROM(str);
	}
      if (check_valuename("batVmax", command_buffer, i, &str))
	{
	  set_max_Vbat_EEPROM(str);
	}
      if (check_valuename("tmin", command_buffer, i, &str))
	{
	  set_min_temperature_EEPROM(str);
	}
      if (check_valuename("tmax", command_buffer, i, &str))
	{
	  set_max_temperature_EEPROM(str);
	}
      if (check_valuename("fullcharge", command_buffer, i, &str))
	{
	  set_full_charge_value_mAH_EEPROM(str);	  
	}
      if (check_valuename("serial", command_buffer, i, &str))
	{
	  set_serial_number_EEPROM(str);
	}
      if (check_valuename("client", command_buffer, i, &str))
	{
	  set_client_name_EEPROM(str);
	}
    }
}

void process_serial_command()
{
  // Send the first line of the report message, the interrupt at the end of the byte
  // transmission will send the other lines.
  if (strcmp("get_report", g_serial.inbuffer) == 0)
    {
      snprintf(g_serial.outbuffer, TRSTRINGSZ, "bmsReportBegin\n");
      g_serial.TXstate = SER_STATE_SEND_REPORT_VB;
    }
  if (strcmp("ping", g_serial.inbuffer) == 0)
    {
      snprintf(g_serial.outbuffer, TRSTRINGSZ, "Sombrero BMS C2015-2016 Vreemdelabs.com\n");
      g_serial.TXstate = SER_STATE_SEND_PING1;
    }
  if (strcmp("get_params", g_serial.inbuffer) == 0)
    {
      snprintf(g_serial.outbuffer, TRSTRINGSZ, "setup date: %d/%d/%d\n",
	       g_edat.install_date_day,
	       g_edat.install_date_month,
	       g_edat.install_date_day);
      g_serial.TXstate = SER_STATE_SEND_DATE;
    }
  // This is a configuration command
  if (strcmp("set_param", g_serial.inbuffer) == 0)
    {
      execute_config_command(g_serial.inbuffer, g_serial.insize);
    }
  g_serial.outsize = strlen(g_serial.outbuffer);
  g_serial.outindex = 0;
  // Start the transmission
  if (g_serial.outindex < g_serial.outsize)
    {
      UDR0 = g_serial.outbuffer[g_serial.outindex++];
    }
}

// RX interrupt
ISR(USART_RXC_vect, ISR_BLOCK)
{
  char received;

  received = UDR0;
  g_serial.RXstate = SER_STATE_RECEIVE;
  g_serial.inbuffer[g_serial.inindex++] = received;
  if (received == '\n' ||
      g_serial.inindex >= g_serial.insize ||
      g_serial.inindex >= RCVSTRINGSZ)
    {
      g_serial.RXstate = SER_STATE_IDLE;
      if (received == '\n' && g_serial.inindex < RCVSTRINGSZ) // Otherwise there is an error somewhere
	{
	  g_serial.inbuffer[g_serial.inindex] = 0; // Add an end to form a string
	  process_serial_command();
	}
      g_serial.RXstate = SER_STATE_IDLE;
    }
}

int get_undervoltage_event_count(char bat)
{
  if (bat >= g_edat.bat_elements)
    return 0;
  return g_bat[bat].lowVevents;
}

int get_total_undervoltage_event_count()
{
  int  sum;
  char i;

  sum = 0;
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      sum += get_undervoltage_event_count(i);
    }
  return sum;
}

unsigned long get_charge_speed(char bat)
{
  if (bat >= g_edat.bat_elements)
    return 0;
  return g_bat[bat].cap;  
}

void get_element_vbat(char bat, int *pvbat, int *pmvbat)
{
  if (bat >= g_edat.bat_elements)
    {
      *pvbat  = 0;
      *pmvbat = 0;
      return ;
    }
  *pvbat  = g_appdata.vbat[bat] / 1000L;
  *pmvbat = (g_appdata.vbat[bat] % 1000L) / 100;
}

void get_element_vbat_lowest(char bat, int *pvbat, int *pmvbat)
{
  if (bat >= g_edat.bat_elements)
    {
      *pvbat  = 0;
      *pmvbat = 0;
      return ;
    }
  *pvbat  = g_bat[bat].lowestV / 1000L;
  *pmvbat = (g_bat[bat].lowestV % 1000L) / 100;
}

char change_TX_state(char TXstate)
{
  char nextState;

  g_serial.outindex = 0;
  g_serial.outbuffer[0] = 0;
  switch (TXstate)
    {
    case SER_STATE_IDLE:
      nextState = SER_STATE_IDLE;
      break;
    case SER_STATE_SEND_DEBUG;
      nextState = SER_STATE_IDLE;
      break;
      //-----------------------------------------------------------------------------
      // Response to "ping" command
    case SER_STATE_SEND_PING1:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Firmware Version: %d.%d\n", FIRMWARE_VERSION, FIRMWARE_SUBVERSION);
	nextState = SER_STATE_SEND_PING2;
      }
      break;
    case SER_STATE_SEND_PING2:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Elements: %d\n", g_edat.bat_elements);
	nextState = SER_STATE_SEND_PING3;
      }
      break;
    case SER_STATE_SEND_PING3:
      {
	g_serial.outsize = 0;
	nextState = SER_STATE_IDLE;
      }
      break;
      //-----------------------------------------------------------------------------
      // Response to "get_params" command
    case SER_STATE_SEND_DATE:
      {
	unsigned long charge_percent;

	charge_percent = 100L * g_appdata.state_of_charge / g_edat.full_charge;
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Charge: %d%\n", charge_percent);
	nextState = SER_STATE_SEND_CHARGE;
      }
      break;
    case SER_STATE_SEND_CHARGE:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "total capacity: %dAH\n", g_edat.full_charge / 1000L);
	nextState = SER_STATE_SEND_TOTALCAP;
      }
      break;
    case SER_STATE_SEND_TOTALCAP:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "charge cycles: %d\n", g_edat.charge_cycles);
	nextState = SER_STATE_SEND_CHARGECYCLES;
      }
      break;
    case SER_STATE_SEND_CHARGECYCLES:
      {
	unsigned long hours;

	hours = g_edat.charge_time_minutes / 60;
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "total charging time: %dh\n", hours);
	nextState = SER_STATE_SEND_TOTALCHRGTIME;
      }
      break;
    case SER_STATE_SEND_TOTALCHRGTIME:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Vmin: %d,%2dV\n", g_edat.bat_minv / 1000L, (g_edat.bat_minv % 1000L) / 10);
	nextState = SER_STATE_SEND_VMIN;
      }
      break;
    case SER_STATE_SEND_VMIN:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Vmin: %d,%2dV\n", g_edat.bat_maxv / 1000L, (g_edat.bat_maxv % 1000L) / 10);
	nextState = SER_STATE_SEND_VMAX;
      }
      break;
    case SER_STATE_SEND_VMAX:
      {
	unsigned long undevorlatege_evt_cnt;

	undevorlatege_evt_cnt = get_total_undervoltage_event_count();
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "undervoltage events: %d\n", undevorlatege_evt_cnt);
	nextState = SER_STATE_SEND_UNDERVOLTAGEEVTS;
      }
      break;
    case SER_STATE_SEND_UNDERVOLTAGEEVTS:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "maxdischarge: %dA\n", g_appdata.max_discharge);
	nextState = SER_STATE_SEND_MAXDISCHARGE;
      }
      break;
    case SER_STATE_SEND_MAXDISCHARGE:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "serial number: %s\n", g_edat.serial_number);
	nextState = SER_STATE_SEND_SERIAL;
      }
      break;
    case SER_STATE_SEND_SERIAL:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "client: %s\n", g_edat.client);
	nextState = SER_STATE_SEND_CLIENT;
      }
      break;
    case SER_STATE_SEND_CLIENT:
      {
	int years;
	int days;
	int hours;

	years = g_appdata.uptime_days / 365;
	days  = g_appdata.uptime_days % 365;
	hours = g_appdata.minutes / 60;
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "uptime: %dyears %ddays %dh\n", years, days, hours);
	nextState = SER_STATE_SEND_CLIENT;
      }
      break;
    case SER_STATE_SEND_UPTIME:
      {
	g_serial.outsize = 0;
	nextState = SER_STATE_IDLE; // Finished
      }
      break;
      //-----------------------------------------------------------------------------
      // Response to "get_report" command
    case SER_STATE_SEND_REPORT_VB:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Vb: %d,%d\n", g_appdata.total_vbat / 1000L, (g_appdata.total_vbat % 1000L) / 10);
	nextState = SER_STATE_SEND_REPORT_CHRG;
      }
      break;
    case SER_STATE_SEND_REPORT_CHRG:
      {
	unsigned long charge_percent;

	charge_percent = 100L * g_appdata.state_of_charge / g_edat.full_charge;
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "chrg: %d\n", charge_percent);
	nextState = SER_STATE_SEND_REPORT_CHRGMA;
      }
      break;
    case SER_STATE_SEND_REPORT_CHRGMA:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "chrgmAH: %d\n", g_appdata.state_of_charge);
	nextState = SER_STATE_SEND_REPORT_CHRGMA;
      }
      break;
    case SER_STATE_SEND_REPORT_IMA:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "ImAH: %d\n", g_appdata.c_discharge);
	nextState = SER_STATE_SEND_REPORT_STATE;
      }
      break;
    case SER_STATE_SEND_REPORT_STATE:
      {
	switch (g_appdata.app_state)
	  {
	  case STATE_START:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: init\n");
	    break;
	  case STATE_CHARGEON:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: chargeon\n");
	    break;
	  case STATE_CHARGING:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: charging\n");
	    break;
	  case STATE_IDLE:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: idle\n");
	    break;
	  case STATE_RUN:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: run\n");
	    break;
	  case STATE_RELAPSE:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: relapse\n");
	    break;
	  case STATE_SECURITY:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: security\n");
	    break;
	  case STATE_CRITICAL_FAILURE:
	  default:
	    snprintf(g_serial.outbuffer, TRSTRINGSZ, "State: failure\n");
	    break;
	  }
	nextState = SER_STATE_SEND_REPORT_ELTS;
      }
      break;
    case SER_STATE_SEND_REPORT_ELTS:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Elts: %d\n", g_edat.bat_elements);
	nextState = SER_STATE_SEND_REPORT_BATBEGIN;
      }
      break;
    case SER_STATE_SEND_REPORT_BATBEGIN:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "batbegin %d\n", g_serial.batcounter);
	g_serial.batcounter = 0;
	nextState = SER_STATE_SEND_REPORT_BATEVT;
      }
      break;
    case SER_STATE_SEND_REPORT_BATVB:
      {
	unsigned int vbat, mvbat;

	get_element_vbat(g_serial.batcounter, &vbat, &mvbat);
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "Vb: %d,%d\n", vbat, mvbat);
	g_serial.batcounter = 0;
	nextState = SER_STATE_SEND_REPORT_BATVLOW;
      }
      break;
    case SER_STATE_SEND_REPORT_BATVLOW:
      {
	unsigned int vbat, mvbat;

	get_element_vbat_lowest(g_serial.batcounter, &vbat, &mvbat);
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "VLowest: %d,%d\n", vbat, mvbat);
	g_serial.batcounter = 0;
	nextState = SER_STATE_SEND_REPORT_BATEVT;
      }
      break;      
    case SER_STATE_SEND_REPORT_BATEVT:
      {
	int underflow_events;

	underflow_events = get_undervoltage_event_count(g_serial.batcounter);
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "evt: %d\n", underflow_events);
	nextState = SER_STATE_SEND_REPORT_BATAVG;
      }
      break;
    case SER_STATE_SEND_REPORT_BATAVGT:
      {
	int average_charge_time; // Charge speed or whatever

	average_charge_time = get_charge_speed(g_serial.batcounter);
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "avgchgt: %d\n", average_charge_time);
	g_serial.outbuffer++;
	if (g_serial.outbuffer >= g_edat.bat_elements)
	  nextState = SER_STATE_SEND_REPORT_END;   // Finished
	else
	  nextState = SER_STATE_SEND_REPORT_BATBEGIN; // Next battery element
      }
      break;
    case SER_STATE_SEND_REPORT_END:
      {
	snprintf(g_serial.outbuffer, TRSTRINGSZ, "bmsReportEnd\n");
	nextState = SER_STATE_SEND_REPORT_FINISHED;
      }
      break;
    case SER_STATE_SEND_REPORT_FINISHED:
      {
	g_serial.outsize = 0;
	nextState = SER_STATE_IDLE;
      }
      break;
    default:
      g_serial.outsize = 0;
      nextState = SER_STATE_IDLE;
    }
  g_serial.outsize = strlen(g_serial.outbuffer);
  return nextState;
}

// TX interrupt
ISR(USART_TXC_vect, ISR_BLOCK)
{
  // The last byte was sent, send another one if available
  if (g_serial.outindex < g_serial.outsize)
    {
      // Put data into buffer, sends the data
      UDR0 = g_serial.outbuffer[g_serial.outindex++];
    }
  else
    {
      g_serial.TXstate = change_TX_state(g_serial.TXstate);
      if (g_serial.TXstate != SER_STATE_IDLE && g_serial.outsize != 0)
	UDR0 = g_serial.outbuffer[g_serial.outindex++];
    }
}

