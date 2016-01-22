
void read_cfg_from_EEPROM(void)
{
  unsigned char c;
  int           alarm_number;
  
  // Read the first byte,
  // if it is 0xFF then the EEPROM is unprogramed: set to default values and return
  // else read the data
  c = eeprom_read_byte((uint8_t*)0x00);
  if (c == 0xFF)
    {
      memset(&g_c, 0, sizeof(g_c));
      for (alarm_number = 0; alarm_number < 4; alarm_number++)
	{
#if 0
	  g_c.alrm[alarm_number].begin.h = 21;
	  g_c.alrm[alarm_number].end.h = 9;
#else
	  g_c.alrm[alarm_number].begin.h = 0xFF;
#endif
	}
      // 0033609001390    Leclerc Mobile sms center
      g_c.sms_center[0] = 0;
      g_c.sms_center[1] = 0;
      g_c.sms_center[2] = 3;
      g_c.sms_center[3] = 3;
      g_c.sms_center[4] = 6;
      g_c.sms_center[5] = 0;
      g_c.sms_center[6] = 9;
      g_c.sms_center[7] = 0;
      g_c.sms_center[8] = 0;
      g_c.sms_center[9] = 1;
      g_c.sms_center[10] = 3;
      g_c.sms_center[11] = 9;
      g_c.sms_center[12] = 0;
      return ;
    }
  // Read a block @ 0x01
  eeprom_read_block(&g_c, (uint8_t*)0x01, sizeof(g_c));
}

void write_cfg_to_EEPROM(void)
{
  int           i;
  unsigned char c;
  unsigned char *ptr;
  
  ptr = (unsigned char*)&g_c;
#if 1
  for (i = 0; i < sizeof(g_c); i++)
    {
      c = eeprom_read_byte((uint8_t*)0x01 + i);
      if (c != ptr[i])
	eeprom_write_byte((uint8_t*)0x01 + i, ptr[i]);
    }
#else
  eeprom_write_block(&g_c, (uint8_t*)0x01, sizeof(g_c));
#endif
  eeprom_write_byte((uint8_t*)0x00, 0x00);
}

