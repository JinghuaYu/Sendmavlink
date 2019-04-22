#include <stdint.h>
typedef enum
{
	ROLE_CLINE = 0,
	ROLE_SERVER = 1,
}ROLE;

int udp_open(ROLE role);
int udp_close(void);
int udp_send(void *buffer, int buff_len);