
#include "env.h"
#include "main.h"
#include "spi.h"
#include "init.h"
#include "AD7280A.h"
#include "balancing.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];
extern t_ad7280_state       g_ad7280;
extern t_balancing          g_balancing;

void set_balancing(unsigned long balancing_reg)
{
  ad7280_set_balance(&g_ad7280, balancing_reg);
}

void stop_any_balancing(void)
{
  set_balancing(0L);
}

// Even 1Amp curent during a 1hour charge will only be like 1% of the total charge.
// And measuring charge speed will be done half of the time.
// So do not be too serious about it.
void balancing_during_charge(t_balancing *pb, unsigned long *pvbat, char elements, int *ptemperature)
{
  switch (pb->state)
    {
    case BALANCING_STATE_IDLE:
      {
	stop_any_balancing();
      }
      break;
    case BALANCING_STATE_CHARGING_CSPEED:
    case BALANCING_STATE_BWHILE_CHARGING:
    case BALANCING_STATE_BWHILE_CHARGING_STOPED:
    case BALANCING_STATE_COOLDOWN:
    default:
      {
	stop_any_balancing();
	pb->state = BALANCING_STATE_IDLE;
      }
      break;
    }
  setled_balancing(0);
}

void get_voltage_differences(unsigned long *pvbat, char elements, int *pvlow_index, int *pvdiffmax_index)
{
  int           i;
  unsigned long minV, maxV;

  minV = 100000L;
  maxV = 0;
  *pvdiffmax_index = *pvlow_index = 0;
  for (i = 0; i < elements; i++)
    {
      if (pvbat[i] < minV)
	{
	  minV = pvbat[i];
	  *pvlow_index = i;
	}
      if (pvbat[i] > maxV)
	{
	  maxV = pvbat[i];
	  *pvdiffmax_index = i;
	}      
    }
}

int get_highter_temperature(int *ptemperature, char modules)
{
  int  i;
  int  max;

  max = -1000;
  for (i = 0; i < modules; i++)
    {
      if (ptemperature[i] > max)
	{
	  max = ptemperature[i];
	}
    }
  //get_uc_internal_temperature()
  return max;
}

// If t° is way below max temperature, then shunt any battery above the
// lower one.
// As t° closes to the max temperature, only shunt the batteries with higher voltage.
//
void balancing_with_temperature_control(int vlow_index, unsigned long *pvbat,
					int higher_temperature, int max_temperature,
					char elements)
{
  unsigned long balancing_reg;
  int           sorted[MAXBATTERY];
  int           r;
  int           i, j;
  unsigned long balanced;

  // Sort the values, higher values first
  for (i = 0; i < elements; i++)
    sorted[i] = i;
  for (i = elements - 1; i > 0 ; i--)
    for (j = i - 1; j >= 0; j--)
      {
	if (pvbat[sorted[i]] < pvbat[sorted[j]])
	  {
	    r = sorted[i];
	    sorted[i] = sorted[j];
	    sorted[j] = r;
	  }
      }
  // Balance only the n bigger Vbat values depending on the temperature
  balanced = elements;
  if (max_temperature - higher_temperature < TEMPERATURE_HISTERESIS)
    {
      // B = temperature_diffrence * (elements / tempehisteresis)
      balanced = (unsigned long)(max_temperature - higher_temperature);
      balanced = balanced < 0? -balanced : balanced;
      balanced = balanced * (unsigned long)elements;
      balanced = balanced / TEMPERATURE_HISTERESIS;
    }
  balanced = balanced > elements? elements : balanced; // to be sure
  balancing_reg = 0;
  for (i = 0; i < balanced; i++)
    {
      if (sorted[i] != vlow_index)
	if (pvbat[sorted[i]] - pvbat[vlow_index] > 10L)
	  {
	    balancing_reg = balancing_reg | (1 << sorted[i]);
	  }
    }
  set_balancing(balancing_reg);
}

// Only called when the charger has finished charging, or is not charging
void balancing_charger_stoped(t_balancing *pb, unsigned long *pvbat, char elements, int *ptemperature)
{
  int Vdiffmax;
  int lowVi;
  int hiVi;
  int highter_temperature;

  switch (pb->state)
    {
    case BALANCING_STATE_IDLE:
      {
	stop_any_balancing();
	//
	// Check if the batteries are unbalanced
	//
	get_voltage_differences(pvbat, elements, &pb->vlow_index, &pb->vdiffmax_index);
	Vdiffmax = pvbat[pb->vdiffmax_index] - pvbat[pb->vlow_index];
	Vdiffmax = Vdiffmax < 0? -Vdiffmax : Vdiffmax;
	if (Vdiffmax > 50L) // 50mV value between batteries of the pack
	  {
	    setled_balancing(1);
	    pb->state = BALANCING_STATE_BWHILE_CHARGING_STOPED;
	  }
      }
      break;
    case BALANCING_STATE_CHARGING_CSPEED:
    case BALANCING_STATE_BWHILE_CHARGING:
      {
	// These states are not for this functions
	stop_any_balancing();
	pb->state = BALANCING_STATE_IDLE;
      }
      break;
    case BALANCING_STATE_BWHILE_CHARGING_STOPED:
      {
	// Too hot?
	highter_temperature = get_highter_temperature(ptemperature, CFGAD728AMODULES);
	if (highter_temperature >= g_edat.bat_tmax)
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_COOLDOWN;
	    break;
	  }
	// Balanced?
	get_voltage_differences(pvbat, elements, &lowVi, &hiVi);
	Vdiffmax = pvbat[hiVi] - pvbat[pb->vlow_index];
	Vdiffmax = Vdiffmax < 0? -Vdiffmax : Vdiffmax;
	if (Vdiffmax < 20L ||                // 20mV value between batteries of the pack
	    pvbat[lowVi] <= g_edat.bat_minv) // Do not bleed a battery to death
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_IDLE;
	    break;
	  }
	// If the lowest battery is not the one setup when entering the state
	// then return to IDLE. 4mV tolerance
	if (pvbat[pb->vlow_index] > pvbat[lowVi] + 4)
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_IDLE;
	    break;
	  }
	// Balance from higher to lower battery taking temperature into account
	// The temperature sensors must be reliable.
	balancing_with_temperature_control(pb->vlow_index, pvbat,
					   highter_temperature, g_edat.bat_tmax,
					   elements);
      }
      break;
    case  BALANCING_STATE_COOLDOWN:
      {
	stop_any_balancing();
	highter_temperature = get_highter_temperature(ptemperature, CFGAD728AMODULES);
	if (highter_temperature < g_edat.bat_tmax - TEMPERATURE_HISTERESIS)
	  {
	    pb->state = BALANCING_STATE_BWHILE_CHARGING_STOPED;
	  }
      }
      break;
    default:
      break;
    }
  setled_balancing(pb->state == BALANCING_STATE_BWHILE_CHARGING_STOPED);
}

