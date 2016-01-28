
#include "fake_battery.h"
#include "fake_charger.h"

Cfake_charger::Cfake_charger()
{
  m_bpluged = false;
  m_active  = false;
}

Cfake_charger::~Cfake_charger()
{
}

void Cfake_charger::plug_charger(bool bpluged)
{
  m_bpluged = bpluged;
}

void Cfake_charger::stop_charger(bool bstop)
{
  m_active = !bstop;
}

float Cfake_charger::get_TotalCharge(Cfake_battery *pbats, int batnum)
{
  int   i;
  float charge;

  charge = 0;
  for (i = 0; i < batnum; i++)
    {
      charge += pbats[i].m_charge;
    }
  charge = charge / batnum;
  return charge;
}

float Cfake_charger::get_Vpack(Cfake_battery *pbats, int batnum)
{
  int   i;
  float Vsum;

  Vsum = 0;
  for (i = 0; i < batnum; i++)
    {
      Vsum += pbats[i].get_mV();;
    }
  return Vsum;
}

void Cfake_charger::check_Vmax(Cfake_battery *pbats, int batnum)
{
  float Vsum;

  Vsum = get_Vpack(pbats, batnum);
  if (Vsum > CHARGESTOPTHRESHOLD)
    stop_charger(true);
}

void Cfake_charger::add_charge(Cfake_battery *pbats, int batnum, float timems)
{
  int   i;

  check_Vmax(pbats, batnum);
  if (!m_active)
    {
      return;
    }
  for (i = 0; i < batnum; i++)
    {
      pbats[i].add_charge(CHARGER_AMPS, timems);
    }
}


