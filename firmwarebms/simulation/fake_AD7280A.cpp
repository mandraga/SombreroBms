
#include "fake_AD7289A.h"

Cfake_AD7280A::Cfake_AD7280A(int modules)
{
  int i;

  m_modules = modules;
  m_balance = 0;
  m_temperature = 4467;
}

Cfake_AD7280A::~Cfake_AD7280A()
{
  
}

void Cfake_AD7280A::set_balancing(int batnum, bool balance)
{
  if (balance)
    m_balance = m_balance | (1 << batnum);
  else
    m_balance = m_balance & ~(1 << batnum);
}

int Cfake_AD7280A::get_balancing(int batnum)
{
  return (m_balance & (1 << batnum)) != 0? 1 : 0;
}

void Cfake_AD7280A::balancing_shunt(Cfake_battery *pbats, int batnum, float delayms)
{
  int   i;
  float mAmps = 1180; // 1,8Amps

  charge = 0;
  for (i = 0; i < batnum; i++)
    {
      if (m_balance & (1 << i))
	pbats[i].shunt(mAmps, delayms);
    }
}

void Cfake_AD7280A::set_AUX_mV(int tempmv)
{
  m_temperature = tempmv;
}

int Cfake_AD7280A::get_AUX_mV()
{
  return m_temperature;
}
