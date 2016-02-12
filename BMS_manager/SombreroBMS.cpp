
#include "serialport.h"
#include "SomberoBMS.h"

CSombreroBMS::CSombreroBMS(CSerialPort *ser_port):
  m_ser_port(ser_port)
{
}

CSombreroBMS::~CSombreroBMS()
{
}

void CSombreroBMS::set_port(CSerialPort *ser_port)
{
  m_ser_port = ser_port;
}

void CSombreroBMS::ping(char *report, int buffer_size, int report_size)
{
  const inst cbuff_size = 4096;
  char       data[cbuff_size];
  int        i, read, rcv_bytes;
  bool       bgo_on;

  if (m_ser_port == NULL)
    return ;
  // Send the ping command
  snprintf(data, cbuff_size, "ping\n");
  m_ser_port->write_serial_port(data, strlen(data));
  // Read back the shit
  rcv_bytes = 0;
  while (bgo_on)
    {
      read = m_ser_port->read_serial_port(data, cbuff_size);
      rcv_bytes += read;
      for (i = 0; i < rcv_bytes; i++)
	{
	  if (data[i] == '\0')
	    bgo_on = false;
	}
    }
}

bool CSombreroBMS::get_next_line(char **pdata, int size, char *pcode, char *pvalue)
{
  int   i;
  char  pd;
  char *pbegin;
  char *pend;
  char *pcolumn;

  if (m_ser_port == NULL)
    return false;
  pbegin = pend = pcolumn = pd = *pdata;
  if (*pbegin == '\0')
    return false;
  i = 0;
  while (pd[i] != '\n' && pd[i] != '\0')
    {
      if (pd[i] == ':')
	pcolumn = &pd[i];
      i++;
    }
  pend = &pd[i];
  // next line
  *pdata = &pd[i + 1];
  // copy the code
  for (i = 0; i <  pcolumn - pbegin; i++)
    {
      pcode[i] = pbegin[i];
    }
  pcode[i] = '\0';
  // copy the value
  pcolumn++;
  while (pcolumn != pend && pcolumn == ' ')
    {
      pcolumn++;
    }
  i = 0;
  while (pcolumn != pend)
    {
      pvalue[i++] = *pcolumn;
      pcolumn++;
    }
  pvalue[i++] = '\0';
  if (*pend != '\n')
    return false;
  return true;
}

void CSombreroBMS::get_params(t_params *pparams)
{
  const inst cbuff_size = 4096;
  char       data[cbuff_size];
  int        i, read, rcv_bytes;
  bool       bgo_on;
  char      *pdata;
  char       code[cbuff_size];
  char       value[cbuff_size];

  if (m_ser_port == NULL)
    return ;
  // Send the ping command
  snprintf(data, cbuff_size, "get_params\n");
  m_ser_port->write_serial_port(data, strlen(data));
  // Read back the shit
  rcv_bytes = 0;
  while (bgo_on)
    {
      read = m_ser_port->read_serial_port(data, cbuff_size);
      rcv_bytes += read;
      for (i = 0; i < rcv_bytes && bgo_on; i++)
	{
	  if (data[i] == '\0')
	    {
	      bgo_on = false;
	    }
	}
    }
  // Read the structure
  pdata = data;
  while (get_next_line(&pdata, code, value))
    {
      if (sntrcmp(code, "setup date", cbuff_size))
	{
	  strncpy(pparams->setupdate, value, SETUP_DATE_SIZE * sizeof(char));
	}
      if (sntrcmp(code, "charge", cbuff_size))
	{
	  pparams->charge_percent = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "total capacity", cbuff_size))
	{
	  pparams->total_capacity = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "Vbat", cbuff_size))
	{
	  pparams->Vbat = strtof(value, NULL, 10);	  
	}
      if (sntrcmp(code, "Bat elements", cbuff_size))
	{
	  pparams->BatElements = strtol(value, NULL, 10);	  
	}
      if (sntrcmp(code, "charge cycles", cbuff_size))
	{
	  pparams->ChargeCycles = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "total charging time", cbuff_size))
	{
	  pparams->ChargeTimeTotal = strtol(value, NULL, 10);	  
	}
      if (sntrcmp(code, "Vmin", cbuff_size))
	{
	  pparams->EltVmin = strtof(value, NULL, 10);
	}
      if (sntrcmp(code, "Vmax", cbuff_size))
	{
	  pparams->EltVmax = strtof(value, NULL, 10);
	}
      if (sntrcmp(code, "MaxVbat", cbuff_size))
	{
	  pparams->MaxVbat = strtof(value, NULL, 10);
	}
      if (sntrcmp(code, "undervoltage events", cbuff_size))
	{
	  pparams->undervoltageEvts = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "maxdischarge", cbuff_size))
	{
	  pparams->maxdischarge = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "average discharge", cbuff_size))
	{
	  pparams->avgdischarge = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "serial number", cbuff_size))
	{
	  strncpy(pparams->serialnumber, value, SERNUM_SIZE * sizeof(char));
	}
      if (sntrcmp(code, "client", cbuff_size))
	{
	  strncpy(pparams->client, value, CLIENT_STR_SIZE * sizeof(char));
	}
      if (sntrcmp(code, "uptime", cbuff_size))
	{
	  strncpy(pparams->uptime, value, UPTIME_STR_SIZE * sizeof(char));
	}
      if (sntrcmp(code, "mintemperature", cbuff_size))
	{
	  pparams->tmin = strtol(value, NULL, 10);
	}
      if (sntrcmp(code, "maxtemperature", cbuff_size))
	{
	  pparams->tmax = strtol(value, NULL, 10);	  
	}
      if (sntrcmp(code, "temperature", cbuff_size))
	{
	  pparams->temperature = strtol(value, NULL, 10);
	}
    }
}

