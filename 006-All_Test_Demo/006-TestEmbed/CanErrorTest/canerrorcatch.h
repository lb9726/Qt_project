#ifndef CANERRORCATCH_H
#define CANERRORCATCH_H

#include <QObject>
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

#define errout(_s) fprintf(stderr, "error class: %s\n", (_s))
#define errcode(_d) fprintf(stderr, "error code: %02x\n", (_d))
#define myerr(str)  fprintf(stderr, "%s, %s, %d: %s\n", __FILE__, __func__, __LINE__, str)

#ifndef AF_CAN
#define AF_CAN 29
#endif
#ifndef PF_CAN
#define PF_CAN AF_CAN
#endif

class CanErrorCatch : public QObject
{
    Q_OBJECT
public:
    explicit CanErrorCatch(QObject *parent = 0);
    static void print_frame(struct can_frame *fr);
    static void handle_err_frame(const struct can_frame *fr);
    static int test_can_rw(int fd, int master);
    void initCatch();
signals:

public slots:
};

#endif // CANERRORCATCH_H
