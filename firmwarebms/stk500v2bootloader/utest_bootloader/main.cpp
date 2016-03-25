#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include "command.h"

#define PORT     "/dev/ttyUSB0"
#define BAUDRATE B115200

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
  tcsetattr(fd, TCSANOW, &tty);
}

int main(int argc, char *argv[])
{
  unsigned char  command[256];
  int            cmdsize;
  int            fd;
  struct termios tty, old;
  int            msg_count;
  int            msg_size;
  int            i, crc;
  size_t         written;
  int            read_bytes;
  unsigned char  c[256];
  int            cnt;

  // Open the port
  init_serial_port(PORT, fd, tty, old);

  // Create the message (CMD_SIGN_ON)
  msg_count = 0;
  cmdsize = 0;
  command[cmdsize++] = MESSAGE_START;
  command[cmdsize++] = msg_count++;
  msg_size = 1;
  command[cmdsize++] = msg_size >> 8;
  command[cmdsize++] = msg_size & 0xFF;
  command[cmdsize++] = TOKEN;
  command[cmdsize++] = CMD_SIGN_ON;
  for (i = 0, crc = 0; i < cmdsize; i++)
    {
      crc = crc ^ command[i];
    }
  command[cmdsize++] = crc;

  //stk_receive(command, cmdsize);

  // Sent the command to the device
  if (fd != 0)
    {
      printf("-> Sending a serial command:\n");
      for (i = 0; i < cmdsize; i++)
	printf("0x%02x\n", command[i]);
      written = write(fd, command, cmdsize);
      if (written == (size_t)cmdsize)
	{
	  printf("success\n");
	}
    }

  // Wait for some response
  usleep(10000);

  // Read it
  printf("Reading:\n");
  cnt = 0;
  while (cnt < 300)
    {
      read_bytes = read(fd, c, 1);
      while (read_bytes > 0)
	{
	  for (i = 0; i < (int)read_bytes; i++)
	    printf("0x%01x %c\n", c[i], c[i]);
	  read_bytes = read(fd, c, 1);
	}
      usleep(10000);
      cnt++;
    }

  // Close the port
  if (fd != 0)
    {
      // On revient à l'ancien et on quitte
      tcsetattr(fd, TCSANOW, &old);
      close(fd);
    }
  return 0;
}
