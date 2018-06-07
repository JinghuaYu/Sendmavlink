#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "udp.h"

int socketfd;
struct sockaddr_in client_addr;

bool udp_thread_running = false;
pthread_t udp_thread;

void *udp_receive(void *arg)
{
	pthread_detach(pthread_self());

	int len = sizeof(struct sockaddr_in);
	while (udp_thread_running) {
		if(!udp_thread_running)
			break;
		fd_set read_fdset;
		FD_ZERO(&read_fdset);
		FD_SET(socketfd, &read_fdset);
		struct timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		int select_fd = select(socketfd + 1, &read_fdset, NULL, NULL, &timeout);
		if (select_fd > 0) {
			if (FD_ISSET(socketfd, &read_fdset)) {
				char buff[512];
				memset(buff, 0, 512);
				int recv_size = recvfrom(socketfd, buff, 512, 0, (struct sockaddr*)&client_addr, &len);
				if (recv_size > 0){
					printf("the receive_len is %d\n", recv_size);
					printf("client: %s:port:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					mavlink_parse(buff, recv_size);
				}
			}
		}
	} //while receive_mavlink_thread_running


	printf("close the socketfd\n");

	pthread_exit(NULL);
	return NULL;
}

int udp_send(void *buffer, int buff_len)
{
	if (NULL == buffer) {
		return -1;
	}

	int send_size = sendto(socketfd, buffer, buff_len, 0,
							(struct sockaddr*)&client_addr,
							sizeof(struct sockaddr_in));
	printf("sendto size is %d\n", send_size);
}

int udp_open(void)
{
 	socketfd = socket(AF_INET,SOCK_DGRAM,0);
	if(socketfd < 0) {
		printf("create socketfd failed\n");
		return -1;
	}
	struct sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(14550);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(socketfd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr_in));
	if (ret < 0){
		printf("bind socketfd failed\n");
		return -1;
	}

	udp_thread_running = true;
	ret = pthread_create(&udp_thread,
							 NULL,
							 udp_receive,
							 &udp_thread_running);
	if(ret) {
		printf("create udp_thread failed\n");
		return -1;
	}

	return 0;
}

void udp_close(void)
{
	udp_thread_running = false;
	pthread_join(udp_thread, NULL);
	close(socketfd);
}
