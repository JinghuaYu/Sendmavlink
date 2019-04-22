#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mavlink_handle.h"
#include "udp.h"

int main(int argc, char **argv)
{
	udp_open(ROLE_SERVER);

	char input_buf[128] = {0};
	while(true) {
		printf("input: MSG_ID PARAM_ID [PARAM_VALUE]\n");
		char *pinput = fgets(input_buf, sizeof(input_buf), stdin);
		if (!strncmp(input_buf, "quit", 4)) {
			break;
		}

		if (isdigit(input_buf[0])) {
			int mavlink_msg_id = 0;
			if (sscanf(input_buf, "%d%*[^ ]", &mavlink_msg_id) != -1) {
				printf("the input msgid is %u\n", mavlink_msg_id);
				switch(mavlink_msg_id)
				{
					case MAVLINK_MSG_ID_PARAM_EXT_SET:
					{
						char param_id[16] = {0};
						char param_value[128] = {0};
						sscanf(input_buf, "%*[^ ]%s%*[ ]%s", param_id, param_value);
						handle_input_param_id(param_id, param_value);

						break;
					}

					case MAVLINK_MSG_ID_PARAM_EXT_REQUEST_READ:
					{
						char param_id[16] = {0};
						sscanf(input_buf, "%*[^ ]%s", param_id);
						break;
					}
				}
			}
		}
	}
	udp_close();
	return 0;
}
