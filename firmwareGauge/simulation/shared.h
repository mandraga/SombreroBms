
typedef struct   s_internal_message
{
  int            message_code;
  void          *passociated_event;
  int            size;
  char           data[MESSAGE_CONTENT_SIZE];
}                t_internal_message;

typedef struct                  s_shared_data
{
  pthread_mutex_t               data_mutex;
  CTCP_Client                  *ptcpclient;
  pthread_t                     threadclient;
  std::list<t_internal_message> network2app_message_list;
  bool                          bconnected;
  bool                          bquit;
}                               t_shared_data;

