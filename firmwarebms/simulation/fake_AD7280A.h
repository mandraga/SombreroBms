
class Cfake_AD7280A
{
 public:
  Cfake_AD7280A(int modules);
  ~Cfake_AD7280A();

  int  get_balancing(int batnum);
  void set_balancing(int batnum, bool balance);
  void balancing_shunt(Cfake_battery *pbats, int batnum, float delayms);
  void set_AUX_mV(int tempmv);
  int  get_AUX_mV();

 public:
  int m_modules;
  int m_balance = 0;
  int m_temperature;
};

