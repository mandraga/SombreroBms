
#define BAT_MINMV 2990
//#define BAT_CHRGV 3300
#define BAT_MAXV  3800

#define CHARGESTOPTHRESHOLD 39300 // 39,2V

#define CAPACITYMAH 60000

class Cfake_battery
{
 public:
  Cfake_battery();
  ~Cfake_battery();

  void set_capa(int capacity_mAH, int initial_charge_mAH);
  int  get_mV(); // Depends on charge
  void add_charge(float mAmps, float mSeconds);
  void shunt(float mAmps, float mSeconds);
  bool is_dead();

 public:
  float m_charge;
  float m_capacity;
 private:
  float m_chmv;
  bool  m_dead; // If charge > 10% capacity
};
