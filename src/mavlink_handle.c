#include "mavlink_handle.h"
#include "wireless_interface.h"

static void data_dump(char *buf_name, uint8_t *buffer, int input_len)
{
	if (NULL == buf_name || NULL == buffer) {
		printf("the buf_name or buffer is NULL\n");
		return;
	}
	printf("The %s input_len %u: dump as below:\n", buf_name, input_len);
	for (int i = 0; i < input_len; i++) {
		printf("0x%02X ", buffer[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}

	printf("\n");
}


int mavlink_handle(mavlink_message_t *msg)
{
	switch(msg->msgid) {
		case MAVLINK_MSG_ID_HEARTBEAT:
			printf("received MAVLINK_MSG_ID_HEARTBEA\n");
			break;
		case MAVLINK_MSG_ID_PARAM_EXT_ACK:
		{
			printf("received MAVLINK_MSG_ID_PARAM_EXT_ACK\n");
			uint8_t param_result;
			mavlink_param_ext_ack_t mavlink_param_ext_ack;
			mavlink_msg_param_ext_ack_decode(msg, &mavlink_param_ext_ack);
			param_result = mavlink_param_ext_ack.param_result;
			printf("param_result: %d\n", param_result);
			if (!strcmp(mavlink_param_ext_ack.param_id, WLS_SCAN)) {

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_CONNECT)) {
				printf("received WLS_CONNECT msg\n");

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_DISCONNECT)) {
				printf("received WLS_DISCONNECT msg\n");

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_SELECTNETID)) {
				printf("received WLS_SELECTNETID msg\n");

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_SETFRQCHAN)) {
				printf("received WLS_SETFRQCHAN msg\n");

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_SETBANDWIDTH)) {
				printf("received WLS_SETBANDWIDTH msg\n");

			} else if (!strcmp(mavlink_param_ext_ack.param_id, WLS_SETDATARATE)) {
				printf("received WLS_SETDATARATE msg\n");

			} else {
				printf("received: %s\n", mavlink_param_ext_ack.param_id);
				printf("received: param_value: %s\n", mavlink_param_ext_ack.param_value);
			}

			break;
		}

		case MAVLINK_MSG_ID_PARAM_EXT_VALUE:
		{
			mavlink_param_ext_value_t mavlink_param_ext_value;
			printf("received: param_value: %s\n", mavlink_param_ext_value.param_value);

			break;
		}
		default:
			printf("received other mavlink message id %d\n", msg->msgid);
			break;
	}

	return 0;
}

int mavlink_parse(char *buffer, int size)
{
	int	index = 0;
	mavlink_message_t mavlink_message;
	mavlink_status_t mavlink_status;
	memset(&mavlink_message, 0, sizeof(mavlink_message_t));
	memset(&mavlink_status, 0, sizeof(mavlink_status_t));
	while (index < size) {
		int ret = mavlink_parse_char(MAVLINK_COMM_0, buffer[index], &mavlink_message, &mavlink_status);
		if (MAVLINK_FRAMING_BAD_CRC == ret) {
			++index;
			continue;
		}

		if (MAVLINK_FRAMING_BAD_SIGNATURE == ret) {
			++index;
			continue;
		}

		if (MAVLINK_FRAMING_OK != ret) {
			++index;
			continue;
		}
		ret = mavlink_handle(&mavlink_message);

		++index;
	} // while-loop

	return 0;
}

int mavlink_send(uint8_t mavlink_msg_id, void *msg_buf)
{
	if (NULL == msg_buf) {
		return -1;
	}

	uint16_t mav_size = 0;
	mavlink_message_t mavlink_message;
	memset(&mavlink_message, 0, sizeof(mavlink_message_t));
	switch(mavlink_msg_id)
	{
		case MAVLINK_MSG_ID_PARAM_EXT_SET:
		{
			mavlink_param_ext_set_t *pmavlink_param_ext_set;
			pmavlink_param_ext_set = (mavlink_param_ext_set_t*)msg_buf;
			printf("set param_id :%s param_value: %s\n",
				   pmavlink_param_ext_set->param_id,
				   pmavlink_param_ext_set->param_value);

			mav_size = mavlink_msg_param_ext_set_pack(1, 190, &mavlink_message, 1, 89,
												pmavlink_param_ext_set->param_id,
												pmavlink_param_ext_set->param_value, 0);
			break;
		}

		case MAVLINK_MSG_ID_PARAM_EXT_REQUEST_READ:
		{
			mavlink_param_ext_request_read_t *pmavlink_param_ext_request_read;
			pmavlink_param_ext_request_read = (mavlink_param_ext_request_read_t*)msg_buf;
			printf("read param_id :%s\n", pmavlink_param_ext_request_read->param_id);
			mav_size = mavlink_msg_param_ext_request_read_pack(1, 190, &mavlink_message,
                               1, 89, pmavlink_param_ext_request_read->param_id, -1);
			break;
		}
		default:
			break;
	}
	printf("the mav_size is %d\n", mav_size);
	uint8_t buffer[256];
	memset(buffer, 0, 255);
	int buff_len = mavlink_msg_to_send_buffer(buffer, &mavlink_message);
	printf("the mavlink msg len is %d\n", buff_len);
	udp_send(buffer, buff_len);
	return 0;
}

int handle_input_param_id(char *param_id, char *param_value)
{
	mavlink_param_ext_set_t mavlink_param_ext_set;
	memset(&mavlink_param_ext_set, 0, sizeof(mavlink_param_ext_set_t));

	if (!strcmp(param_id, WLS_SCAN)) {
		printf("input WLS_SCAN msg\n");

	} else if (!strcmp(param_id, WLS_CONNECT)) {
		printf("input WLS_CONNECT msg\n");

	} else if (!strcmp(param_id, WLS_DISCONNECT)) {
		printf("input WLS_DISCONNECT msg\n");

	} else if (!strcmp(param_id, WLS_SELECTNETID)) {
		printf("input WLS_SELECTNETID msg\n");

	} else if (!strcmp(param_id, WLS_SETFRQCHAN)) {
		printf("input WLS_SETFRQCHAN msg\n");

	} else if (!strcmp(param_id, WLS_SETBANDWIDTH)) {
		printf("input WLS_SETBANDWIDTH msg\n");

	} else if (!strcmp(param_id, WLS_SETDATARATE)) {
		printf("input WLS_SETDATARATE msg\n");

	} else {
		printf("input: %s\n", (char *)param_id);
	}

	memcpy(mavlink_param_ext_set.param_id, param_id, 16);
	memcpy(mavlink_param_ext_set.param_value, param_value, 128);
	mavlink_send(MAVLINK_MSG_ID_PARAM_EXT_SET, &mavlink_param_ext_set);

	return 1;
}
