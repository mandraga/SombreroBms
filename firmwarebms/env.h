
// All the custom values are here

// Number of monitored cells
#define CFGBATNUMBER     11

#if CFGBATNUMBER==10
#define CFGAD728AMODULE_0_CHAN 5
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==11
#define CFGAD728AMODULE_0_CHAN 6
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==12
#define CFGAD728AMODULE_0_CHAN 6
#define CFGAD728AMODULE_1_CHAN 6
#define CFGAD728AMODULES  2
#endif
#if CFGBATNUMBER==15
#define CFGAD728AMODULE_0_CHAN 5
#define CFGAD728AMODULE_1_CHAN 5
#define CFGAD728AMODULE_2_CHAN 5
#define CFGAD728AMODULES  3
#endif

// Max channels for a 3 modules board
#define MAX_CHANNELS  ((6 + 3) * 3)

