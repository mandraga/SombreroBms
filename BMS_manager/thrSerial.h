

void start_serial_thread(t_shared_data *pshared);
void stop_serial_thread(t_shared_data *pshared);

void add_command_in_locked_area(eserial_command type, const char *command, t_shared_data *pshared);
