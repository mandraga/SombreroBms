#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "SombreroBMS.h"

CSombreroBMS::CSombreroBMS()
{
  memset(&m_report, 0, sizeof(t_report));
  memset(&m_params, 0, sizeof(t_params));
}

CSombreroBMS::~CSombreroBMS()
{
}

bool CSombreroBMS::get_next_line(char **pdata, int size, char *pcode, char *pvalue)
{
  int   i;
  char *pd;
  char *pbegin;
  char *pend;
  char *pcolumn;

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
  // Command limiter like reportBegin
  if (pcolumn == pbegin)
    {
      pcolumn = pend;
    }
  // next line
  *pdata = &pd[i + 1];
  // copy the code
  for (i = 0; i <  pcolumn - pbegin; i++)
    {
      pcode[i] = pbegin[i];
    }
  pcode[i] = '\0';
  // copy the value
  if (pcolumn != pend)
    pcolumn++;
  while (pcolumn != pend && *pcolumn == ' ')
    {
      pcolumn++;
    }
  i = 0;
  while (pcolumn != pend && i < size)
    {
      pvalue[i++] = *pcolumn;
      pcolumn++;
    }
  pvalue[i++] = '\0';
  if (*pend != '\n')
    return false;
  return true;
}

bool CSombreroBMS::is_equal(char *code, const char *value)
{
  int len;

  len = strlen(value);
  return strncmp(code, value, len) == 0;
}

void CSombreroBMS::parse_BMS_params_string(t_params *pparams, char *string)
{
  const int  cbuff_size = 4096;
  char      *pdata;
  char       code[cbuff_size];
  char       value[cbuff_size];

  // Read the structure
  pdata = string;
  while (get_next_line(&pdata, cbuff_size, code, value))
    {
      if (is_equal(code, "setup date"))
	{
	  strncpy(pparams->setupdate, value, SETUP_DATE_SIZE * sizeof(char));
	}
      if (is_equal(code, "charge"))
	{
	  pparams->charge_percent = strtol(value, NULL, 10);
	}
      if (is_equal(code, "total capacity"))
	{
	  pparams->total_capacity = strtol(value, NULL, 10);
	}
      if (is_equal(code, "Vbat"))
	{
	  pparams->Vbat = strtof(value, NULL);	  
	}
      if (is_equal(code, "Elements"))
	{
	  pparams->BatElements = strtol(value, NULL, 10);	  
	}
      if (is_equal(code, "charge cycles"))
	{
	  pparams->ChargeCycles = strtol(value, NULL, 10);
	}
      if (is_equal(code, "total charging time"))
	{
	  pparams->ChargeTimeTotal = strtol(value, NULL, 10);	  
	}
      if (is_equal(code, "Vmin"))
	{
	  pparams->EltVmin = strtof(value, NULL);
	}
      if (is_equal(code, "Vmax"))
	{
	  pparams->EltVmax = strtof(value, NULL);
	}
      if (is_equal(code, "MaxVbat"))
	{
	  pparams->MaxVbat = strtof(value, NULL);
	}
      if (is_equal(code, "undervoltage events"))
	{
	  pparams->undervoltageEvts = strtol(value, NULL, 10);
	}
      if (is_equal(code, "maxdischarge"))
	{
	  pparams->maxdischarge = strtol(value, NULL, 10);
	}
      if (is_equal(code, "average discharge"))
	{
	  pparams->avgdischarge = strtol(value, NULL, 10);
	}
      if (is_equal(code, "serial number"))
	{
	  strncpy(pparams->serialnumber, value, SERNUM_SIZE * sizeof(char));
	}
      if (is_equal(code, "client"))
	{
	  strncpy(pparams->client, value, CLIENT_STR_SIZE * sizeof(char));
	}
      if (is_equal(code, "uptime"))
	{
	  strncpy(pparams->uptime, value, UPTIME_STR_SIZE * sizeof(char));
	}
      if (is_equal(code, "max recorded temp"))
	{
	  pparams->tmax = strtol(value, NULL, 10);	  
	}
      if (is_equal(code, "max temperature"))
	{
	  pparams->tmax = strtol(value, NULL, 10);	  
	}
      if (is_equal(code, "min recorded temp"))
	{
	  pparams->tmin = strtol(value, NULL, 10);
	}
      if (is_equal(code, "min temperature"))
	{
	  pparams->tmin = strtol(value, NULL, 10);
	}
      if (is_equal(code, "temperature"))
	{
	  pparams->temperature = strtol(value, NULL, 10);
	}
    }
}

