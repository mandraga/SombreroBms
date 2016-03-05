#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "serialport.h"

//#define SERVERBOSE

CSerialPort::CSerialPort()
{
  m_fd = 0;
}

CSerialPort::~CSerialPort()
{
}

// First call:
// prompt> mkfifo /tmp/BMSsim_serialFIFO
// to use a named pipe instead of a serial port for simulation purposes.
int CSerialPort::open_serial_port(const char *devicename)
{
  // Open the device
  if ((m_fd = open(devicename, O_RDWR | O_NONBLOCK)) < 0)
    {
      printf("Error: failed to open the serial port at \"%s\"\n", devicename);
      perror(devicename);
      m_fd = 0;
      return 0;
    }
  // Read the tty parameters
  tcgetattr(m_fd, &m_tty);
  m_old = m_tty;
  
  // disconnect the line whatever
  //cfsetospeed(&tty, B0);
  //cfsetispeed(&tty, B0);

  // 9600 baud
  cfsetospeed(&m_tty, B9600);
  cfsetispeed(&m_tty, B9600);
  
  /*
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  
  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
  */
  tcsetattr(m_fd, TCSANOW, &m_tty);
  return 1;
}

void CSerialPort::close_serial_port()
{
  if (m_fd != 0)
    {
      // On revient à l'ancien et on quitte
      tcsetattr(m_fd, TCSANOW, &m_old);
      close(m_fd);
    }
}

bool CSerialPort::read_next_byte(char *byte)
{
  char c[8];
  int  readbytes;

  if (m_fd != 0)
    {
      readbytes = read(m_fd, c, 1);
      if (readbytes > 0)
	{
	  *byte = c[0];
#ifdef SERVERBOSE
	  printf("readbyte \'%c\' %d\n", *byte, *byte);
#endif
	  return true;
	}
    }
  return false;
}

int CSerialPort::read_serial_port(char *data, int maxsize)
{
  char c;
  int  readbytes;
  int  strsize = 0;

  if (m_fd != 0)
    {
      do
	{
	  readbytes = read(m_fd, &c, 1);
	  if (readbytes > 0)
	    {
	      data[strsize++] = c;
#ifdef SERVERBOSE
	      printf("%c", c);
#endif
	    }
	}
      while (c != '\n' && strsize < maxsize - 1);
      data[maxsize - 1] = '\n';
    }
  return strsize;
}

int CSerialPort::write_serial_port(char *data, unsigned int size)
{
  int    strsize = 0;
  size_t written;

  if (m_fd != 0)
    {
      printf("-> Sending a serial command: \"%s\".\n", data);
      written = write(m_fd, data, size);
      if (written == size)
	{
	  strsize = written;
	}
    }
  return strsize;
}

//---------------------------------------------------------------------------
// Local simulation
//---------------------------------------------------------------------------

CSerialPortLocalSim::CSerialPortLocalSim(bool bmaster) : CSerialPort()
{
  m_bsim = bmaster;
  m_fd_TX = 0;
}

CSerialPortLocalSim::~CSerialPortLocalSim()
{
}

// First call:
// prompt> mkfifo /tmp/BMSsim_serialFIFO
// to use a named pipe instead of a serial port for simulation purposes.
int CSerialPortLocalSim::open_serial_port(const char *devicename)
{
  char tx_path[4096];
  char rx_path[4096];

  if (m_bsim)
    {
      strcpy(tx_path, "/tmp/BMSsim_serialFIFO_TX");
      strcpy(rx_path, "/tmp/BMSsim_serialFIFO_RX");
    }
  else
    {
      strcpy(tx_path, "/tmp/BMSsim_serialFIFO_RX");
      strcpy(rx_path, "/tmp/BMSsim_serialFIFO_TX");      
    }

  // Open the fifos
  if ((m_fd = open(rx_path, O_RDONLY | O_NONBLOCK)) < 0)
    {
      printf("Error: failed to open for reading the serial port at \"%s\"\n", rx_path);
      perror(rx_path);
      m_fd = 0;
      return 0;
    }
  // Locks until the other side is opened
  if ((m_fd_TX = open(tx_path, O_WRONLY )) < 0)
    {
      printf("Error: failed to open for writing the serial port at \"%s\"\n", tx_path);
      perror(tx_path);
      m_fd_TX = 0;
      return 0;
    }
  return 1;
}

void CSerialPortLocalSim::close_serial_port()
{
  if (m_fd != 0)
    close(m_fd);
  if (m_fd_TX != 0)
    close(m_fd_TX);
  m_fd = m_fd_TX = 0;
}

// Same
//bool CSerialPortLocalSim::read_next_byte(char *byte)
//int CSerialPortLocalSim::read_serial_port(char *data, int maxsize)

int CSerialPortLocalSim::write_serial_port(char *data, unsigned int size)
{
  int    strsize = 0;
  size_t written;

  if (m_fd_TX != 0)
    {
      printf("-> Sending a serial command: \"%s\".\n", data);
      written = write(m_fd_TX, data, size);
      if (written == size)
	{
	  strsize = written;
	}
    }
  return strsize;
}

