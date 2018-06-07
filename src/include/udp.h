#include <stdint.h>

int udp_open(void);
void udp_close(void);
int udp_send(void *buffer, int buff_len);