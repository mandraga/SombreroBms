
#define SETUP_DATE_SIZE   16
#define SERNUM_SIZE       10
#define CLIENT_STR_SIZE   33
#define UPTIME_STR_SIZE   64
#define STATE_STR_SIZE    32
#define MAX_BAT_ELEMENTS 256

typedef struct s_params
{
  char         setupdate[SETUP_DATE_SIZE];
  int          charge_percent;
  int          total_capacity;
  float        Vbat;
  int          BatElements;
  int          ChargeCycles;
  int          ChargeTimeTotal;
  float        EltVmin;
  float        EltVmax;
  float        MaxVbat;
  int          undervoltageEvts;
  int          maxdischarge;
  int          avgdischarge;
  char         serialnumber[SERNUM_SIZE];
  char         client[CLIENT_STR_SIZE];
  char         uptime[UPTIME_STR_SIZE];
  int          tmin;
  int          tmax;
  int          temperature;
}              t_params;

typedef struct s_batreport
{
  float        Velement;
  float        Vlowest;
  int          events;
  int          average_charging_time;
  bool         balancing;
}              t_batreport;

typedef struct s_report
{
  float        Vbat;
  int          charge_percent;
  float        chargeAH;
  int          currentmA;
  char         state[STATE_STR_SIZE];
  int          elements;
  int          temperature;
  t_batreport  element_array[MAX_BAT_ELEMENTS]; // Ready for huge stacks
}              t_report;

typedef struct s_batmeasurement
{
  float        Vbat[MAX_BAT_ELEMENTS];
}              t_batmeasurement;

class CSombreroBMS
{
 public:
  CSombreroBMS();
  ~CSombreroBMS();

 public:

  void parse_BMS_params_string(t_params *pparams, char *string);
  void parse_BMS_report_string(t_report *preport, char *string);

  void set_param_int(char *valuename, int value, char *str, int strsz);
  void set_param_str(char *valuename, char *pvalue, char *str, int strsz);

  bool is_equal(char *code, const char *value);

  void add_history(t_batmeasurement *pm);
  int  get_history_sz();
  t_batmeasurement *get_hitory_elt(int elt);

 private:
  bool get_next_line(char **pdata, int size, char *pcode, char *pvalue);

 public:
  t_report m_report;
  t_params m_params;
  bool     m_checked[MAX_BAT_ELEMENTS];
 private:
  std::vector<t_batmeasurement> m_Vbat_history;
  t_batmeasurement m_tmpmeasure;
};


