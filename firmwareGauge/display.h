
// Calibration
#define CALIBRATION_V     32.
#define CALIBRATION_CONV 596.

#define TIME_INTERVALL    (100L)          // 100ms
//#define INTERVAL_PER_HOUR (60L * 60L * 10L)

#define VTPBRODGEV(V) (V * 2.9 / 32.)
//#define VTPBRODGEV(V) (V)

// Sets an IO port to 0, the duration of the LOW interval gives tha value to be displayed on the custom gauge.
// States transmited:
// 1 to 11: bar graph values from blinking 1 to 10
// 12:      security state, blink fast
// 13:      charging the bars fill from bottom to top
// 14:      failure the 10 bars blink simultaneouslty
//
#define STATE_TOO_LOW       0
#define STATE_EMPTY_CHARGE  1
#define STATE_1_CHARGE      2
#define STATE_2_CHARGE      3
#define STATE_3_CHARGE      4
#define STATE_4_CHARGE      5
#define STATE_5_CHARGE      6
#define STATE_6_CHARGE      7
#define STATE_7_CHARGE      8
#define STATE_8_CHARGE      9
#define STATE_9_CHARGE     10
#define STATE_10_CHARGE    11
//
#define STATE_SECURITY     12
#define STATE_CHARGING     13
#define STATE_FAILURE      14

void fill_vbat_arr(char *arr, long vbat);

