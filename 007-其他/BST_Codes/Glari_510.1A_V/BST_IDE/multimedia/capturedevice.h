#ifndef CAPTUREDEVICE_H
#define CAPTUREDEVICE_H

#include "global.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>
#include <linux/videodev2.h>

class CaptureDevice : public QObject
{
    Q_OBJECT
public:
    typedef struct{
        void * start;
        size_t length;
    } buffer;
public:
    CaptureDevice(QString dev_name, QObject *pParent=0);
    ~CaptureDevice();

    int open_device();
    int close_device();
    int init_device();
    int start_capturing();
    int stop_capturing();
    int uninit_device();
    int get_frame(void **, size_t*);
    int unget_frame();

    QPixmap get_pixmap(QSize pSize=QSize());

    int init_mmap();

    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

public:
    QString dev_name;
    int fd;
    buffer* buffers;
    unsigned int n_buffers;
    int index;
    v4l2_buf_type      m_type;
    int width, height;

signals:
    void display_error(QString);
};

inline int CaptureDevice::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
    unsigned int pixel32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel32;
    int r, g, b;
    r = y + (1.370705 * (v-128));
    g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
    b = y + (1.732446 * (u-128));
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;
    pixel[0] = r * 220 / 256;
    pixel[1] = g * 220 / 256;
    pixel[2] = b * 220 / 256;
    return pixel32;
}

#endif // CAPTUREDEVICE_H
