
//#include <stdio.h>
//#include <unistd.h>

#include "vbat.h"

//long   g_charge = 1000 * BATCAP; // m Amp    OOOOVERFLOW
long   g_charge = 60000;

char   g_state  = STATE_TOO_LOW; // Charge to full on reset.
long   g_discharge_accumul = 0;
short  g_relax_counter;
long   g_relax_in_Vbat;
long   g_average;
short  g_blink_counter;
long   g_current = 0;

long get_relax_t(long charge)
{
  long relaxt;

  //                                             1
  //relaxt = ((charge - BATLIMIT) * (VBATRELAXHI - VBATRELAXLO)) / (BATCAP - BATLIMIT);
  relaxt = (charge - 1000 * BATLIMIT) / (BATCAP - BATLIMIT);
  //prlongf("relaxoffset=%d\n", relaxt);
  relaxt += 1000 * VBATRELAXLO;
  return relaxt;
}

#define USE_RELAX

long vbat_to_discharge_current(long charge, long vbat)
{
  long current;
  long relax_vbat;
  long diff;

  current = 0;
  switch (g_state)
    {
    case STATE_CHARGED:
    case STATE_CHARGING:
    case STATE_TOO_LOW:
      {
	current = 0;
      }
      break;
    case STATE_RELAX:
      {
	current = 0;
	relax_vbat = get_relax_t(charge);
	//prlongf("relaxt=%d vbat=%d\n", relax_vbat, vbat);
      }
      break;
    case STATE_INUSE:
      {
	relax_vbat = get_relax_t(charge);
	//prlongf("relaxt=%d vbat=%d\n", relax_vbat, vbat);
	if (vbat > relax_vbat)
	  return 0;
	diff = relax_vbat - vbat;
	if (diff <= (1000 * VBATRELAXHI - 1000 * LOWCV))
	  {
	    current = diff * LODISCHARGEC;
	  }
	else
	  {
	    current = diff * HIDISCHARGEC;
	  }
      }
      break;
    }
  return current;
}

void substract_charge(long vbat)
{
  long current;

  current = vbat_to_discharge_current(g_charge, vbat);
  g_current = current;
  g_discharge_accumul += current;
  if (g_discharge_accumul / INTERVAL_PER_HOUR > 0)
    {
      //prlongf("substracting %dmAh\n", g_discharge_accumul / LONGERVAL_PER_HOUR);
      g_charge -= g_discharge_accumul / INTERVAL_PER_HOUR;
      g_discharge_accumul = g_discharge_accumul % INTERVAL_PER_HOUR;
    }
}

void draw_full(char *arr)
{
  long i;

  for (i = 0; i < 10; i++)
    arr[i] = 1;
}

long comp_charge(long i, long g_charge)
{
  if (1000 * BATLIMIT + ((i * 1000 * (BATCAP - BATLIMIT)) / 10) < g_charge)
    {
      return g_blink_counter > 4? 0 : 1;
    }
  return 0;
}

void draw_charge(char *arr, long charge)
{
  long i;

  if (1000 * BATLIMIT + ((2 * 1000 * (BATCAP - BATLIMIT)) / 10) > g_charge)
    {
      g_blink_counter++;
      if (g_blink_counter > 7)
	g_blink_counter = 0;
    }
  else
    g_blink_counter = 0;
  for (i = 0; i < 10; i++)
    {
      arr[i] = comp_charge(i, g_charge);
    }
}

void draw_alarm(char *arr)
{
  long i;

  if (g_blink_counter > 3)
    g_blink_counter = 0;
  else
    g_blink_counter++;
  for (i = 0; i < 10; i++)
    arr[i] = 0;
  arr[0] = g_blink_counter > 1? 1 : 0;
}

void draw_charging(char *arr)
{
  long i;

  for (i = 0; i < 10; i++)
    arr[i] = i < g_blink_counter? 1 : 0;
  if (g_blink_counter > 9)
    g_blink_counter = 0;
  else
    g_blink_counter++;  
}

long myabs(long r)
{
  if (r < 0)
    return (-r);
  return r;
}

void fill_vbat_arr(char *arr, long vbat)
{
  long relax_vbat;

  substract_charge(vbat);
  switch (g_state)
    {
    case STATE_CHARGED:
      {
	draw_full(arr);
	if (vbat < 1000 * VBATOCVDOWN)
	  {
	    g_average = vbat;
	    g_relax_counter = 0;
	    g_state = STATE_INUSE;
	    g_relax_in_Vbat = vbat; // Prev vbat in fact
	  }
      }
      break;
    case STATE_INUSE:
      {
	draw_charge(arr, g_charge);
	g_average = (49 * g_average + vbat) / 50; // 10 seconds average
#ifdef USE_RELAX
	relax_vbat = get_relax_t(g_charge);
	
	if (myabs(g_average - vbat) <= RELAX_THRESHOLD ||
	    vbat > relax_vbat) // 0.15V
	  g_relax_counter++;
	else
	  g_relax_counter = 0;
	if (g_relax_counter > RELAX_EVENTS)
	  {
	    g_relax_in_Vbat = vbat;
	    g_state = STATE_RELAX;
	  }
#endif
	if (vbat < 1000 * VBATLOW ||
	    g_charge < 1000 * BATLIMIT)
	  {
	    g_state = STATE_TOO_LOW;
	  }
	if (vbat > 1000 * VBATOCVUP)
	  {
	    g_state = STATE_CHARGING;
	  }
      }
      break;
    case STATE_RELAX:
      {
	draw_charge(arr, g_charge);
	g_average = (49 * g_average + vbat) / 50; // 10 seconds average
	relax_vbat = get_relax_t(g_charge);
	if (g_average - vbat >= RELAX_THRESHOLD ||
	     myabs(g_relax_in_Vbat - vbat) >= RELAX_THRESHOLD ||
	    (relax_vbat - vbat) > 400)
	  {
	    g_relax_counter = 0;
	    g_state = STATE_INUSE;
	  }
	if (vbat > 1000 * VBATOCVUP)
	  {
	    g_state = STATE_CHARGING;
	  }
	if (vbat < 1000 * VBATLOW ||
	    g_charge < 1000 * BATLIMIT)
	  {
	    g_state = STATE_TOO_LOW;
	  }
      }
      break;
    case STATE_TOO_LOW:
      {
	draw_alarm(arr);
	if (vbat > 1000 * VBATOCVUP)
	  {
	    g_state = STATE_CHARGING;
	  }
	g_average = 0;
      }
      break;
    case STATE_CHARGING:
      {
	draw_charging(arr);
	if (vbat > 1000 * VBATENDCHARGE)
	  {
	    g_state = STATE_CHARGED;
	    g_charge = 1000 * BATCAP;
	  }
	draw_charging(arr);
	if (vbat < 1000 * VBATRELAXHI)
	  {
	    g_state = STATE_INUSE;
	  }
      }
      break;
    }
}

