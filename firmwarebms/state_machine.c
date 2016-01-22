
extern t_pack_variable_data g_appdata;

void State_machine(t_vbat *pvbat)
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
  get_iba(&g_appdata);
  // Amways check if the cherger is on
  chargeron = get_charger_on();
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
      }
      break;
    case STATE_CHARGING:
      {
      }
      break;
    case STATE_IDLE:
      {
      }
      break;
    case STATE_RUN:
      {
      }
      break;
    case STATE_RELAPSE:
      {
      }
      break;
    case  STATE_SECURITY:
      {
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

