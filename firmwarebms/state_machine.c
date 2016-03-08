//#include "stdio.h"

#include "env.h"
#include "main.h"
#include "spi.h"
#include "init.h"
#include "eeprom.h"
#include "AD7280A.h"
#include "balancing.h"
#include "serial.h"
//#include "adc.h"
#include "inout.h"
#include "state_machine.h"

extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;
extern t_eeprom_battery     g_bat[MAXBATTERY];
extern t_ad7280_state       g_ad7280;
extern t_balancing          g_balancing;

// Checks that every voltage is over a value, like 10% under the max value
char all_under_max_Vbat_threshold(void)
{
  unsigned long threshold;
  int           i, ret;

  ret = 1;
  threshold  = g_edat.bat_maxv;
  threshold -= (g_edat.bat_maxv - g_edat.bat_minv) >> 3; // v/8 less than 10%
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      if (g_appdata.vbat[i] >= threshold)
	{
	  ret = 0;
	}
    }
  return ret;
}

// Returns 1 if a battery element is above or equal the max vbat value
char battery_element_overcharge(void)
{
  int i, ret;

  ret = 0;
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      if (g_appdata.vbat[i] >= g_edat.bat_maxv)
	{
	  ret = 1;
	}
    }
  return ret;
}

// Returns 1 if a battery element is under the minimum voltage
char check_VBAT(void)
{
  int           i, ret;
  unsigned long sum;

  ret = 0;
  sum = 0;
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      sum += g_appdata.vbat[i];
      if (g_appdata.vbat[i] <= g_edat.bat_minv)
	{
	  ret = 1;
	}
    }
  // FIXME cli() sei()
  g_appdata.total_vbat = sum;
  return ret;
}

// Removes the Amps from the state of charge value
void change_state_of_charge(void)
{
  unsigned long AH_value;

  // Amps are accumulated separatedly first because the value is too tiny
  g_appdata.c_discharge_accumulator += g_appdata.c_discharge;
  AH_value = g_appdata.c_discharge_accumulator / INTERVALS_PER_HOUR;
  // Reduce the state of charge
  if (AH_value > 0 && g_appdata.state_of_charge > 0)
    {
      if (g_appdata.state_of_charge > AH_value)
	{
	  g_appdata.c_discharge_accumulator = g_appdata.c_discharge_accumulator % INTERVALS_PER_HOUR;
	  g_appdata.state_of_charge -= AH_value;
	}
      else
	g_appdata.state_of_charge = 0;
    }
}

// Check everything else, and store min and max and events in the eeprom
void check_everything_else(void)
{
  //---------------------------------------
  // App data
  //---------------------------------------
  g_appdata.tseconds++;              // Tenth of seconds or 100ms
  if (g_appdata.tseconds >= 600)
    {
      g_appdata.tseconds = 0;
      g_appdata.uptime_minutes++;
      if (g_appdata.uptime_minutes >= 60 * 24)
	{
	  g_appdata.uptime_minutes = 0;
	  g_appdata.uptime_days++;
	}
    }
  //---------------------------------------
  // EEprom data
  //---------------------------------------
  update_temperature_extremes_EEPROM(g_appdata.temperature);
}

void update_local_charging_time(void)
{
  g_appdata.charge_time_count_tenth++;
  if (g_appdata.charge_time_count_tenth >= 600)
    {
      g_appdata.charge_time_count_tenth = 0;
      g_appdata.charge_time_count++;
    }
}

