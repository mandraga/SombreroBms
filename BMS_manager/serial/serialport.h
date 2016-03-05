
#define SERPORTNAME "/dev/ttyUSB0"

class CSerialPort
{
public:
  CSerialPort();
  virtual ~CSerialPort();

  virtual int  open_serial_port(const char *devicename);
  virtual void close_serial_port();
  int  read_serial_port(char *data, int maxsize);
  bool read_next_byte(char *byte);
  virtual int  write_serial_port(char *data, unsigned int size);

protected:
  int             m_fd;
private:
  struct termios  m_tty, m_old;
};

// Used to connect to a simulator locally
class CSerialPortLocalSim : public CSerialPort
{
public:
  CSerialPortLocalSim(bool bmaster);
  ~CSerialPortLocalSim();

  int  open_serial_port(const char *devicename);
  void close_serial_port();
  int  write_serial_port(char *data, unsigned int size);

private:
  int             m_fd_TX;
  bool            m_bsim;
};
