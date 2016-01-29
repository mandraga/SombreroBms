
//#define SHUNT_CURRENT 1280 // mA
#define SHUNT_CURRENT 150000 // mA

class Cfake_AD7280A
{
 public:
  Cfake_AD7280A(int modules);
  ~Cfake_AD7280A();

  int  get_balancing(int batnum);
  void set_balancing(int batnum, bool balance);
  void balancing_shunt(Cfake_battery *pbats, int batnum, float delayms);
  void set_temp(int temp);
  int  get_temp();

 public:
  int m_modules;
  int m_balance;
  int m_temperature;
};

