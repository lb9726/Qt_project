#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#ifndef AF_CAN
#define AF_CAN 29
#endif
#ifndef PF_CAN
#define PF_CAN AF_CAN
#endif
static void print_frame(struct can_frame *fr)
{
    int i;
    printf("recv: can.id=0x%08x ", fr->can_id & CAN_EFF_MASK);
    //printf("%08x\n", fr->can_id);
    printf("dlc = %d ", fr->can_dlc);
    printf("data = ");
    for (i = 0; i < fr->can_dlc; i++)
        printf("%02x ", fr->data[i]);
    printf("\n");
}

#define errout(_s) fprintf(stderr, "error class: %s\n", (_s))
#define errcode(_d) fprintf(stderr, "error code: %02x\n", (_d))

static void handle_err_frame(const struct can_frame *fr)
{
    if (fr->can_id & CAN_ERR_TX_TIMEOUT) {
        errout("CAN_ERR_TX_TIMEOUT");
    }
    if (fr->can_id & CAN_ERR_LOSTARB) {
        errout("CAN_ERR_LOSTARB");
        errcode(fr->data[0]);
    }
    if (fr->can_id & CAN_ERR_CRTL) {
        errout("CAN_ERR_CRTL");
        errcode(fr->data[1]);
    }
    if (fr->can_id & CAN_ERR_PROT) {
        errout("CAN_ERR_PROT");
        errcode(fr->data[2]);
        errcode(fr->data[3]);
    }
    if (fr->can_id & CAN_ERR_TRX) {
        errout("CAN_ERR_TRX");
        errcode(fr->data[4]);
    }
    if (fr->can_id & CAN_ERR_ACK) {
        errout("CAN_ERR_ACK");
    }
    if (fr->can_id & CAN_ERR_BUSOFF) {
        errout("CAN_ERR_BUSOFF");
    }
    if (fr->can_id & CAN_ERR_BUSERROR) {
        errout("CAN_ERR_BUSERROR");
    }
    if (fr->can_id & CAN_ERR_RESTARTED) {
        errout("CAN_ERR_RESTARTED");
    }
}
#define myerr(str)  fprintf(stderr, "%s, %s, %d: %s\n", __FILE__, __func__, __LINE__, str)
static int test_can_rw(int fd, int master)
{
    int ret, i;
    struct can_frame fr, frdup;
    struct timeval tv;
    fd_set rset;
    while (1) {
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        FD_ZERO(&rset);
        FD_SET(fd, &rset);
        ret = select(fd+1, &rset, NULL, NULL, NULL);
        if (ret == 0) {
            myerr("select time out");
            return -1;
        }
        /* select 调用无错返回时，表示有符合规则的数据帧到达  */
        ret = read(fd, &frdup, sizeof(frdup));
        if (ret < sizeof(frdup)) {
            myerr("read failed");
            return -1;
        }
        if (frdup.can_id & CAN_ERR_FLAG) { /* 检查数据帧是否错误 */
            handle_err_frame(&frdup);
            myerr("CAN device error");
            continue;
        }
        print_frame(&frdup); /* 打印数据帧信息  */
        ret = write(fd, &frdup, sizeof(frdup));  /* 把接收到的数据帧发送出去 */
        if (ret < 0) {
            myerr("write failed");
            return -1;
        }
    }
    return 0;
}
int main(int argc, char *argv[])
{
    int s;
    int ret;
    struct sockaddr_can addr;
    struct ifreq ifr;
    int master;
    srand(time(NULL));
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);  /* 创建套接字 */
    if (s < 0) {
        perror("socket PF_CAN failed");
        return 1;
    }
    /* 把套接字绑定到 can0 接口  */
    strcpy(ifr.ifr_name, "can0");
    ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
        perror("ioctl failed");
        return 1;
    }
    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(s, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return 1;
    }
    /* 设置过滤规则 */
    if (0) {
        struct can_filter filter[2];
        /* 第 1 个规则是可以接收 ID 为 0x200 & 0xFFF 的数据帧 */
        filter[0].can_id = 0x200 | CAN_EFF_FLAG;
        filter[0].can_mask = 0xFFF;
        /* 第 2 个规则是可以接收 ID 为 0x20F& 0xFFF 的数据帧 */
        filter[1].can_id = 0x20F | CAN_EFF_FLAG;
        filter[1].can_mask = 0xFFF;
        /* 启用过滤规则，只要 CAN0 接收到的数据帧满足上面 2 个规则中的任何一个也被接受*/
        ret = setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &filter, sizeof(filter));
        if (ret < 0) {
            perror("setsockopt failed");
            return 1;
        }
    }
    test_can_rw(s, master);  /*进入测试*/
    close(s);
    return 0;
}
