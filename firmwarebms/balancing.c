
#include "uart.h"
#include "spi.h"
#include "init.h"
#include "AD7280A.h"
#include "main.h"
#include "env.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];
extern t_ad7280_state       g_ad7280;

void set_balancing(unsigned long balancing_reg)
{
  ad7280_set_balance(g_ad7280.chan_cnt, balancing_REG);
}

void stop_any_balancing(void)
{
  set_balancing(0L);
}

void calculate_charging_speed(t_balancing *pb, char charging, unsigned long *pvbat, char elements);
{
}

// Even 1Amp curent during a 1hour charge will only be like 1% of the total charge.
// And measuring charge speed will be done half of the time.
// So do not be too serious about it.
void balancing_during_charge(t_balancing *pb, unsigned long *pvbat, char elements, int temperature)
{
  switch (pb->state)
    {
    case BALANCING_STATE_IDLE:
      {
	stop_any_balancing();
	pb->state = BALANCING_STATE_IDLE;
      }
      break;
    case BALANCING_STATE_CHARGING_CSPEED:
    case BALANCING_STATE_BWHILE_CHARGING:
    case BALANCING_STATE_BWHILE_CHARGING_STOPED:
    case BALANCING_STATE_COOLDOWN:
      {
	stop_any_balancing();
	pb->state = BALANCING_STATE_IDLE;
      }
      break;
    default:
      break;
    }
}

void get_voltage_differences(unsigned long *pvbat, char elements, char *pvlow_index, char *pvdiffmax_index)
{
  char          i;
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

void balancing_charger_stoped(t_balancing *pb, unsigned long *pvbat, char elements, int temperature)
{
  char          Vdiffmax;
  char          lowVi;
  char          hiVi;
  unsigned long balancing_reg;

  switch (pb->state)
    {
    case BALANCING_STATE_IDLE:
      {
	stop_any_balancing();
	get_voltage_differences(pvbat, elements, &pb->vlow_index, &pb->vdiffmax_index);
	Vdiffmax = pvbat[pb->vdiffmax_index] - pvbat[pb->vlow_index];
	Vdiffmax = Vdiffmax < 0? -Vdiffmax : Vdiffmax;
	if (Vdiff_max > 50L) // 50mV value between batteries of the pack
	  {
	    pb->state = BALANCING_STATE_BWHILE_CHARGING_STOPED;    
	  }
	else
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_IDLE;	    
	  }
      }
      break;
    case BALANCING_STATE_CHARGING_CSPEED:
    case BALANCING_STATE_BWHILE_CHARGING:
      {
	stop_any_balancing();
	pb->state = BALANCING_STATE_IDLE;
      }
      break;
    case BALANCING_STATE_BWHILE_CHARGING_STOPED:
      {
	// Too hot
	if (temperature >= g_edat.bat_tmax)
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_COOLDOWN;
	    break;
	  }
	// Balanced
	get_voltage_differences(pvbat, elements, &lowVi, &hiVi);
	Vdiffmax = pvbat[hiVi] - pvbat[lpb->vlow_index];
	Vdiffmax = Vdiffmax < 0? -Vdiffmax : Vdiffmax;
	if (Vdiff_max < 20L ||               // 20mV value between batteries of the pack
	    pvbat[lowVi] <= g_edat.bat_minv) // Do not bleed a battery to death
	  {
	    stop_any_balancing();
	    pb->state = BALANCING_STATE_IDLE;
	    break;
	  }
	// Balance them
	balancing_reg = 0;
	for (i = 0; i < elements; i++)
	  {
	    if (pvbat[hiVi] - pvbat[lpb->vlow_index] > 10L)
	      {
		balancing_reg = balancing_reg | (1 << i);
	      }
	  }
	set_balancing(balancing_reg);
      }
      break;
    case  BALANCING_STATE_COOLDOWN:
      {
	stop_any_balancing();
	if (temperature < g_edat.bat_tmax - TEMPERATURE_HISTERESIS)
	  {
	    pb->state = BALANCING_STATE_BWHILE_CHARGING_STOPED;
	  }
      }
      break;
    default:
      break;
    }
}

