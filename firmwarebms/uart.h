
#define BAUDRATE   9600

/*  Taken from Peter Fleury code, (it was too tigh here to use his library)
 *  @brief  UART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000L for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT(baudRate,xtalCpu) ((xtalCpu)/((baudRate)*16l)-1)

#define RCVSTRINGSZ 54
#define TRSTRINGSZ  54

typedef struct  s_serialport
{
  char          RXstate;
  char          inbuffer[RCVSTRINGSZ];
  char          inCRC;
  int           inindex;
  char          TXstate;
  char          outbuffer[TRSTRINGSZ];
  char          outsize;
  char          outCRC;
  int           outindex;
  int           batcounter;
}               t_serialport;

void init_serial_vars(void);

void uart_init(unsigned int baudrate);
void uart_puts(char *str);
void uart_puts_P(const char *str);
void send_first_byte(void);

