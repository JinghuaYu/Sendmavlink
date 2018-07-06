#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <linux/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "mavlink.h"

int socketfd;
struct sockaddr_in client_addr;
sem_t g_send_sema;
sem_t g_read_sema;
bool receive_mavlink_thread_running = false;
bool send_mavlink_thread_running = false;

static void data_dump(uint8_t *buf_name, uint8_t *buffer, int input_len)
{
	if (!buf_name || !buffer) {
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

int handle_mavlink_msg(mavlink_message_t *msg)
{
	char rssi;
	switch(msg->msgid) {
		case MAVLINK_MSG_ID_HEARTBEAT:
			data_dump((uint8_t *)"heartbeat", (uint8_t *)msg, 21);
			printf("received MAVLINK_MSG_ID_HEARTBEA\n");
			break;
		case MAVLINK_MSG_ID_RADIO_STATUS:
			printf("received MAVLINK_MSG_ID_RADIO_STATUS\n");
			data_dump((uint8_t *)"radio_status", (uint8_t *)msg, 18);
			rssi = (char)mavlink_msg_radio_status_get_remrssi(msg);
			printf("rssi %d\n", rssi);
			break;
		default:
			printf("received other mavlink message id %d\n", msg->msgid);
			break;
	}

	return 1;
}


void *receive_mavlink_handle(void *arg)
{
	pthread_detach(pthread_self());
 	socketfd = socket(AF_INET,SOCK_DGRAM,0);
	if(socketfd < 0){
		printf("create socketfd failed\n");
		return NULL;
	}
	struct sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(14550);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socketfd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if (ret < 0){
		printf("bind socketfd failed\n");
		return NULL;
	}

	int len = sizeof(client_addr);
	mavlink_message_t msg;
	mavlink_status_t status;
	unsigned int index = 0;
	unsigned int buf_start = 0;
	while (receive_mavlink_thread_running){
		if(!receive_mavlink_thread_running)
			break;
		fd_set read_fdset;
		FD_ZERO(&read_fdset);
		FD_SET(socketfd, &read_fdset);
		struct timeval timeout;
		timeout.tv_sec = 15;
		timeout.tv_usec = 0;
		int select_fd = select(socketfd + 1, &read_fdset, NULL, NULL, &timeout);
		if (select_fd <= 0) {
			printf("%s: select error\n", __func__);
		}
		if (select_fd > 0) {
			if (FD_ISSET(socketfd, &read_fdset)) {
				uint8_t buff[128];
				memset(buff, 0, 128);
				int recv_size = recvfrom(socketfd, buff, 128, 0, (struct sockaddr*)&client_addr, &len);
				if (recv_size > 0){
					printf("the receive_len is %d\n", recv_size);
					printf("client: %s:port:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					data_dump((uint8_t *)"received data", buff, recv_size);
					index = 0;
					buf_start = 0;
					memset(&msg, 0, sizeof(mavlink_message_t));
					while (index < recv_size) {
						ret = mavlink_parse_char(MAVLINK_COMM_0, buff[index], &msg, &status);
						if (MAVLINK_FRAMING_BAD_CRC == ret) {
							printf("Got MAVLINK_FRAMING_BAD_CRC return while parsing mavlink!");
							++index;
							continue;
						}

						if (MAVLINK_FRAMING_BAD_SIGNATURE == ret) {
							printf("Got MAVLINK_FRAMING_BAD_SIGNATURE return while parsing mavlink!");
							++index;
							continue;
						}

						if (MAVLINK_FRAMING_OK != ret) {
							++index;
							continue;
						}
						// If handleMavlinkMsg returns false, that means this message should still
						// be forwarded to other components; otherwise not.
						ret = handle_mavlink_msg(&msg);

						++index;
							// Update the buffer start index after the index is increased.
						buf_start = index;
					} // while-loop
				}
			}
		}
	} //while receive_mavlink_thread_running

	pthread_exit(NULL);
	return NULL;
}

void *send_mavlink_handle(void *arg)
{
	mavlink_message_t msg;
	bool open_rtsp_fc = *(bool *)arg;
	int mav_size;

	if (open_rtsp_fc) {
		printf("open_rtsp_fc true\n");
		mav_size = mavlink_msg_command_long_pack(1, 1, &msg, 1, MAV_COMP_ID_CAMERA,
												32774, 1, 1, 0, 0, 0, 0, 0, 0);
	} else {
		printf("open_rtsp_fc false\n");
		mav_size = mavlink_msg_command_long_pack(1, 1, &msg, 1, MAV_COMP_ID_CAMERA,
												32774, 1, 0, 0, 0, 0, 0, 0, 0);
	}


	printf("the mavlink_msg_command_long_pack mav_size is %d\n", mav_size);
	mavlink_command_long_t command_long_packet;
	memcpy(&command_long_packet, msg.payload64, sizeof(mavlink_command_long_t));
	printf("msgid %d, command %d, param1 %d\n", msg.msgid,
			command_long_packet.command, (int)command_long_packet.param1);

	uint8_t buffer[256];
	memset(buffer, 0, 255);
	int buff_len = mavlink_msg_to_send_buffer(buffer, &msg);
	printf("the mavlink msg len is %d\n", buff_len);

	bool enable_rtsp = false;
	while(send_mavlink_thread_running){
		sem_wait(&g_send_sema);
		if(!send_mavlink_thread_running) break;
		if (enable_rtsp){
			int send_size = sendto(socketfd, buffer, buff_len, 0,
									(struct sockaddr*)&client_addr,
									sizeof(struct sockaddr_in));
			if(send_size > 0) {
				printf("sendto size is %d\n", send_size);
				enable_rtsp = true;
			}
		}
	}

	pthread_exit(0);
	return NULL;
}

void signal_handle(int argc)
{
	receive_mavlink_thread_running = false;

	send_mavlink_thread_running = false;
	sem_post(&g_send_sema);

	close(socketfd);
	printf("close the socketfd\n");

	return;
}

int main(int argc, uint8_t **argv)
{
	signal(SIGINT, signal_handle);

	bool open_rtsp_fc = false;
	if (argc == 2) {
		if(!strcmp("open_rtsp_fc", argv[1])) {
			open_rtsp_fc = true;
		}
		if (!strcmp("close_rtsp_fc", argv[1])) {
			open_rtsp_fc = false;
		}
	} else {
		printf("You can use as below:\n");
		printf("send_mavlink open_rtsp_fc\n");
		printf("send_mavlink close_rtsp_fc\n");
		return -1;
	}

	sem_init(&g_send_sema, 0, 0);
	sem_init(&g_read_sema, 0, 0);

	pthread_t receive_mavlink_thread;
	receive_mavlink_thread_running = true;
	int ret = pthread_create(&receive_mavlink_thread, NULL, receive_mavlink_handle, NULL);
	if(ret){
		printf("create receive_mavlink_thread failed\n");
		return -1;
	}

	pthread_t send_mavlink_thread;
	send_mavlink_thread_running = true;
	ret = pthread_create(&send_mavlink_thread, NULL, send_mavlink_handle, &open_rtsp_fc);
	if(ret){
		printf("create send_mavlink_thread failed\n");
		return -1;
	}

	pthread_join(send_mavlink_thread, NULL);
}
