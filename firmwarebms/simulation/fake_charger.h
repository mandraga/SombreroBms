
#define CHARGER_AMPS 33000.

class Cfake_charger
{
 public:
  Cfake_charger();
  ~Cfake_charger();

  void  plug_charger(bool bpluged);
  void  stop_charger(bool bstop);
  float get_Vpack(Cfake_battery *pbats, int batnum);
  void  check_Vmax(Cfake_battery *pbats, int batnum);
  void  add_charge(Cfake_battery *pbats, int batnum, float timems);
  float get_TotalCharge(Cfake_battery *pbats, int batnum);

 public:
  bool m_bpluged;
  bool m_active;
};
