
#define MAX_MSG_SIZE 4096

enum eserial_command
  {
    enothing,
    eping,
    einformation,
    ereport,
    eprogram,
    eupdategui
  };

typedef struct    s_serial_request
{
  eserial_command type;
  char            command[MAX_MSG_SIZE];
}                 t_serial_request;

//---------------------------------------------------------------------

enum etab
  {
    tabunknown,
    tabport,
    tababout,
    tabsetup,
    tabcharge,
    tabparam,
    tabhist
  };

//---------------------------------------------------------------------
// Shared data
//---------------------------------------------------------------------

typedef struct                s_shared_data
{
  bool                        bquit;
  std::string                 serial_port;
  CSerialPort                *pserial;
  pthread_t                   threadserial;
  pthread_mutex_t             datamutex;
  std::list<t_serial_request> request;
  bool                        brequest_ready;
  char                        bms_version[MAX_MSG_SIZE];
  char                        param_msg[MAX_MSG_SIZE];
  char                        report_msg[MAX_MSG_SIZE];
  CSombreroBMS               *pBMS;
  etab                        last_tab;
}                             t_shared_data;

//#define INSPECT_INTERLOCK
#ifdef INSPECT_INTERLOCK
#define LOCK   printf("-> Locking %s %d\n", __FILE__, __LINE__); pthread_mutex_lock(&pshared->datamutex);
#define UNLOCK pthread_mutex_unlock(&pshared->datamutex); printf("<- Unlocking %s %d\n", __FILE__, __LINE__);
#else
#define LOCK   pthread_mutex_lock(&pshared->datamutex);
#define UNLOCK pthread_mutex_unlock(&pshared->datamutex);
#endif

