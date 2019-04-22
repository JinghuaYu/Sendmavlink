#include "mavlink.h"

int mavlink_parse(char *buffer, int size);
int mavlink_handle(mavlink_message_t *msg);
int handle_input_param_id(char *param_id, char *param_value);
