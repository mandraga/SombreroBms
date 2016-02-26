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

#include "gfxareas.h"
#include "keypress.h"

#include "../serial.h"
#include "../env.h"
#include "../main.h"
#include "../AD7280A.h"
#include "../inout.h"
#include "../eeprom.h"
#include "../state_machine.h"
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
#include "serialport.h"
#include "fake_uart.h"
//
#include "app.h"
//#include "sdlcalls.h"

extern t_serialport g_serial;

int g_thrSerialretval;

Cappdata *g_papp;

// Do not send the byte, just send everything
void send_first_byte(char byte)
{
  while (g_serial.TXstate != SER_STATE_IDLE && g_serial.outsize != 0)
    {
      g_papp->m_fake_uart.send(g_serial.outbuffer, g_serial.outsize);
      g_serial.TXstate = change_TX_state(g_serial.TXstate);
    }
}

void* thr_serial(void *p_data)
{
  Cappdata       *papp = (Cappdata*)p_data;
  bool            bquit;
  bool            brcv;
  char            byte;

  g_papp = papp;
  g_thrSerialretval = 0;
  printf("Initialising the serial thread\n");
  LOCK;
  bquit = papp->m_bquit;
  UNLOCK;
  while (!bquit)
    {
      // Read a command
      LOCK;
      while ((brcv = papp->m_fake_uart.get_next_byte(&byte)))
	{
	  g_serial.RXstate = SER_STATE_RECEIVE;
	  g_serial.inbuffer[g_serial.inindex++] = byte;
	  if (byte == '\n' ||
	      g_serial.inindex >= g_serial.insize ||
	      g_serial.inindex >= RCVSTRINGSZ)
	    {
	      g_serial.RXstate = SER_STATE_IDLE;
	      if (byte == '\n' && g_serial.inindex < RCVSTRINGSZ) // Otherwise there is an error somewhere
		{
		  g_serial.inbuffer[g_serial.inindex] = 0; // Add an end to form a string
		  // Answer in this through "send_firs_byte"
		  process_serial_command();
		}
	      g_serial.RXstate = SER_STATE_IDLE;
	    }
	}
      bquit = papp->m_bquit;
      UNLOCK;
      usleep(1000);
    }
  printf("Serial thread closing\n");
  return &g_thrSerialretval;
}

void start_serial_thread(Cappdata *app)
{
  int ret;

  ret = pthread_create(&app->m_threadserial, NULL,
		       thr_serial, (void*)&app);
  if (ret)
    printf("Serial thread creation error.");
}

void stop_serial_thread(Cappdata *papp)
{
  int *pretval;

  LOCK;
  papp->m_bquit = true;
  UNLOCK;
  pthread_join(papp->m_threadserial, (void**)&pretval);
  if (*pretval)
    printf("Something in the Serial thread failed %d\n", *pretval);
}

