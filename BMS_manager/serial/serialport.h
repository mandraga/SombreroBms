
#define SERPORTNAME "/dev/ttyUSB0"

class CSerialPort
{
public:
  CSerialPort();
  ~CSerialPort();

  int  open_serial_port(char *devicename);
  void close_serial_port();
  int  read_serial_port(char *data, int maxsize);
  bool read_next_byte(char *byte);
  int  write_serial_port(char *data, unsigned int size);

private:
  int             m_fd;
  struct termios  m_tty, m_old;
};

