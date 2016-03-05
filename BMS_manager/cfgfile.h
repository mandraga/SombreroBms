
#define CONFIG_FILE_NAME "cfg.xml"

class Cxml_cfg_file_decoder
{
 public:
  Cxml_cfg_file_decoder();
  ~Cxml_cfg_file_decoder();

 public:
  // Path
  std::string get_user_config_path();
  std::string get_user_documents_path();

  // Reading
  bool open_for_reading(std::string file_name);
  bool read_window_position(int *x, int *y);
  bool read_window_size(int *x, int *y);
  bool read_serial_port_name(std::string *ser_name);
  bool close();

  // Writing
  bool open_for_writing();
  bool write(std::string file_name);

  void write_window_position(int x, int y);
  void write_window_size(int x, int y);
  void write_serial_port_name(std::string ser_name);

 private:
  bool get_int_attribute(TiXmlNode* pnode, std::string name, int *pvalue);
  bool get_string_attribute(TiXmlNode* pnode, std::string name, std::string *pvalue);
  std::string get_user_home();

 private:
  TiXmlDocument *m_pdoc;
};


