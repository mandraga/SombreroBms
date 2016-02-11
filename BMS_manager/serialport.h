
#define SERPORTNAME "/dev/ttyUSB0"

class CSerialPort
{
public:
  CSerialPort();
  ~CSerialPort();

  int open_serial_port(char *devicename);
  int close_serial_port();
  int read_serial_port(char *data, int maxsize);
  int write_serial_port(char *data, int size);

private:
  FILE m_fd;
  struct termios m_tty, m_old;
};

