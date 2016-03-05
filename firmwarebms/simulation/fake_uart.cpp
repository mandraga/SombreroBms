#include <string.h>
#include <stdio.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "serialport.h"
#include "fake_uart.h"

Cfake_UART::Cfake_UART()
{
  bool bmaster;

  bmaster = true;
  m_pserial = new CSerialPortLocalSim(bmaster);
}

Cfake_UART::~Cfake_UART()
{
  m_pserial->close_serial_port();
  delete m_pserial;
}

int Cfake_UART::create_FIFO(char *fifo_path)
{
  struct stat st;
  int         res;

  res = stat(fifo_path, &st);
  if (res < 0)
    {
#ifdef VERBOSE
      printf("Adding the named pipe: \"%s\"\n", fifo_path);
#endif
      // Create a named pipe
      int pipe = mkfifo(fifo_path, S_IRUSR | S_IWUSR); //0664);
      if (pipe < 0)
	{
	  printf("mkfifo failed on \"%s\"\n", fifo_path);
	  return 0;
	}
    }  
  return 1;
}

int Cfake_UART::init()
{
  char        FIFO_RX_path[512];
  char        FIFO_TX_path[512];

  // Create a fifo in the 2 directions
  strncpy(FIFO_TX_path, "/tmp/BMSsim_serialFIFO_TX", 512);
  create_FIFO(FIFO_TX_path);
  strncpy(FIFO_RX_path, "/tmp/BMSsim_serialFIFO_RX", 512);
  create_FIFO(FIFO_RX_path);
  // Open the port with the special subclass
  return m_pserial->open_serial_port("unused");
}

void Cfake_UART::send(char *bytes, int size)
{
  m_pserial->write_serial_port(bytes, size);
  //for (int i = 0; i < size; i++)
  //  printf("sending %c %d\n", bytes[i], bytes[i]); 
}

bool Cfake_UART::get_next_byte(char *byte)
{
  return m_pserial->read_next_byte(byte);
}