#define PACK_REPORT 1
#define ELMT_REPORT 2

void CSombreroBMS::get_report(t_report *preport)
{
  const inst cbuff_size = 4096;
  char       data[cbuff_size];
  int        i, read, rcv_bytes;
  bool       bgo_on;
  char      *pdata;
  char       code[cbuff_size];
  char       value[cbuff_size];
  int        state = PACK_REPORT;
  int        bat;

  if (m_ser_port == NULL)
    return ;
  // Send the ping command
  snprintf(data, cbuff_size, "get_report\n");
  m_ser_port->write_serial_port(data, strlen(data));
  // Read back the shit
  rcv_bytes = 0;
  while (bgo_on)
    {
      read = m_ser_port->read_serial_port(data, cbuff_size);
      rcv_bytes += read;
      for (i = 0; i < rcv_bytes && bgo_on; i++)
	{
	  if (data[i] == '\0')
	    {
	      bgo_on = false;
	    }
	}
    }
  // Read the structure
  pdata = data;
  while (get_next_line(&pdata, code, value))
    {
      switch (state)
	{
	case PACK_REPORT:
	  {
	    if (sntrcmp(code, "Vbat", cbuff_size))
	      {
		preport-> = strtof(value, NULL, 10);
	      }
	    if (sntrcmp(code, "chrg", cbuff_size))
	      {
		preport->charge_percent = strtol(value, NULL, 10);
	      }
	    if (sntrcmp(code, "chrgmAH", cbuff_size))
	      {
		preport->chargemAH = strtol(value, NULL, 10);
	      }
	    if (sntrcmp(code, "ImA", cbuff_size))
	      {
		preport->currentmA = strtol(value, NULL, 10);
	      }
	    if (sntrcmp(code, "state", cbuff_size))
	      {
		strncpy(preport->state, value, STATE_STR_SIZE * sizeof(char));
	      }
	    if (sntrcmp(code, "Elts", cbuff_size))
	      {
		preport->elements = strtol(value, NULL, 10);
	      }
	    if (sntrcmp(code, "temp", cbuff_size))
	      {
		preport->temperature = strtol(value, NULL, 10);
	      }
	    if (sntrcmp(code, "batbegin", cbuff_size))
	      {
		bat = strtol(value, NULL, 10) - 1;
		state = ELMT_REPORT;
	      }	    
	  }
	  break;
	case ELMT_REPORT:
	  {
	    if (bat >= 0 && bat < MAX_BAT_ELEMENTS)
	      {
		if (sntrcmp(code, "Vb", cbuff_size))
		  {
		    preport->elements[bat].Velement = strtof(value, NULL, 10);
		  }
		if (sntrcmp(code, "Lowest", cbuff_size))
		  {
		    preport->elements[bat].Vlowest = strtof(value, NULL, 10);
		  }
		if (sntrcmp(code, "evt", cbuff_size))
		  {
		    preport->elements[bat].events = strtol(value, NULL, 10);
		  }
		if (sntrcmp(code, "avgchgt", cbuff_size))
		  {
		    preport->elements[bat].average_charging_time = strtol(value, NULL, 10);
		  }
		if (sntrcmp(code, "balan", cbuff_size))
		  {
		    preport->elements[bat].balancing = (strtol(value, NULL, 10) != 0);
		  }
	      }
	    if (sntrcmp(code, "batbegin", cbuff_size))
	      {
		bat = strtol(value, NULL, 10) - 1;
		state = ELMT_REPORT;
	      }   
	  }
	  break;
	default:
	  break;
	};
    }
}

void CSombreroBMS::set_param_int(char *valuename, int value)
{
  const inst cbuff_size = 4096;
  char       data[cbuff_size];

  if (m_ser_port == NULL)
    return ;
  snprintf(data, cbuff_size, "set_param -%s %d\n", valuename, value);
  m_ser_port->write_serial_port(data, strlen(data));
}

void CSombreroBMS::set_param_str(char *valuename, char *pvalue)
{
  const inst cbuff_size = 4096;
  char       data[cbuff_size];

  if (m_ser_port == NULL)
    return ;
  snprintf(data, cbuff_size, "set_param -%s %s\n", valuename, pvalue);
  m_ser_port->write_serial_port(data, strlen(data));
}

