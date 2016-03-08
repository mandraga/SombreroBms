
void close_function(void *pdata);
void set_button(Fl_Button *pb, void* puser_data);
void cancel_callback(Fl_Widget *pwi, void *pdata);

#define MAXVBATRADIOB 18

typedef struct s_radio_VBat
{
  int          num;
  char         check_labels[8];
  void        *appdata;
}              t_radio_VBat;

typedef struct                  s_app_data
{
  std::string                   path;
  std::string                   ser_path; // Serial port file descriptor
  int                           xpos;
  int                           ypos;
  TTF_Font                     *pfont;
  Fl_Window                    *pwindow;
  //
  Fl_Check_Button              *pcheckbox_Vbat[MAXVBATRADIOB];
  t_radio_VBat                  radiodat[MAXVBATRADIOB];
  Fl_Button                    *but_ser_apply;
  Fl_Choice                    *serialport_choice;
  std::vector<std::string>      vserial_ports;
  // Info text
  Fl_Text_Buffer               *pinfo_buffer;
  // Setup
  Fl_Button                    *but_program_setup_date;
  Fl_Button                    *but_program_client_name;
  Fl_Button                    *but_program_serial;
  Fl_Button                    *but_program_capacityAH;
  Fl_Button                    *but_program_FullpackV;
  Fl_Button                    *but_program_minV;
  Fl_Button                    *but_program_maxV;
  Fl_Button                    *but_program_minT;
  Fl_Button                    *but_program_maxT;
  //
  Fl_Input                     *input_setup_date;
  Fl_Input                     *input_client_name;
  Fl_Input                     *input_serial;
  Fl_Int_Input                 *input_capacityAH;
  Fl_Float_Input               *input_FullpackV;
  Fl_Float_Input               *input_minV;
  Fl_Float_Input               *input_maxV;
  Fl_Int_Input                 *input_minT;
  Fl_Int_Input                 *input_maxT;
  //
  Fl_Tabs                      *tabs;
  GLBatWindow                  *pGLBatWindow;
  GLBatCurvWindow              *pGLBatCurvWindow;
  t_shared_data                 shared;
}                               t_app_data;

#define DATAPATH "./"
#define MAINFONT "VeraMono.ttf"
