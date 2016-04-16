
#define EZGO
#ifdef EZGO
#define VBATENDCHARGE 39.
#define VBATOCVDOWN   36.6
#define VBATOCVUP     37.2
#define VBATRELAXHI   36.2 // Most important value, reference for current use
#define VBATRELAXLO   35.2 // At 80% of discharge
#define VBATLOW       32.
#else
// Power supply test values
#define VBATENDCHARGE 31.
#define VBATOCVDOWN   28.6
#define VBATOCVUP     29.2
#define VBATRELAXHI   28.2 // Most important value, reference for current use
#define VBATRELAXLO   27.2 // At 80% of discharge
#define VBATLOW       23.
#endif

// Calibration
#define CALIBRATION_V     32.
#define CALIBRATION_CONV 596.

#define BATCAP        60L // Amps
//#define BATLIMIT      12 // At 80% of discharge
#define BATLIMIT      20L

#define LODISCHARGEC    166L
#define LOWCV           34.1 // Higher discharge current from here down
#define HIDISCHARGEC    300L

#define RELAX_THRESHOLD 100L // 0.1V

#define STATE_CHARGED   1
#define STATE_INUSE     2
#define STATE_RELAX     3
#define STATE_TOO_LOW   4
#define STATE_CHARGING  5

#define TIME_INTERVALL    (100L)          // 100ms
#define INTERVAL_PER_HOUR (60L * 60L * 10L)
#define RELAX_EVENTS      (100L)      // 10 x 10events = 10 second

#define VTPBRODGEV(V) (V * 2.9 / 32.)
//#define VTPBRODGEV(V) (V)

void fill_vbat_arr(char *arr, long vbat);

