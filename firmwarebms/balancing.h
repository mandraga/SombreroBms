
// Note that charging is done at the charger max current, it preserves battery life.

#define BALANCING_STATE_IDLE                   0
#define BALANCING_STATE_CHARGING_CSPEED        1
#define BALANCING_STATE_BWHILE_CHARGING        2
#define BALANCING_STATE_BWHILE_CHARGING_STOPED 3
#define BALANCING_STATE_COOLDOWN               4

// Clears all the balancing outputs
void stop_any_balancing(void);

// IDEAS ABOUT BALANCING

// Discharge balancing could be called in IDLE mode when the battery pack is setup.
// Then when (charging cycles count) > 10 it will stop.
// Discharge balacing uses low currents.
// However it can induce bugs and batteries should be balanced with a specialised
// hardware before use so it will not be implemented here.
// Plus V bat changes during the relapse time or relax time or whatever time!!!!
// Thus it must wait during charge.

// The balancing is done while the charger is on.
// 1) while charging based on the charging speed
// 2) after a battery is full (charger powered but off), the batteries are discharged
//    at a rate depending on their charging speed so that the resistors temperature is
//    equal during the balancing.
//
// The capacity of each battery is not calculated, the goal is that they end up with the
// same Voltage: Vbat / elements.
//
// In case of temperature overflow of the PCB, the charging and the balancing are stoped.
// It restarts with an histeresis value.
//

// The balancing could also be made when the last battery is in the middle of the charge
// voltage range. It's simpler. And half reliable.
// It would be like:
// 1 IDLE
// 2  balance every battery over the last value and depending on temperature.
//    if (tmax - t > 20)
//      all minus the lowest are balaced;
//    else
//      the higher (((tmax - t) / 20) * elements) are balanced
//
// 3 if (vbat = (vfull + vstart) / 2) balance everything to the lowest
//

typedef struct s_balancing
{
  char         state;
  int          vdiffmax_index;
  int          vlow_index;     // Reference battery, the lowest V. A combination between the most discharged and the bigger capacity.
}              t_balancing;

int get_highter_temperature(int *ptemperature, char modules);

void balancing_during_charge(t_balancing *pb, unsigned long *pvbat, char elements, int *ptemperature);

void balancing_charger_stoped(t_balancing *pb, unsigned long *pvbat, char elements, int *ptemperature);

