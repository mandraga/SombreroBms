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

int create_command(unsigned char *command, int *msg_count, unsigned char cmd, unsigned char *buffer, int buffer_size)
{
  int i, crc;
  int cmdsize, msg_size;

  cmdsize = 0;
  command[cmdsize++] = MESSAGE_START;
  command[cmdsize++] = (*msg_count)++;
  msg_size = 1 + buffer_size;
  command[cmdsize++] = msg_size >> 8;
  command[cmdsize++] = msg_size & 0xFF;
  command[cmdsize++] = TOKEN;
  // Command code
  command[cmdsize++] = cmd;
  // Buffer data
  for (i = 0; i < buffer_size; i++)
    command[cmdsize++] = buffer[i];
  // Crc
  for (i = 0, crc = 0; i < cmdsize; i++)
    {
      crc = crc ^ command[i];
    }
  command[cmdsize++] = crc;
  return cmdsize;
}

void send_command(int fd, unsigned char *command, int cmdsize)
{
  int    i;
  size_t written;

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
}

void read_return(int fd, const char *str)
{
  int            read_bytes;
  int            cnt;
  int            i;
  unsigned char  crc;
  unsigned char  c[256];

  // Read it
  printf("Reading %s:\n", str);
  cnt = 0;
  crc = 0;
  while (cnt < 300)
    {
      read_bytes = read(fd, c, 1);
      while (read_bytes > 0)
	{
	  for (i = 0; i < (int)read_bytes; i++)
	    {
	      crc ^= c[i];
	      printf("0x%02x %c\n", c[i], c[i]);
	    }
	  read_bytes = read(fd, c, 1);
	}
      usleep(1000);
      cnt++;
    }
  printf("must be 0 - crc = 0x%02x\n", crc);
  printf("reading stoped\n");
}

#define SLEEP_DELAY 1000

int main(int argc, char *argv[])
{
  unsigned char  command[256];
  int            cmdsize;
  unsigned char  command_data[256];
  int            cmd_data_size;
  int            fd;
  struct termios tty, old;
  int            msg_count;

  // Open the port
  init_serial_port(PORT, fd, tty, old);

  //----------------------------------------------------------------------------
  // Create the message (CMD_SIGN_ON)
  msg_count = 0;
  cmd_data_size = 0;
  cmdsize = create_command(command, &msg_count, CMD_SIGN_ON, command_data, cmd_data_size);

  //stk_receive(command, cmdsize);

  // Sent the command to the device
  send_command(fd, command, cmdsize);

  // Wait for some response
  //usleep(SLEEP_DELAY);
  read_return(fd, "CMD_SIGN_ON");

#define READEVELSE
#ifdef READEVELSE
  //----------------------------------------------------------------------------
  // Flash page erase test
  //
  // upload the data
  // program the flash
  // download the result and compare
  //
  //----------------------------------------------------------------------------
  int address = 0;

  command_data[0] = 0;
  command_data[1] = 0;
  command_data[2] = (address >> 8) & 0xFF;
  command_data[3] = address & 0xFF;
  cmd_data_size = 4;
  //
  cmdsize = create_command(command, &msg_count, CMD_LOAD_ADDRESS, command_data, cmd_data_size);
  //stk_receive(command, cmdsize);
  send_command(fd, command, cmdsize);


  // Wait for some response
  usleep(SLEEP_DELAY);
  read_return(fd, "CMD_LOAD_ADDRESS");

  for (cmd_data_size = 0; cmd_data_size < 128; cmd_data_size++)
    {
      command_data[cmd_data_size] = 0xCE;
    }
  //
  cmdsize = create_command(command, &msg_count, CMD_PROGRAM_FLASH_ISP, command_data, cmd_data_size);
  //stk_receive(command, cmdsize);
  send_command(fd, command, cmdsize);

  // Wait for some response
  usleep(SLEEP_DELAY);
  read_return(fd, "CMD_PROGRAM_FLASH_ISP");

  //----------------------------------------------------------------------------
  // Read the result
  //----------------------------------------------------------------------------
  command_data[0] = 0;
  command_data[1] = 0;
  command_data[2] = (address >> 8) & 0xFF;
  command_data[3] = address & 0xFF;
  cmd_data_size = 4;
  //
  cmdsize = create_command(command, &msg_count, CMD_LOAD_ADDRESS, command_data, cmd_data_size);
  //stk_receive(command, cmdsize);
  send_command(fd, command, cmdsize);

  // Wait for some response
  usleep(SLEEP_DELAY);
  read_return(fd, "CMD_LOAD_ADDRESS");

  cmd_data_size = 0;
  command_data[cmd_data_size++] = 0;
  command_data[cmd_data_size++] = 128;
  command_data[cmd_data_size++] = 0;
  //
  cmdsize = create_command(command, &msg_count, CMD_READ_FLASH_ISP, command_data, cmd_data_size);
  //stk_receive(command, cmdsize);
  send_command(fd, command, cmdsize);

  // Wait for some response
  usleep(SLEEP_DELAY);
  read_return(fd, "CMD_PROGRAM_READ_ISP");

  //----------------------------------------------------------------------------
#endif

  // Close the port
  if (fd != 0)
    {
      // On revient à l'ancien et on quitte
      tcsetattr(fd, TCSANOW, &old);
      close(fd);
    }
  return 0;
}

