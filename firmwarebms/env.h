
// All the custom values are here

#define FIRMWARE_VERSION    0
#define FIRMWARE_SUBVERSION 1

//----------------------------------------------------------
// Value in mHom of the current measurement shunt
//----------------------------------------------------------
#define SHUNT_VAL    250L // µhoms
#define INA197GAIN    50L
#define DACREFV     5000L // 5V

//----------------------------------------------------------
// Number of monitored cells
//----------------------------------------------------------
#define CFGBATNUMBER     11

//----------------------------------------------------------
// Maximum peak current
//----------------------------------------------------------
#define MAXP_CURRENT     300L  // Amp
#define MAXC_CURRENT     180L  // Amp

#define MAXBATTERY 18
#define MAXMODULES  3

// Max AD7280A output channels for a 3 modules board
#define MAX_CHANNELS  ((6 + 3) * MAXMODULES)

// Discharge current to pass to run mode, in mA
// An hysteresis of DISCHARGE_THRESHOLD / 2 is applyed somewhere.
#define DISCHARGE_THRESHOLD 100L

// Measurements per second
#define SAMPLING_PER_SECOND  10L
#define SAMPLING_INTERVAL_MS (1000L / SAMPLING_PER_SECOND)
// Number of current measures for one hour, 10 per second
#define INTERVALS_PER_HOUR (60L * 60L * SAMPLING_PER_SECOND)

#define TEMPERATURE_HISTERESIS 15L   // °C

#define DEFAULT_CHARGE  1000

#if CFGBATNUMBER==10
#define CFGAD728AMODULE_0_CHAN 5
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULE_2_CHAN 0
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==11
#define CFGAD728AMODULE_0_CHAN 6
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULE_2_CHAN 0
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==12
#define CFGAD728AMODULE_0_CHAN 6
#define CFGAD728AMODULE_1_CHAN 6
#define CFGAD728AMODULE_2_CHAN 0
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==15
#define CFGAD728AMODULE_0_CHAN 5
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULE_2_CHAN 5
#define CFGAD728AMODULES  3
#endif


