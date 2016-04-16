
//#include <stdio.h>
//#include <unistd.h>

#include "display.h"

char   g_state  = STATE_TOO_LOW; // Charge to full on reset.
short  g_blink_counter;

void draw_lobat(char *arr)
{
  int i;

  if (g_blink_counter > 3)
    g_blink_counter = 0;
  else
    g_blink_counter++;
  for (i = 0; i < 10; i++)
    arr[i] = 0;
  arr[0] = g_blink_counter > 1? 1 : 0;
}

void draw_alarm(char *arr)
{
  int  i;
  char leds;
  
  if (g_blink_counter > 4)
    g_blink_counter = 0;
  else
    g_blink_counter++;
  leds = g_blink_counter > 2? 1 : 0;
  for (i = 0; i < 10; i++)
    arr[i] = leds;
}

void draw_charging(char *arr)
{
  int i;

  if (g_blink_counter > 9)
    g_blink_counter = 0;
  else
    g_blink_counter++;  
  for (i = 0; i < 10; i++)
    arr[i] = i < g_blink_counter? 1 : 0;
}

void draw_failure(char *arr)
{
  int i;

  if (g_blink_counter > 2 * 9)
    g_blink_counter = 0;
  else
    g_blink_counter++;  
  if (i < 9)
    {
      for (i = 0; i < 10; i++)
	arr[i] = i < g_blink_counter? 1 : 0;
    }
  else
    {
      for (i = 0; i < 10; i++)
	arr[9 - i] = 9 + i > g_blink_counter? 1 : 0;      
    }
}

void fill_bars(char *arr, int bars)
{
  int i;

  for (i = 0; i < 10; i++)
    {
      arr[i] = (i < bars - STATE_EMPTY_CHARGE)? 1 : 0; // [1, 10]
    }
}

void fill_vbat_arr(char *arr, int state)
{
  switch (g_state)
    {
    case STATE_TOO_LOW:
    case STATE_EMPTY_CHARGE:
      {
	draw_lobat(arr);
      }
      break;
    case STATE_1_CHARGE:
    case STATE_2_CHARGE:
    case STATE_3_CHARGE:
    case STATE_4_CHARGE:
    case STATE_5_CHARGE:
    case STATE_6_CHARGE:
    case STATE_7_CHARGE:
    case STATE_8_CHARGE:
    case STATE_9_CHARGE:
    case STATE_10_CHARGE:
      {
	fill_bars(arr, g_state);
      }
      break;
    case STATE_SECURITY:
      {
	draw_alarm(arr);
      }
      break;
    case STATE_CHARGING:
      {
	draw_charging(arr);
      }
      break;
    case STATE_FAILURE:
    default:
      {
	draw_failure(arr);
      }
      break;
    }
}
