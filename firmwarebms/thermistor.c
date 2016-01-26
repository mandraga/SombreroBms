
#include "AD7280A.h"
#include "thermistor.h"

// Resistance in homs from -25°C to +120°C by steps of 5°C
flash unsigned int ntclookup[160 / 5] =
  {99489, 76385, 59157, 46194, 36356, 28829, 23025, 18515, 14986, 12205, 10000,
   8240, 6827, 5686, 4760, 4004, 3383, 2872, 2448, 2096, 1801, 1553, 1345, 1168,
   1019, 891, 782, 688, 607, 538};

unsigned long get_temp_V(int temp)
{
  unsigned long Vt;
  unsigned long Rt;
  unsigned long nextRt;
  int           mod, div;

  temp = temp - TSTART;
  if (temp >= TSTOP - TSTART)
    {
      Rt = ntclookup[140 / 5]; // Something like 115°
    }
  else
    {
      if (temp < 0)
	Rt = ntclookup[0]; // -25°C
      else
	{
	  div = (temp / 5);
	  mod = (temp % 5);
	  if (mod == 0)
	    {
	      Rt = ntclookup[div];
	    }
	  else
	    {
	      // Approximation
	      Rt     = (unsigned long)ntclookup[div];
	      nextRt = (unsigned long)ntclookup[div + 1];
	      // next value is always lower and 5°C above, this aprowimate between the two
	      Rt = Rt - ((mod * (Rt - nextRt)) / 5L);
	    }
	}
    }
  Vt = AD7280_VREG_MV * STATIC_RESISTOR_HOM / (STATIC_RESISTOR_HOM + Rt);
}

// We have Vref 5200mV, then CTN thermistor, AUX measurement point, 10Khom resistor, VSS
int mv_to_temperature(unsigned long V)
{
  int i, intV;
  int last;

  intV = V;
  // Find the 5V range
  for (i = TSTART; i < TSTOP; i += 5)
    {
      if (get_temp_V(i) <= intV)
	break;
    }
  // Find the aproximated value, from the last highter value
  for (i = i - 5; i < TSTOP; i++)
    {
      if (get_temp_V(i) <= intV)
	return (i);
    }
  return 1000;
}

