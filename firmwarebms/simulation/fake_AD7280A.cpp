#include <unistd.h>
#include <iterator>
#include <list>
#include <vector>
#include <string>
//
#include "fake_battery.h"
#include "fake_AD7280A.h"
#include "fake_charger.h"
//

Cfake_AD7280A::Cfake_AD7280A(int modules)
{
  m_modules = modules;
  m_balance = 0;
  m_temperature = 25;
}

Cfake_AD7280A::~Cfake_AD7280A()
{
  
}

void Cfake_AD7280A::set_balancing(int batnum, bool balance)
{
  int mask;

  if (balance)
    m_balance = m_balance | (1 << batnum);
  else
    {
      mask = ~(1 << batnum);
      m_balance = m_balance & mask;
    }
}

int Cfake_AD7280A::get_balancing(int batnum)
{
  return (m_balance & (1 << batnum)) != 0? 1 : 0;
}

void Cfake_AD7280A::balancing_shunt(Cfake_battery *pbats, int batnum, float delayms)
{
  int   i;
  float mAmps = 1180; // 1,8Amps

  for (i = 0; i < batnum; i++)
    {
      if (m_balance & (1 << i))
	pbats[i].shunt(mAmps, delayms);
    }
}

void Cfake_AD7280A::set_temp(int temp)
{
  m_temperature = temp;
}

int Cfake_AD7280A::get_temp()
{
  return m_temperature;
}