#define PACK_REPORT     1
#define ELMT_REPORT     2
#define ELMT_REPORT_END 3

void CSombreroBMS::parse_BMS_report_string(t_report *preport, char *string)
{
  const int  cbuff_size = 4096;
  char      *pdata;
  char       code[cbuff_size];
  char       value[cbuff_size];
  int        state = PACK_REPORT;
  int        bat;

  // Read the structure
  pdata = string;
  bat = 0;
  while (get_next_line(&pdata, cbuff_size, code, value))
    {
      switch (state)
	{
	case PACK_REPORT:
	  {
	    if (is_equal(code, "Vb"))
	      {
		preport->Vbat = strtof(value, NULL);
	      }
	    if (is_equal(code, "chrg"))
	      {
		preport->charge_percent = strtol(value, NULL, 10);
		printf("charge percent = %d   value=%s\n", preport->charge_percent, value);
	      }
	    if (is_equal(code, "chAH"))
	      {
		preport->chargeAH = strtof(value, NULL);
	      }
	    if (is_equal(code, "ImA"))
	      {
		preport->currentmA = strtol(value, NULL, 10);
	      }
	    if (is_equal(code, "State"))
	      {
		strncpy(preport->state, value, STATE_STR_SIZE * sizeof(char));
	      }
	    if (is_equal(code, "Elts"))
	      {
		preport->elements = strtol(value, NULL, 10);
	      }
	    if (is_equal(code, "temp"))
	      {
		preport->temperature = strtol(value, NULL, 10);
	      }
	    if (is_equal(code, "batbegin"))
	      {
		bat = strtol(value, NULL, 10);
		state = ELMT_REPORT;
	      }	    
	  }
	  break;
	case ELMT_REPORT:
	  {
	    if (bat >= 0 && bat < MAX_BAT_ELEMENTS)
	      {
		if (is_equal(code, "Vb"))
		  {
		    preport->element_array[bat].Velement = strtof(value, NULL);
		  }
		if (is_equal(code, "Lowest"))
		  {
		    preport->element_array[bat].Vlowest = strtof(value, NULL);
		  }
		if (is_equal(code, "evt"))
		  {
		    preport->element_array[bat].events = strtol(value, NULL, 10);
		  }
		if (is_equal(code, "avgchgt"))
		  {
		    preport->element_array[bat].average_charging_time = strtol(value, NULL, 10);
		  }
		if (is_equal(code, "balan"))
		  {
		    preport->element_array[bat].balancing = (strtol(value, NULL, 10) != 0);
		  }
	      }
	    if (is_equal(code, "batbegin"))
	      {
		bat = strtol(value, NULL, 10);
		state = ELMT_REPORT;
	      }
	    if (is_equal(code, "bmsReportEnd"))
	      {
		// Save the current Vbat values
		for (int i = 0; i < MAX_BAT_ELEMENTS; i++)
		  {
		    if (i < preport->elements)
		      m_tmpmeasure.Vbat[i] = preport->element_array[i].Velement;
		    else
		      m_tmpmeasure.Vbat[i] = 0;
		  }
		add_history(&m_tmpmeasure);
		//
		state = ELMT_REPORT_END;
	      }
	  }
	  break;
	default:
	  break;
	};
    }
}

void CSombreroBMS::set_param_int(char *valuename, int value, char* str, int strsz)
{
  snprintf(str, strsz, "set_param -%s %d\n", valuename, value);
}

void CSombreroBMS::set_param_str(char *valuename, char *pvalue, char* str, int strsz)
{
  snprintf(str, strsz, "set_param -%s %s\n", valuename, pvalue);
}

void CSombreroBMS::add_history(t_batmeasurement *pm)
{
  m_Vbat_history.push_back(*pm);
}

int  CSombreroBMS::get_history_sz()
{
  return m_Vbat_history.size();
}

t_batmeasurement *CSombreroBMS::get_hitory_elt(int elt)
{
  if (elt < get_history_sz())
    {
      return &m_Vbat_history[elt];
    }
  return NULL;
}

