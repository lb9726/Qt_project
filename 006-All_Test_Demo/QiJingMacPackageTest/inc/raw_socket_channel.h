#ifndef __RAW_SOCKET_CHANNEL_H__
#define __RAW_SOCKET_CHANNEL_H__

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netpacket/packet.h>

#define RS_USED_ETHDEV      "eth0"
#define RS_MTYPE            0x8888

#define RS_FOREVER          (unsigned int)(-1)
#define RS_RW_TIMEOUT       300 

typedef struct __packed ethernet_std 
{
	uint8_t     ODA[ETHER_ADDR_LEN];
	uint8_t     OSA[ETHER_ADDR_LEN];
	uint16_t    MTYPE;
} ethernet_std_t;

typedef struct
{
	int             fd;
	char const      *ifname;
	uint8_t         peer[ETHER_ADDR_LEN];
	uint8_t         host[ETHER_ADDR_LEN];
	uint16_t        type;
    int             timeout;
} raw_socket_channel_t;

int  db_open_channel(raw_socket_channel_t *pch);
int  db_close_channel(raw_socket_channel_t *pch);
int  db_read_packet(raw_socket_channel_t *pch, uint8_t *buff, int len);
int  db_send_packet(raw_socket_channel_t *pch, uint8_t *buff, int len);
int  db_check_if_channel_inited(void);

#endif /* __RAW_SOCKET_CHANNEL_H__ */

