
#include "fake_battery.h"

Cfake_battery::Cfake_battery(int capacity_mAH, int initial_charge_mAH)
{
  m_capacity = capacity_mAH;
  m_charge = initial_charge_mAH;
  m_chmv = (float)(BAT_MAXV - BAT_MINMV) / m_capacity;
}

Cfake_battery::~Cfake_battery()
{
  
}

int Cfake_battery::get_mV()
{
  if (charge < 0)
    {
      return m_chmv * charge * 5.;
    }
  if (charge > capacity)
    {
      return BAT_MAXV + m_chmv * (charge - m_capacity) * 5.;
    }
  return m_chmv * charge;
}

void Cfake_battery::add_charge(float mAmps, float mSeconds)
{
  charge += mAmps * mSeconds;
}

void Cfake_battery::shunt(float mAmps, float mSeconds)
{
  charge -= mAmps * mSeconds;
}

