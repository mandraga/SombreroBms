
#include "fake_battery.h"

Cfake_battery::Cfake_battery()
{
}

Cfake_battery::~Cfake_battery()
{
  
}

void Cfake_battery::set_capa(int capacity_mAH, int initial_charge_mAH)
{
  m_dead = false;
  m_capacity = capacity_mAH;
  m_charge = initial_charge_mAH;
  m_chmv = (float)(BAT_MAXV - BAT_MINMV) / m_capacity;
}

int Cfake_battery::get_mV()
{
  if (m_dead)
    return 1300;
  if (m_charge < 0)
    {
      return m_chmv * m_charge * 5.;
    }
  if (m_charge > m_capacity)
    {
      return BAT_MAXV + m_chmv * (m_charge - m_capacity) * 5.;
    }
  return BAT_MINMV + m_chmv * m_charge;
}

void Cfake_battery::add_charge(float mAmps, float mSeconds)
{
  if (m_dead)
    return ;
  m_charge += mAmps * mSeconds / (float)(60 * 60 * 1000);
  if (m_charge > m_capacity * 1.1)
    m_dead = true;
}

void Cfake_battery::shunt(float mAmps, float mSeconds)
{
  if (m_dead)
    return ;
  m_charge -= mAmps * mSeconds / (float)(60 * 60 * 1000);
}

