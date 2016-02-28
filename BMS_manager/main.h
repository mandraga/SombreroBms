
void close_function(void *pdata);
void set_button(Fl_Button *pb, void* puser_data);
void cancel_callback(Fl_Widget *pwi, void *pdata);

typedef struct                  s_app_data
{
  std::string                   path;
  int                           xpos;
  int                           ypos;
  Fl_Window                    *pwindow;
  //Fl_Check_Button              *pcheckbox_record;
  //Fl_Check_Button              *pcheckbox_do_not;
  //Fl_Button                    *but_cancel;
  Fl_Button                    *but_ser_appply;
  Fl_Choice                    *serial_port_choice;
  std::vector<std::string>      vserial_ports;
  //Fl_Choice                    *input_device_choice;
  //Fl_Choice                    *output_device_choice;
  t_shared_data                 shared;
}                               t_app_data;

