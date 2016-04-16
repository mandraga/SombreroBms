
class Ckeypress
{
 public:
  Ckeypress();

  void key_on(int code);
  void key_off(int code);

  bool is_pressed(int code);
 private:
  bool  m_bkeypress[256];
};
