#include "stdio.h"

#include "../env.h"
#include "../main.h"
#include "../AD7280A.h"

// This generates the mV value tables related to temperatures

#define STATIC_RESISTOR_HOM 10000L

// Resistance in homs from -25°C to +120°C by steps of 5°C from the datasheet
// CTN thermistor reference at Farnell: 161263
//                             Vishay:  NTC0805E3103JMT
unsigned int ntclookup[] =
  {99489, 76385, 59157, 46194, 36356, 28829, 23025, 18515, 14986, 12205, 10000,
   8240, 6827, 5686, 4760, 4004, 3383, 2872, 2448, 2096, 1801, 1553, 1345, 1168,
   1019, 891, 782, 688, 607, 538};

int get_temp_V(int Rt)
{
  float Vt;

  Vt = (float)(AD7280_VREG_MV * STATIC_RESISTOR_HOM) / (float)(STATIC_RESISTOR_HOM + Rt);
  return (int)(Vt);
}

int main(void)
{
  int i, size;

  size = sizeof(ntclookup) / sizeof(int);
  //size = 160 / 5;
  printf("int array_size = %d;\n", size);
  printf("const unsigned int ntclookup[%d] PROGMEM =\n{", size);
  for (i = 0; i < size; i++)
    {
      printf("%d, ", get_temp_V(ntclookup[i]));
    }
  printf("};\n");
  return 0;
}

