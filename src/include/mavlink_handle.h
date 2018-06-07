#include "mavlink.h"

int mavlink_parse(char *buffer, int size);
int mavlink_handle(mavlink_message_t *msg);
int handle_input_command(char *param_id, char *param_value);
