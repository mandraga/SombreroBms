//-----------------------------------------------------------------------------------------------
// Tests common functions of a boot loader with stk500v2 protocol
//
// Used to port the bootloader to new architectures and check if it works
//
// The protocol can be found in the atmel's appnote AVR068.pdf
//
// If still more trouble with the bootloader:
// + add #if 1
//       #define DEBUG(...) fprintf(stderr, __VA_ARGS__)
// In avrdudes's stk500v2.c to be able to see what is transmited on the line and
// were it fails.
//
//-----------------------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define PORT     "/dev/ttyUSB0"
#define BAUDRATE B9600

int stk_receive(unsigned char *msg, int size);

void init_serial_port(const char *devicename, int &fd, struct termios &tty, struct termios &old)
{
  // Open the device
  if ((fd = open(devicename, O_RDWR | O_NONBLOCK)) < 0)
    {
      printf("Error: failed to open the serial port at \"%s\"\n", devicename);
      perror(devicename);
      fd = 0;
      return;
    }
  // Read the tty parameters
  tcgetattr(fd, &tty);
  old = tty;

  // disconnect the line whatever
  //cfsetospeed(&tty, B0);
  //cfsetispeed(&tty, B0);

  // baudrate
  cfsetospeed(&tty, BAUDRATE);
  cfsetispeed(&tty, BAUDRATE);
  
  
  tty.c_cflag     &=  ~PARENB;            // Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  
  tty.c_cflag     &=  ~CRTSCTS;           // no flow control
  tty.c_cc[VMIN]   =  1;                  // read doesn't block
  tty.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
  tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
  
  tcsetattr(fd, TCSANOW, &tty);
}

void read_return(int fd, const char *str)
{
  int            read_bytes;
  int            cnt;
  int            i;
  const int      csz = 4096;
  unsigned char  c[csz];

  // Read it
  //printf("Reading %s:\n", str);
  cnt = 0;
  while (cnt < 300)
    {
      read_bytes = read(fd, c, 1);
      while (read_bytes > 0 && read_bytes < csz)
	{
	  for (i = 0; i < (int)read_bytes; i++)
	    {
	      printf("%c", c[i]);
	    }
	  read_bytes = read(fd, c, 1);
	}
      usleep(1000);
      cnt++;
    }
  //printf("-- next\n");
}

#define SLEEP_DELAY 1000

int main(int argc, char *argv[])
{
  int            fd;
  struct termios tty, old;

  // Open the port
  init_serial_port(PORT, fd, tty, old);

  //----------------------------------------------------------------------------

  usleep(SLEEP_DELAY);
  printf("Ready to read:\n");
  while (true)
    read_return(fd, "input");

  //----------------------------------------------------------------------------

  // Close the port
  if (fd != 0)
    {
      // Restore the old tty attributes
      tcsetattr(fd, TCSANOW, &old);
      close(fd);
    }
  return 0;
}

