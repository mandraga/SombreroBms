
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "serialport.h"

CSerialPort::CSerialPort()
{
  m_fd = 0;
}

CSerialPort::~CSerialPort()
{
  close_serial_port();
}

// First call:
// prompt> mkfifo /tmp/BMSsim_serialFIFO
// to use a named pipe instead of a serial port for simulation purposes.
int CSerialPort::open_serial_port(char *devicename)
{  
  // Open the device
  if ((m_fd = open(devicename, O_RDWR)) < 0)
    {
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
  m_tty.c_cc[VMIN]   =  1;                  // read doesn't block
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
	  printf("%c", c[0]);
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
	      printf("%c", c);
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
      written = write(m_fd, data, size);
      if (written == size)
	{
	  strsize = written;
	}
    }
  return strsize;
}

