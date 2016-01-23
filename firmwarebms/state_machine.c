
extern t_pack_variable_data g_appdata;
extern t_eeprom_data        g_edat;

// Checks that every voltage is under a value, like 15% under the max value
char all_under_threshold()
{
  unsigned long threshold;
  int           i, ret;

  ret = 1;
  threshold  = g_edat.bat_maxv;
  threshold -= (g_edat.bat_maxv * 10) / 100; // 10%
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      if (g_appdata.vbat[i] > threshold)
	{
	  ret = 0;
	}
    }
  return ret;
}

// Returns 1 if a battery element is above or equal the max vbat value
char battery_full()
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

// Returns 1 if a battery element is under the minimu voltage
char check_VBAT()
{
  int i, ret;

  ret = 0;
  for (i = 0; i < g_edat.bat_elements; i++)
    {
      if (g_appdata.vbat[i] <= g_edat.bat_minv)
	{
	  ret = 1;
	}
    }
  return ret;
}

// Removes the Amps from the state of charge value
void change_state_of_charge()
{
  unsigned long AH_value;

  // Amps are accumulated separatedly first because the value is too tiny
  g_appdata.c_discharge_accumulator += g_appdata.c_discharge;
  AH_value = g_appdata.c_discharge_accumulator / INTERVALS_PER_HOUR;
  // Reduce the state of charge
  if (AH_value > 0)
    {
      g_appdata.state_of_charge -= AH_value;
      g_discharge_accumul = g_discharge_accumul % INTERVALS_PER_HOUR;
    }
  // Average discharge current on the vehicle or on whatever uses the pack
  // (63 * avg + c) / 64;
  g_appdata.average_discharge = (g_appdata.average_discharge * 63 + g_appdata.c_discharge) >> 6;
}

void State_machine(t_eeprom_data *pedat)
{
  char chargeron;
  char buzer;
  char cutmains;
  char charger_off;
  char VBAT_low;
  char lederror;

  // Always read VBAT
  get_vbat(&g_appdata);
  // Always read the current
  get_ibat(&g_appdata);
  // Amways check if the charger is on
  chargeron = get_charger_on();
  // Check for undervoltage
  VBAT_low = check_VBAT();
  //
  switch (g_appdata.app_state)
    {
    case STATE_START:
      {
	sei();   // enable interrupts
	g_appdata.app_state = STATE_IDLE;
      }
      break;
    case STATE_CHARGEON:
      {
	if (chargeron == 0)
	  {
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_RELAPSE;
	  }
	else
	  {
	    if (all_under_threshold())
	      {
		g_appdata.charging_started = 1; // Used ot display a progress animation on the gauge
		g_appdata.app_state = STATE_CHARGING;
	      }
	  }
      }
      break;
    case STATE_CHARGING:
      {
	if (chargeron == 0)
	  {
	    // FIXME state of charge problem
	    g_appdata.idle_counter = 0;
	    g_appdata.app_state = STATE_RELAPSE;
	  }
	else
	  {
	    if (battery_full())
	      {
		g_appdata.charging_started = 0;
		g_appdata.average_discharge = 0;
		g_appdata.c_discharge = 0;
		g_appdata.c_discharge_accumulator = 0;
		g_appdata.state_of_charge = g_edat.full_charge;
		g_appdata.app_state = STATE_CHARGEON;
	      }
	  }
      }
      break;
    case STATE_IDLE:
      {
	if (chargeron != 0)
	  g_appdata.app_state = STATE_CHARGEON;
	else
	  {
	    if (VBAT_low)
	      g_appdata.app_state = STATE_SECURITY;
	    else
	      if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
		g_appdata.app_state = STATE_RUN;
	  }
      }
      break;
    case STATE_RUN:
      {
	change_state_of_charge();
	if (VBAT_low || chargeron)
	  {
	    g_appdata.app_state = STATE_SECURITY;
	  }
	else
	  {
	    if (g_appdata.c_discharge < DISCHARGE_THRESHOLD / 2)
	      {
		g_appdata.idle_counter = 0;
		g_appdata.app_state = STATE_RELAPSE;
	      }
	  }
      }
      break;
    case STATE_RELAPSE:
      {
	if (chargeron != 0)
	  {
	    g_appdata.app_state = STATE_CHARGEON;
	  }
	else
	  if (VBAT_low)
	    {
	      g_appdata.app_state = STATE_SECURITY;
	    }
	  else
	    {
	      if (g_appdata.c_discharge > DISCHARGE_THRESHOLD)
		g_appdata.app_state = STATE_RUN;
	      else
		{
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
	    }
      }
      break;
    case  STATE_SECURITY:
      {
	if (chargeron != 0)
	  {
	    g_appdata.app_state = STATE_CHARGEON;
	  }
	else
	  {
	    if (!VBAT_low)
	      {
		g_appdata.idle_counter = 0;
		g_appdata.app_state = STATE_RELAPSE;
	      }
	  }
      }
      break;
    case STATE_CRITICAL_FAILURE:
    default:
      {
      }
      break
    }
  //
  // Change the outputs depending on the state
  //
  buzer = (chargeron && (g_appdata.app_state == STATE_RUN)) || VBAT_low;
  cutmains = VBAT_low || (g_appdata.app_state == STATE_CHARGEON) || (g_appdata.app_state == STATE_CHARGING);
  // Stop the charger only if it is powered & not charging
  charger_off = (g_appdata.app_state != STATE_CHARGING) && (g_appdata.app_state == STATE_CHARGEON);
  lederror = VBAT_low || (g_appdata.app_state == STATE_CRITICAL_FAILURE);
  //
  // Output the signals
  //
  set_buzer(buzer);
  set_main_relay(cutmains);
  set_charger_disabled(charger_off);
  setled_error(lederror);
  // Outputs a signal for the gauge chip on one wire. 0 or VBAT, signal time is the value
  output_gauge(&g_appdata);
}


