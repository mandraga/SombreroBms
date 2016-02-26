#include <stdio.h>
#include <termios.h>

#include "serialport.h"
#include "fake_uart.h"

Cfake_UART::Cfake_UART()
{
  m_pserial = new CSerialPort();
}

Cfake_UART::~Cfake_UART()
{
  m_pserial->close_serial_port();
  delete m_pserial;
}

int Cfake_UART::init()
{
  return m_pserial->open_serial_port((char*)"/tmp/BMSsim_serialFIFO");
}

void Cfake_UART::send(char *bytes, int size)
{
  m_pserial->write_serial_port(bytes, size);
}

bool Cfake_UART::get_next_byte(char *byte)
{
  return m_pserial->read_next_byte(byte);
}

