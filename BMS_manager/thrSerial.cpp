#include <unistd.h>
#include <string>
#include <list>
#include <vector>
#include <termios.h>

#include <time.h>
#include <tinyxml.h>

// Linux and windows
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
//
#include "gfxareas.h"
#include "mesh.h"
#include "gl2Dprimitives.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>

#include "serial/serialport.h"
#include "cfgfile.h"
//
/*
#include "port_tab.h"
#include "about_tab.h"
#include "battery_tab.h"
#include "info_tab.h"
#include "setup_tab.h"
*/
#include "battery_tab.h"
#include "charge_tab.h"
//
#include "SombreroBMS.h"
#include "shared.h"
#include "main.h"
#include "thrSerial.h"


int g_thrSerialretval;

void add_command_in_locked_area(eserial_command type, const char *command, t_shared_data *pshared)
{
  t_serial_request rq;

  if (pshared->request.size() > 3)
    return;
  rq.type = type;
  strncpy(rq.command, command, MAX_MSG_SIZE);
  pshared->request.push_back(rq);
}

void send_command(t_serial_request *prq, t_shared_data *pshared)
{
  char             byte;
  char             message[MAX_MSG_SIZE];
  int              sz, ctn;
  //
  enum eserial_state
    {
      ser_nothing,
      ser_waiting_response
    };
  eserial_state ser_state;

  ser_state = ser_nothing;
  switch (prq->type)
    {
    case enothing:
      break;
    case eping:
      {
	while (pshared->pserial->read_next_byte(&byte)); // Empty the buffer
	snprintf(message, MAX_MSG_SIZE, "ping\n");
	pshared->pserial->write_serial_port(message, 1 + strlen(message));
	ser_state = ser_waiting_response;
      }
      break;
    case einformation:
      {
	while (pshared->pserial->read_next_byte(&byte)); // Empty the buffer
	snprintf(message, MAX_MSG_SIZE, "get_params\n");
	pshared->pserial->write_serial_port(message, 1 + strlen(message));
	ser_state = ser_waiting_response;
      }
      break;
    case ereport:
      {
	while (pshared->pserial->read_next_byte(&byte)); // Empty the buffer
	snprintf(message, MAX_MSG_SIZE, "get_report\n");
	pshared->pserial->write_serial_port(message, 1 + strlen(message));
	ser_state = ser_waiting_response;
      }
      break;
    case eprogram:
      {
	snprintf(message, MAX_MSG_SIZE, "set_param %s\n", prq->command);
	pshared->pserial->write_serial_port(message, 1 + strlen(message));
	usleep(500000);
	ser_state = ser_nothing;
      }
      break;
    case eupdategui:
    default:
      break;
    }
  if (ser_state == ser_waiting_response)
    {
      // Get the bytes from the serial line. Wait a little if nothing is found.
      sz = 0;
      for (ctn = 0; !pshared->pserial->read_next_byte(&byte) && ctn < 10; ctn++)
	usleep(10000);
      while (sz < MAX_MSG_SIZE && byte != 0 && ctn < 10) // Is the end of the message
	{
	  message[sz++] = byte;
	  for (ctn = 0; !pshared->pserial->read_next_byte(&byte) && ctn < 10; ctn++)
	    usleep(1000);
	}
      if (byte == 0 && sz > 0)
	{
	  message[sz] = '\0';
	  printf("<- Received a serial message: \"%s\".\n", message);
	  LOCK;
	  // Process the shit, like if it was a sewer pipe.
	  switch (prq->type)
	    {
	    case enothing:
	      break;
	    case eping:
	      {
		strncpy(pshared->bms_version, message, MAX_MSG_SIZE);
	      }
	      break;
	    case einformation:
	      {
		strncpy(pshared->param_msg, message, MAX_MSG_SIZE);
		pshared->pBMS->parse_BMS_params_string(&pshared->pBMS->m_params, pshared->param_msg);
	      }
	      break;
	    case ereport:
	      {
		strncpy(pshared->report_msg, message, MAX_MSG_SIZE);
		pshared->pBMS->parse_BMS_report_string(&pshared->pBMS->m_report, pshared->report_msg);
	      }
	      break;
	    case eprogram:
	    case eupdategui:
	    default:
	      break;
	    }
	  // This will trigger a screen refresh
	  add_command_in_locked_area(eupdategui, " ", pshared);
	  UNLOCK;
	}
    }
}

void* thr_serial(void *p_data)
{
  t_shared_data   *pshared = (t_shared_data*)p_data;
  bool             bquit;
  t_serial_request rq;
  std::list<t_serial_request>::iterator iter;

  g_thrSerialretval = 0;
  printf("Initialising the serial thread\n");
  LOCK;
  bquit = pshared->bquit;
  if (!bquit)
    add_command_in_locked_area(eping, " ", pshared); // Constant string, called once
  UNLOCK;
  usleep(10000);
  while (!bquit)
    {
      // Read a command
      LOCK;
      if (pshared->request.size())
	{
	  iter = pshared->request.begin();
	  while (iter != pshared->request.end())
	    {
	      if ((*iter).type != eupdategui)
		{
		  rq = (*iter);
		  iter = pshared->request.erase(iter);
		  if (pshared->pserial != NULL)
		    {
		      UNLOCK;
		      send_command(&rq, pshared);
		      LOCK;
		    }
		}
	      iter++;
	    }
	}
      bquit = pshared->bquit;
      UNLOCK;
      usleep(1000);
    }
  printf("Serial thread closing\n");
  return &g_thrSerialretval;
}

void start_serial_thread(t_shared_data *pshared)
{
  int ret;

  ret = pthread_create(&pshared->threadserial, NULL,
		       thr_serial, (void*)pshared);
  if (ret)
    printf("Serial thread creation error.");
}

void stop_serial_thread(t_shared_data *pshared)
{
  int *pretval;

  LOCK;
  pshared->bquit = true;
  UNLOCK;
  usleep(200000);
  pthread_join(pshared->threadserial, (void**)&pretval);
  if (*pretval)
    printf("Something in the Serial thread failed %d\n", *pretval);
}

