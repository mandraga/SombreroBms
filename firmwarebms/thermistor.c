#include <avr/pgmspace.h>

#include "env.h"
#include "main.h"
#include "AD7280A.h"
#include "thermistor.h"

const unsigned int ntclookup[30] PROGMEM =
  {474, 601, 751, 925, 1121, 1339, 1574, 1823, 2081, 2341, 2600, 
   2850, 3090, 3315, 3523, 3713, 3885, 4039, 4177, 4298, 4406,
   4500, 4583, 4656, 4719, 4774, 4822, 4865, 4902, 4934};

unsigned long get_temp_V(int temp)
{
  unsigned long Vt, nextVt;
  int           mod, div;

  temp = temp - TSTART;
  if (temp >= TSTOP - TSTART)
    return (unsigned long)ntclookup[29]; // Something like 120°
  if (temp < 0)
    return (unsigned long)ntclookup[0]; // -25°C
  //
  div = (temp / 5);
  mod = (temp % 5);
  if (mod == 0)
    {
      return (unsigned long)ntclookup[div];
    }
  // Overflow check
  if (div > 28)
    {
      return (unsigned long)ntclookup[29]; // Something like 120°
    }
  // Approximation
  Vt     = (unsigned long)ntclookup[div];
  nextVt = (unsigned long)ntclookup[div + 1];
  // next value is always highter and 5°C above, this aproximates between the two
  Vt = Vt + (((unsigned long)mod * (nextVt - Vt)) / 5L);
  return Vt;
}

// We have Vref 5200mV, then CTN thermistor, AUX measurement point, 10Khom resistor, VSS
int mv_to_temperature(unsigned long V)
{
  int i, intV;

  intV = V;
  // Find the 5V range
  for (i = TSTART; i < TSTOP; i += 5)
    {
      if (get_temp_V(i) >= intV)
	break;
    }
  // Find the aproximated value, from the last highter value
  for (i = i - 5; i < TSTOP; i++)
    {
      if (get_temp_V(i) >= intV)
	return (i);
    }
  return 1000;
}

