
void replace_char(char a, char b, char *str)
{
  int len;
  int i;
  
  len = strlen(str);
  for (i = 0; i < len; i++)
    if (str[i] == a)
      str[i] = b;
}

int get_next_byte(unsigned char *ch, unsigned char *pstatus)
{
  unsigned int c;
  int          retry;
  int          timeout = 800;
  
  retry = 0;
  while (((c = uart_getc()) & UART_NO_DATA) && retry < timeout)
    {
      _delay_ms(1);
      retry++;
    }
  if (retry >= timeout) // Timeout
    return 1;
  *ch = c & 0xFF;
  *pstatus = c >> 8;
  return 0;
}

void empty_RX()
{
  unsigned char c, st;
  
  while (!get_next_byte(&c, &st));
}

void dump_input(void)
{
  unsigned char c, st;
  int           sz;
  char          str[5];

  lcd_clrscr();
  for (sz = 0;;)
    {
      if (get_next_byte(&c, &st))
	{
	  //_delay_ms(10000);
	  break;
	}
      else
	{
	  sprintf(str, "%X%X ", c >> 4, c & 0x0F);
	  lcd_puts(str);
	  sz++;
	  _delay_ms_S(1000);
	  if (sz % 16 == 0)
	    {
	      lcd_clrscr();
	      lcd_home();
	    }
	  if (sz % 8 == 0)
	    lcd_putc('\n');
	}
    }
}

void dump_string(unsigned char *ps, int sz)
{
  int   i;
  char  str[5];
  
  for (i = 0; i < sz; i++)
    {
      if (i % 8 == 0)
	{
	  lcd_putc('\n');
	}
      if (i % 8 == 0)
	{
	  lcd_clrscr();
	  lcd_home();
	  _delay_ms_S(1000);
	}
      sprintf(str, "%X%X ", ps[i] >> 4, ps[i] & 0x0F);
      lcd_puts(str);
      _delay_ms_S(1000);
    }
}

void dump_input_hex(void)
{
  unsigned char c, st;
  int           sz;
  unsigned char b[80];
  
  lcd_clrscr();
  for (sz = 0;;)
    {
      if (get_next_byte(&c, &st))
	{
	  //_delay_ms(10000);
	  break;
	}
      //lcd_putc(c);
      b[sz] = c;
      sz++;
    }
  dump_string(b, sz);
}

void fbus_send_command(unsigned char command, unsigned char *pdata, int len)
{
  unsigned char data[256];
  int           sz;
  int           i;
  unsigned char sum_even;
  unsigned char sum_odd;
  
  data[0] = 0x1E;
  data[1] = 0x00; // Destination: the phone
  data[2] = 0x0C; // Source: the pc
  data[3] = command;
  // overflow check
  if (len - 6 > UART_TX_BUFFER_SIZE)
    {
      lcd_clrscr();
      lcd_puts("tx overflow");
      _delay_ms_S(12000);
      return; // FIMXE??
    }
  if (len >= 0x78)
    {
      lcd_clrscr();
      lcd_puts("message too long");
      _delay_ms_S(12000);
      return; // FIMXE??
    }
  // Message size
  data[4] = len >> 8;    // H byte
  data[5] = len & 0xFF;  // L byte
  sz = 6;	
  for (i = 0; i < len; i++)
    {
      data[sz + i] = pdata[i];
    }
  sz += len;
  // Odd even message length check
  if (sz & 1)
    {
      data[sz] = 0; // 0 padding
      sz++;
    }
  // calculate the check sum
  message_check_sum(sz, data, &sum_even, &sum_odd);
  data[sz] = sum_even;
  data[sz + 1] = sum_odd;
  sz += 2;
  // Send the message
  //dump_string(data, sz);
  fbus_send_bytes(data, sz);
  // Wait for all the data to be sent
  wait_transmit();
}

// Sends VBat for each battery + Current +  state
void send_VBAT()
{
}

// Sends the charge data for the last charge
void send_LastCharge()
{
}

// Sends the error log from the eeprom
// first   per battary event count
// second  error queue
void send_err_log()
{
  
}

// Sends global information about the pack
// Parto of it is eeprom, part of it is in ram
void send_info()
{
  
}

