#include "inc/db_types.h"
#include "inc/raw_socket_channel.h"

extern raw_socket_channel_t gv_raw_sock_channel;

static uint8_t  sv_msg[] =
{
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 
    0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 
    0x88, 0x88,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
    0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
}; 

uint8_t recv_mac[ETHER_ADDR_LEN] =
{
    0x74, 0x46, 0xa0, 0xb4, 0xfa, 0x7b
}; 

int main(int argc, char **argv)
{
    raw_socket_channel_t    *pch;
    char                    nic_name[100] = "eth0";	
    int                     mode = 0;
    uint8_t                 buff[100];
    int                     len = 0;
    int                     loop = 0;
    int                     i = 0;
    int                     iret = 0;
    
	if (argc != 3)
	{
	    printf("\nUsage: %s c|s eth_name\n\n", argv[0]);
	    return 1;
	}

    if (!memcmp(argv[1], "c", strlen("c")))
    {
        mode = 0;
    }
    else
    {
        mode = 1;
    }
    
    pch = &gv_raw_sock_channel;
    strcpy(nic_name, argv[2]);
    pch->ifname = nic_name;

    iret = db_open_channel(pch);
    if (iret != 0)
    {
        return 1;
    }

    while (1)
    {
        if (!mode)
        {
            len = sizeof(sv_msg);
            memcpy(buff, sv_msg, len);
            if (loop % 2)
            {
                // change peer's mac
                memcpy(buff, recv_mac, ETHER_ADDR_LEN);
            }
            db_send_packet(pch, buff, len);
            printf("loop = %d, send %d bytes ...\n", loop++, len);
        }
        else
        {
            len = db_read_packet(pch, buff, 1024);
            if (len > 0)
            {
                printf("loop = %d, received %d bytes, buff =\n\t", loop ++, len);
                for (i = 0; i < len; i++)
                {
                    if (i && !(i % 16))
                    {
                        printf("\n\t");
                    }
                    printf("0x%02x ", buff[i]);
                }
                printf("\n");
                
                ethernet_std_t *eth_hdr;
                if ((unsigned)len > sizeof(ethernet_std_t))
                {
                    eth_hdr = (ethernet_std_t *)buff; 
                    if (!memcmp(eth_hdr->ODA, recv_mac, ETHER_ADDR_LEN))
                    {
                        printf("Ah, this msg is dedicated for me ...\n");
                    }
                    else
                    {
                        printf("No, this msg is not for me ...\n");
                    }
                }
                printf("\n");
            }
        }        
        sleep(5);
    }

    db_close_channel(pch);
    
    return 0;
}

/* EOF */
