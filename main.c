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
sem_t gsema;
bool receive_mavlink_thread_running = false;
bool send_mavlink_thread_running = false;

int handle_mavlink_msg(mavlink_message_t *msg)
{
	if(MAVLINK_MSG_ID_HEARTBEAT == msg->msgid){
		printf("received MAVLINK_MSG_ID_HEARTBEA\n");
	}else{
		printf("other mavlink msg %d\n", msg->msgid);
	}

	return 1;
}


void *receive_mavlink_handle(void *arg)
{
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

	char buff[512];
	int len = sizeof(client_addr);
	mavlink_message_t msg;
	mavlink_status_t status;
	unsigned int index = 0;
	unsigned int buf_start = 0;
	while (receive_mavlink_thread_running){
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
				memset(buff, 0, 512);
				int recv_size = recvfrom(socketfd, buff, 511, 0, (struct sockaddr*)&client_addr, &len);
				if (recv_size > 0){
					printf("the receive_len is %d\n", recv_size);
					printf("client: %s:port:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
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
					//have received msg post the send mavlink msg
					sem_post(&gsema);
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
	int ret = mavlink_msg_command_long_pack(1, 1, &msg, 1, 1, 32774, 1, 1, 0, 0, 0, 0, 0, 0);
	printf("the mavlink_msg_command_long_pack ret is %d\n", ret);

	uint8_t buffer[256];
	memset(buffer, 0, 255);
	int buff_len = mavlink_msg_to_send_buffer(buffer, &msg);
	printf("the mavlink msg len is %d\n", buff_len);

	while(send_mavlink_thread_running){
		sem_wait(&gsema);
		if(!send_mavlink_thread_running) break;

		int send_size = sendto(socketfd, buffer, buff_len, 0, (struct sockaddr*)&client_addr,
			sizeof(struct sockaddr_in));
		if(send_size > 0) {
			printf("sendto size is %d\n", send_size);
		}
	}

	pthread_exit(0);
	return NULL;
}

void signal_handle(int argc)
{
	receive_mavlink_thread_running = false;

	send_mavlink_thread_running = false;
	sem_post(&gsema);

	close(socketfd);
	printf("close the socketfd\n");

	return;
}

int main(int argc, char **argv)
{
	signal(SIGINT, signal_handle);

	sem_init(&gsema, 0, 0);

	pthread_t receive_mavlink_thread;
	receive_mavlink_thread_running = true;
	int ret = pthread_create(&receive_mavlink_thread, NULL, receive_mavlink_handle, NULL);
	if(ret){
		printf("create receive_mavlink_thread failed\n");
		return -1;
	}

	pthread_t send_mavlink_thread;
	send_mavlink_thread_running = true;
	ret = pthread_create(&send_mavlink_thread, NULL, send_mavlink_handle, NULL);
	if(ret){
		printf("create send_mavlink_thread failed\n");
		return -1;
	}

	pthread_join(send_mavlink_thread, NULL);
}
