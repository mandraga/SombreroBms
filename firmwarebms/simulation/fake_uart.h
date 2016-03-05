
#define BSZ 4096

class Cfake_UART
{
public:
  Cfake_UART();
  ~Cfake_UART();

  int init();
  void send(char *bytes, int size);
  bool get_next_byte(char *byte);

private:
  int create_FIFO(char *fifo_path);

private:
  CSerialPortLocalSim *m_pserial;
  char input[BSZ];
  char output[BSZ];
};