void State_machine()
{
  char chargeron;
  char buzer;
  char cutmains;
  char charger_off;
  char VBAT_low;
  char lederror;

  // Always read VBAT
  get_VBAT(&g_ad7280, &g_appdata);
  // Always read the current
  get_IBAT(&g_appdata);
  // Amways check if the charger is on
  chargeron = get_charger_ON();
  // Check for undervoltage
  VBAT_low = check_VBAT();
  //
  check_everything_else();
  // If a battery went deeper than previously, then store the value
  update_battery_low_values_EEPROM();
  //
  switch (g_appdata.app_state)
    {
    case STATE_START:
      {
	g_appdata.app_state = STATE_IDLE;
      }
      break;
    case STATE_CHARGEON:
      {
	char balanced;

	balanced = balancing_charger_stoped(&g_balancing, g_appdata.vbat, g_edat.bat_elements,
					    g_appdata.temperature);
	// First check if the car runs, stop everything then because the device is pluged
	if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
	  {
	    stop_any_balancing();
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_CURRENT_SECURITY;
	    break;
	  }
	if (chargeron == 0)
	  {
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_RELAX;
	    break;
	  }
	// If a battery is low, go to CHARGING
	// And no battery is full
	if ((balanced &&
	     all_under_max_Vbat_threshold() &&
	     g_appdata.total_vbat < g_edat.full_chargeV) || VBAT_low)
	  {
	    g_appdata.charging_started = 1; // Used ot display a progress animation on the gauge
	    if (g_appdata.prev_idle)
	      {
		inc_charge_cylces_EEPROM(); // Charge cycles ++
		g_appdata.prev_idle = 0;
	      }
	    g_appdata.charge_time_count_tenth = g_appdata.charge_time_count = 0;
	    g_appdata.app_state = STATE_CHARGING;
	    break;
	  }
      }
      break;
    case STATE_CHARGING:
      {
	update_local_charging_time();
	// Does nothing but goes back to no balancing
	balancing_during_charge(&g_balancing, g_appdata.vbat, g_edat.bat_elements,
				g_appdata.temperature);
	// First check if the car runs, stop everything then because the device is pluged
	if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
	  {
	    g_appdata.idle_counter = 0;
	    stop_any_balancing();
	    g_appdata.app_state = STATE_CURRENT_SECURITY;
	    break;
	  }
	// The charger is unpluged during charge
	if (chargeron == 0)
	  {
	    // FIXME state of charge does not change here
	    update_battery_charge_values_EEPROM(); // If the charger is unpluged here, store the charging time
	    update_charge_time_minutes_EEPROM(g_appdata.charge_time_count);
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_CHARGEON;
	    break;
	  }
	// Stop charging and balance, the battery is not full yet, but an element is overcharged
	if (battery_element_overcharge())
	  {
	    g_appdata.app_state = STATE_CHARGEON;
	  }
	// The battery is full and probably balanced
	if (g_appdata.total_vbat > g_edat.full_chargeV)
	  {
	    update_battery_charge_values_EEPROM(); // When a battey is at Vmax, store the charging time
	    update_charge_time_minutes_EEPROM(g_appdata.charge_time_count);
	    g_appdata.charging_started = 0;
	    g_appdata.c_discharge = 0;
	    g_appdata.c_discharge_accumulator = 0;
	    g_appdata.state_of_charge = g_edat.full_charge; // Set the SOC to the full value
	    g_appdata.app_state = STATE_CHARGEON;
	    break;
	  }
      }
      break;
    case STATE_IDLE:
      {
	if (chargeron)
	  {
	    g_appdata.prev_idle = 1;
	    g_appdata.app_state = STATE_CHARGEON;
	  }
	else
	  {
	    // A battery is under the minimal value
	    if (VBAT_low)
	      {
		update_battery_low_events_EEPROM();
		g_appdata.app_state = STATE_SECURITY;
	      }
	    else
	      if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
		g_appdata.app_state = STATE_RUN;
	  }
      }
      break;
    case STATE_RUN:
      {
	change_state_of_charge(); // SOC update
	//
	if (chargeron)
	  {
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_CURRENT_SECURITY;
	    break;
	  }
	if (VBAT_low)
	  {
	    update_battery_low_events_EEPROM();
	    g_appdata.app_state = STATE_SECURITY;
	    break;
	  }
	if (g_appdata.c_discharge < DISCHARGE_THRESHOLD / 2)
	  {
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_RELAX;
	  }
      }
      break;
    case STATE_RELAX:
      {
	if (chargeron != 0)
	  {
	    g_appdata.prev_idle = 1; // More or less, but count a new charge cycle
	    g_appdata.app_state = STATE_CHARGEON;
	    break;
	  }
	if (VBAT_low)
	  {
	    update_battery_low_events_EEPROM();
	    g_appdata.app_state = STATE_SECURITY;
	    break;
	  }
	if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
	  {
	    g_appdata.app_state = STATE_RUN;
	    break;
	  }
	// After being used the lithium cells voltage tends to go
	// up for 20 seconds.
	// To know when it is finished, the VBAT change rate must be
	// calculated and compared to a threshold. When it is low like
	// 1mV/4seconds then go to IDLE.
	// Or wait 20 seconds and go to IDLE, t's simpler.
	g_appdata.idle_counter++;
	if (g_appdata.idle_counter > 20 * SAMPLING_PER_SECOND)
	  {
	    g_appdata.app_state = STATE_IDLE;
	  }
      }
      break;
    case  STATE_SECURITY:
      {
	if (chargeron != 0)
	  {
	    g_appdata.app_state = STATE_CHARGEON;
	    break;
	  }
	if (!VBAT_low)
	  {
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_RELAX;
	    break;
	  }
      }
      break;
    case  STATE_CURRENT_SECURITY:
      {
	// The pack is disconnected because of current flow where it is not suposed
	// therefore, no current is flowing. Therefore wait.
	g_appdata.idle_counter++;
	// 10 seconds to figure out whats happening
	if (g_appdata.idle_counter > 10 * SAMPLING_PER_SECOND)
	  {
	    g_appdata.app_state = STATE_RELAX;
	  }
      }
      break;      
    case STATE_CRITICAL_FAILURE:
    default:
      {
      }
      break;
    }
  //
  // Change the outputs depending on the state
  //
  buzer = (chargeron && (g_appdata.app_state == STATE_RUN)) || VBAT_low;
  cutmains = VBAT_low || g_appdata.app_state == STATE_CURRENT_SECURITY;
  // Stop the charger only if it is powered & not charging
  charger_off = (g_appdata.app_state != STATE_CHARGING) && chargeron;
  lederror = VBAT_low || (g_appdata.app_state == STATE_CRITICAL_FAILURE)
                      || (g_appdata.app_state == STATE_SECURITY)
                      || (g_appdata.app_state == STATE_CURRENT_SECURITY);
  //
  // Output the signals
  //
  set_buzer(buzer);
  set_main_relay(cutmains);
  set_charger_disabled(charger_off);
  setled_error(lederror);
  // Outputs a signal for the gauge chip on one wire. 0 or VBAT, signal time is the value
  set_gauge_out(g_appdata.state_of_charge, g_edat.full_charge,
		VBAT_low, (g_appdata.app_state == STATE_CHARGING),
		(g_appdata.app_state == STATE_CURRENT_SECURITY) || (g_appdata.app_state == STATE_CRITICAL_FAILURE));
}

