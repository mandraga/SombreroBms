
#define BAT_MINMV 3000
#define BAT_MAXV  3600

#define CHARGESTOPTHRESHOLD 39300 // 39,2V

class Cfake_battery
{
 public:
  Cfake_battery(int capacity_mAH, int initial_charge_mAH);
  ~Cfake_battery();

  int  get_mV(); // Depends on charge
  void add_charge(float mAmps, float mSeconds);
  void shunt(float mAmps, float mSeconds);

 public:
  float m_charge;
  float m_capacity;
};
