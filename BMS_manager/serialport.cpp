
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>

#include "serialport.h"

CSerialPort::CSerialPort()
{
  m_fd = NULL;
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
      m_fd = NULL;
      return 0;
    }
  // Lecture des parametres
  tcgetattr(m_fd, &tty);
  tcgetattr(m_fd, &old);
  
  // disconnect the line whatever
  //cfsetospeed(&tty, B0);
  //cfsetispeed(&tty, B0);

  // 9600 baud
  cfsetospeed(&tty, B9600);
  cfsetispeed(&tty, B9600);
  
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
  tcsetattr(m_fd, TCSANOW, &tty);
  return 1;
}

void CSerialPort::close_serial_port()
{
  if (m_fd != NULL)
    {
      // On revient à l'ancien et on quitte
      tcsetattr(fd, TCSANOW, &m_old);
      close(m_fd);
    }
}

int CSerialPort::read_serial_port(char *data, int maxsize)
{
  char c[256];
  int  readbytes;
  int  strsize = 0;

  if (m_fd != NULL)
    {
      do
	{
	  readbytes = read(m_fd, c, 1);
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

int CSerialPort::write_serial_port(char *data, int size)
{
  int    strsize = 0;
  size_t written;

  if (m_fd != NULL)
    {
      written = write(m_fd, data, size);
      if (written == size)
	{
	  strsize = written;
	}
    }
  return strsize;
}
