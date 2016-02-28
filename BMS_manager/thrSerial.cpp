#include <unistd.h>
#include <iterator>
#include <list>
#include <vector>
#include <string>
#include <termios.h>

#include <pthread.h>

#include <sys/time.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//
#include "shared.h"

int g_thrSerialretval;

void* thr_serial(void *p_data)
{
  t_shared_data   *pshared = (t_shared_data*)p_data;
  bool             bquit;
  bool             brcv;
  char             byte;
  t_serial_request rq;
  char             message[MAX_MSG_SIZE];
  int              sz, ctn;
  //
  enum eserial_state
    {
      ser_nothing,
      ser_waiting_response
    };
  eserial_state ser_state;

  g_thrSerialretval = 0;
  ser_state = ser_nothing;
  printf("Initialising the serial thread\n");
  LOCK;
  bquit = pshared->bquit;
  UNLOCK;
  while (!bquit)
    {
      // Read a command
      LOCK;
      if (pshared->pserial)
	{
	  while (pshared->request.size())
	    {
	      rq = pshared->request.pop_front();
	      UNLOCK;
	      switch (rq.type)
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
		    snprintf(message, MAX_MSG_SIZE, "set_param %s\n", rq.command);
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
			usleep(10000);
		    }
		  if (byte == 0  && sz > 0)
		    {
		      LOCK;
		      // Process the shit, like if it was a sewer pipe.
		      switch (rq.type)
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
			    pshared->pBMS->parse_BMS_params_string(pshared->pBMS->m_params, pshared->param_msg);
			  }
			  break;
			case ereport:
			  {
			    strncpy(pshared->report_msg, message, MAX_MSG_SIZE);
			    pshared->pBMS->parse_BMS_report_string(pshared->pBMS->m_report, pshared->report_msg);
			  }
			  break;
			case eprogram:
			case eupdategui:
			default:
			  break;
			}
		      UNLOCK;
		    }
		}
	    }
      bquit = pshared->bquit;
      UNLOCK;
      usleep(1000);
    }
  printf("Serial thread closing\n");
  return &g_thrSerialretval;
}

void start_serial_thread(t_app_data *papp_data)
{
  int ret;

  ret = pthread_create(&papp_data->shared->threadserial, NULL,
		       thr_serial, (void*)&papp_data->shared);
  if (ret)
    printf("Serial thread creation error.");
}

void stop_serial_thread(t_app_data *papp_data)
{
  int *pretval;

  LOCK;
  papp_data->shared.bquit = true;
  UNLOCK;
  pthread_join(papp_data->shared->threadserial, (void**)&pretval);
  if (*pretval)
    printf("Something in the Serial thread failed %d\n", *pretval);
}

