#include "inc/db_types.h"
#include "inc/raw_socket_channel.h"

static int inited = 0;

raw_socket_channel_t gv_raw_sock_channel = 
{
    -1,
    RS_USED_ETHDEV,    
    { 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    RS_MTYPE,
    RS_RW_TIMEOUT
};

int db_open_channel(raw_socket_channel_t *pch)
{
    struct ifreq ifreq;
    struct sockaddr_ll sockaddr_ll = 
    {    
        PF_PACKET,
        0x0000,
        0x0000,
        ARPHRD_ETHER,
        PACKET_OTHERHOST,
        ETHER_ADDR_LEN,
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
    };

	if (geteuid ())
	{
	    printf("You must have root privileges to open raw socket !\n\n");
		return -1;
	}
	    
    memset (&ifreq, 0, sizeof (ifreq));
    sockaddr_ll.sll_protocol = htons (pch->type);
    
    if ((pch->fd = socket (sockaddr_ll.sll_family, SOCK_RAW, sockaddr_ll.sll_protocol)) == -1) 
    {    
        printf("%s : Can't open %s\n", __func__, pch->ifname);
        pch->fd = -1;
        return -1;
    }
    
    memcpy (ifreq.ifr_name, pch->ifname, sizeof (ifreq.ifr_name));
    if (ioctl (pch->fd, SIOCGIFINDEX, &ifreq) == -1) 
    {    
        printf("%s : Can't read index: %s\n", __func__, ifreq.ifr_name);
        close (pch->fd);
        pch->fd = -1;
        return -2;
    }
    
    sockaddr_ll.sll_ifindex = ifreq.ifr_ifindex;
    if (ioctl (pch->fd, SIOCGIFHWADDR, &ifreq) == -1) 
    {    
        printf("%s : Can't read address: %s\n", __func__, ifreq.ifr_name);
        close (pch->fd);
        pch->fd = -1;
        return -3;
    }
    
    memcpy (sockaddr_ll.sll_addr, ifreq.ifr_ifru.ifru_hwaddr.sa_data, sizeof (sockaddr_ll.sll_addr));
    if (bind (pch->fd, (struct sockaddr *) (&sockaddr_ll), sizeof (sockaddr_ll)) == -1) 
    {    
        printf("%s : Can't bind socket: %s\n", __func__, ifreq.ifr_name);
        close (pch->fd);
        pch->fd = -1;
        return -4;
    }
    
    memcpy (pch->host, sockaddr_ll.sll_addr, sizeof (pch->host));
    if (ioctl (pch->fd, SIOCGIFFLAGS, &ifreq) == -1) 
    {
        printf("%s : Can't read state: %s\n", __func__, ifreq.ifr_name);
        close (pch->fd);
        pch->fd = -1;
        return -5;
    }
    
    ifreq.ifr_flags |= (IFF_UP | IFF_BROADCAST | IFF_MULTICAST | IFF_PROMISC);
    ifreq.ifr_flags &= ~ (IFF_ALLMULTI);
    
    if (ioctl (pch->fd, SIOCSIFFLAGS, &ifreq) == -1) 
    {
        printf("%s : Can't save state: %s\n", __func__, ifreq.ifr_name);
        close (pch->fd);
        pch->fd = -1;
        return -6;
    }
    
    inited = 1;
    
    return (0);
}

int db_close_channel(raw_socket_channel_t *pch) 
{
    inited = 0;
    return (close (pch->fd));
}

int db_read_packet(raw_socket_channel_t *pch, uint8_t *buff, int len) 
{
    struct pollfd pollfd = 
    {    
        pch->fd,
        POLLIN,
        0
    };

    int status = poll (&pollfd, 1, pch->timeout);
    memset (buff, 0, len);
    if (status > 0) 
    {    
        len = recvfrom (pch->fd, buff, len, 0, (struct sockaddr *) (0), (socklen_t *)(0));

        return (len);
    }
    
    return (status);
}

int db_send_packet(raw_socket_channel_t *pch, uint8_t *buff, int len) 
{
    if(pch->fd == -1)
    {
        return 0;
    }

    len = sendto (pch->fd, buff, len, 0, (struct sockaddr *) (0), (socklen_t) (0));

    return (len);
}

int db_check_if_channel_inited(void)
{
	return inited;
}

/* EOF */
