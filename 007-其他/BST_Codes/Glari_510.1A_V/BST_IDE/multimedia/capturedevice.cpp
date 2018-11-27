#include "capturedevice.h"

CaptureDevice::CaptureDevice(QString dev_name, QObject *pParent) :
    QObject(pParent)
{
    dev_name = dev_name;
    fd = -1;
    buffers = NULL;
    n_buffers = 0;
    index = -1;
}

CaptureDevice::~CaptureDevice()
{
    stop_capturing();
    uninit_device();
    close_device();
}

int CaptureDevice::open_device()
{
    if(close_device() == -1)
        return -1;
    fd = open(dev_name.toStdString().c_str(), O_RDWR/*|O_NONBLOCK*/, 0);
    if(-1 == fd)
    {
        emit display_error(tr("open: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int CaptureDevice::close_device()
{
    if(fd != -1 && -1 == close(fd))
    {
        emit display_error(tr("close: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int CaptureDevice::init_device()
{
    if(-1 == fd)
        return -1;
    v4l2_capability cap;
    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 is no V4l2 device").arg(dev_name));
        }
        else
        {
            emit display_error(tr("VIDIOC_QUERYCAP: %1").arg(QString(strerror(errno))));
        }
        return -1;
    }
    qDebug() << "Driver Name:" << (const char*)cap.driver;
    qDebug() << "Card Name:" << (const char*)cap.card;
    qDebug() << "Bus info:" << (const char*)cap.bus_info;
    qDebug() << "Driver Version:" <<
                (int)((cap.version>>16)&0x000000ff) <<
                (int)((cap.version>>8)&0x000000ff) <<
                (int)(cap.version&0x000000ff);
    qDebug() << "capabilities:" << cap.capabilities;
    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
    {
        qDebug() << "is video capture device";
        m_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }
    else
    {
        emit display_error(tr("%1 is not capture device").arg(dev_name));
        return -1;
    }
    //>@查看是否支持STREAM
    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        emit display_error(tr("%1 does not support streaming i/o").arg(dev_name));
        return -1;
    }

    //>@显示所有格式
    struct v4l2_format fmt;
    fmt.type=m_type;
    if(-1 == ioctl(fd, VIDIOC_G_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_G_FMT%1").arg(QString(strerror(errno))));
        return -1;
    }
    qDebug() << "Current data format information:";
    qDebug() << "width" << fmt.fmt.pix.width;
    qDebug() << "height" << fmt.fmt.pix.height;
    qDebug() << "pixelformat" << fmt.fmt.pix.pixelformat;
    qDebug() << "field" << fmt.fmt.pix.field;
    qDebug() << "bytesperline" << fmt.fmt.pix.bytesperline;
    qDebug() << "sizeimage" << fmt.fmt.pix.sizeimage;
    qDebug() << "colorspace" << fmt.fmt.pix.colorspace;

    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=m_type;
    qDebug() << "Support format:";
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
        {
             qDebug() << "the current format:"  << (const char *)(fmtdesc.description);
             //break;
        }
        qDebug() << fmtdesc.index+1 << ":/t" << (const char *)(fmtdesc.description);
        fmtdesc.index++;
    }
    //>@设置所有格式
    width = fmt.fmt.pix.width = 320;
    height = fmt.fmt.pix.height = 240;
    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_S_FMT%1").arg(QString(strerror(errno))));
        return -1;
    }

    fmt.type=m_type;
    if(-1 == ioctl(fd, VIDIOC_G_FMT, &fmt))
    {
        emit display_error(tr("VIDIOC_G_FMT%1").arg(QString(strerror(errno))));
        return -1;
    }
    qDebug() << "Current data format information:";
    qDebug() << "width" << fmt.fmt.pix.width;
    qDebug() << "height" << fmt.fmt.pix.height;
    qDebug() << "pixelformat" << fmt.fmt.pix.pixelformat;
    qDebug() << "field" << fmt.fmt.pix.field;
    qDebug() << "bytesperline" << fmt.fmt.pix.bytesperline;
    qDebug() << "sizeimage" << fmt.fmt.pix.sizeimage;
    qDebug() << "colorspace" << fmt.fmt.pix.colorspace;

    //>@设置视频采集的输入
    struct v4l2_input inputenum;
    inputenum.index=0;
    inputenum.type=m_type;
    qDebug() << "Support Input:";
    while(ioctl(fd,VIDIOC_ENUMINPUT,&inputenum)!=-1)
    {
        qDebug() << inputenum.index
                 <<  "name" << (const char *)(inputenum.name)
                 << "type" << inputenum.type
                 << "audioset" << inputenum.audioset
                 << "tuner" << inputenum.tuner
                 << "std" << inputenum.std
                 << "status" << inputenum.status;
        inputenum.index++;
    }
    int input;
    if(-1 == ioctl(fd, VIDIOC_G_INPUT, &input))
    {
        qDebug() << "VIDIOC_G_INPUT";
    }
    else
    {
        qDebug() << "input" << input;
        input = inputenum.index-1;
        if(-1 == ioctl(fd, VIDIOC_S_INPUT, &input))
        {
            qDebug() << "VIDIOC_S_INPUT Error";
            //emit display_error(tr("VIDIOC_S_INPUT%1").arg(QString(strerror(errno))));
            //return -1;
        }
    }
    v4l2_std_id std;
    if(-1 == ioctl(fd, VIDIOC_G_STD, &std))
    {
        qDebug() << "VIDIOC_G_STD";
    }
    else
    {
        qDebug() << "std" << std;
        std = V4L2_STD_PAL_60;
        if(-1 == ioctl(fd, VIDIOC_S_STD, &std))
        {
            qDebug() << "VIDIOC_S_STD Error";
            //emit display_error(tr("VIDIOC_S_STD%1").arg(QString(strerror(errno))));
            //return -1;
        }
    }

    //>@查选视频裁减能力
    v4l2_cropcap cropcap;
    cropcap.type = m_type;
    if(-1 == ioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        emit display_error(tr("VIDIOC_CROPCAP%1").arg(QString(strerror(errno))));
        return -1;
    }
    qDebug() << "bounds left:" << cropcap.bounds.left;
    qDebug() << "bounds top:" << cropcap.bounds.top;
    qDebug() << "bounds width:" << cropcap.bounds.width;
    qDebug() << "bounds height:" << cropcap.bounds.height;
    qDebug() << "defrect left:" << cropcap.defrect.left;
    qDebug() << "defrect top:" << cropcap.defrect.top;
    qDebug() << "defrect width:" << cropcap.defrect.width;
    qDebug() << "defrect height:" << cropcap.defrect.height;
    qDebug() << "pixelaspect numerator:" << cropcap.pixelaspect.numerator;
    qDebug() << "pixelaspect top:" << cropcap.pixelaspect.denominator;
    //>@设置缩放
    struct v4l2_crop crop;
    memset(&crop, 0, sizeof (crop));
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c.left = 0;
    crop.c.top = 0;
    crop.c.width = 320;
    crop.c.height = 240;
    if (ioctl(fd, VIDIOC_S_CROP, &crop) < 0)
    {
        qDebug() << "set cropping failed ";
    }

    struct v4l2_streamparm parm;
    parm.type = m_type;
    if (ioctl(fd, VIDIOC_G_PARM, &parm))
    {
        qDebug() << "capability" << parm.parm.capture.capability;	  /*  Supported modes */
        qDebug() << "capturemode" << parm.parm.capture.capturemode;	  /*  Current mode */
        qDebug() << "timeperframe.denominator" << parm.parm.capture.timeperframe.denominator;  /*  Time per frame in .1us units */
        qDebug() << "timeperframe.numerator" << parm.parm.capture.timeperframe.numerator;  /*  Time per frame in .1us units */
        qDebug() << "extendedmode" << parm.parm.capture.extendedmode;  /*  Driver-specific extensions */
        qDebug() << "readbuffers" << parm.parm.capture.readbuffers;
    }
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = 15;
    if (ioctl(fd, VIDIOC_S_PARM, &parm) < 0)
    {
        qDebug() << "VIDIOC_S_PARM failed";
    }

    struct v4l2_control ctrl;
    if (ioctl(fd, VIDIOC_G_CTRL, &ctrl))
    {
        qDebug() << "id" << ctrl.id;
        qDebug() << "value" << ctrl.value;
    }
    // Set rotation
    ctrl.id = V4L2_CID_PRIVATE_BASE + 0;
    ctrl.value = 1;
    if (ioctl(fd, VIDIOC_S_CTRL, &ctrl) < 0)
    {
        qDebug() << "VIDIOC_S_CTRL failed";
    }

    //>@申请一个拥有四个缓冲帧的缓冲区
    struct v4l2_requestbuffers req;
    req.count=2;
    req.type=m_type;
    req.memory=V4L2_MEMORY_MMAP;
    if(-1 == ioctl(fd,VIDIOC_REQBUFS,&req))
    {
        emit display_error(tr("VIDIOC_REQBUFS%1").arg(QString(strerror(errno))));
        return -1;
    }
    //将四个已申请到的缓冲帧映射到应用程序，用buffers 指针记录。
    buffers = (buffer*)calloc (req.count, sizeof (*buffers));
    if (!buffers)
    {
        emit display_error(tr("Out of memory"));
        return -1;
    }
    //映射
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;
        memset(&buf,0,sizeof(buf));
        buf.type = m_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        //查询序号为n_buffers 的缓冲区，得到其起始物理地址和大小
        if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))
        {
            emit display_error(tr("Count:%1, VIDIOC_QUERYBUF:%2").arg(n_buffers).arg(QString(strerror(errno))));
            return -1;
        }
        buffers[n_buffers].length = buf.length;
        //映射内存
        buffers[n_buffers].start =mmap (NULL,buf.length,PROT_READ | PROT_WRITE ,MAP_SHARED,fd, buf.m.offset);
        if(MAP_FAILED == buffers[n_buffers].start)
        {
            emit display_error(tr("Count:%1, mmap error").arg(n_buffers));
            return -1;
        }
    }

    // 将缓冲帧放入队列
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = m_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
        {
            emit display_error(tr("Count:%1, VIDIOC_QBUF:%2").arg(n_buffers).arg(QString(strerror(errno))));
            return -1;
        }
    }

    qDebug() << "Init OK";
    return 0;
}

int CaptureDevice::init_mmap()
{
    if(-1 == fd)
        return -1;
    v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 4;
    req.type = m_type;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        if(EINVAL == errno)
        {
            emit display_error(tr("%1 does not support memory mapping").arg(dev_name));
            return -1;
        }
        else
        {
            emit display_error(tr("VIDIOC_REQBUFS %1").arg(QString(strerror(errno))));
            return -1;
        }
    }

    if(req.count < 0)
    {
        emit display_error(tr("Insufficient buffer memory on %1").arg(dev_name));
        return -1;
    }

    buffers = (buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers)
    {
        emit display_error(tr("out of memory"));
        return -1;
    }

    for(n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = m_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
        {
            emit display_error(tr("VIDIOC_QUERYBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap(NULL, // start anywhere
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd, buf.m.offset);

        if(MAP_FAILED == buffers[n_buffers].start)
        {
            emit display_error(tr("mmap %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    return 0;

}

int CaptureDevice::start_capturing()
{
    if(-1 == fd)
        return -1;
    //>@启动数据流
    v4l2_buf_type type = m_type;
    if (-1 == ioctl (fd, VIDIOC_STREAMON, &type))
    {
        emit display_error(tr("VIDIOC_STREAMON:%1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int CaptureDevice::stop_capturing()
{
    if(-1 == fd)
        return 0;
    v4l2_buf_type type;
    type = m_type;
    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        emit display_error(tr("VIDIOC_STREAMOFF: %1").arg(QString(strerror(errno))));
        return -1;
    }
    return 0;
}

int CaptureDevice::uninit_device()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i)
    {
        if(-1 == munmap(buffers[i].start, buffers[i].length))
        {
            emit display_error(tr("munmap: %1").arg(QString(strerror(errno))));
            return -1;
        }
    }
    free(buffers);
    return 0;
}

int CaptureDevice::get_frame(void **frame_buf, size_t* len)
{
    if(-1 == fd)
        return -1;
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = m_type;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf))
    {
        emit display_error(tr("VIDIOC_DQBUF: %1").arg(QString(strerror(errno))));
        return -1;
    }


    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    qDebug() << "frame buff index:" << queue_buf.index  << "addr:" << frame_buf;

    return 0;
}

QPixmap CaptureDevice::get_pixmap(QSize pSize)
{
    unsigned char *pp = (unsigned char *)malloc(width*height*3 * sizeof(char));
    unsigned char* frame_buf;
    unsigned int len;
    int rs = get_frame((void **)&frame_buf, &len);
    if(rs == -1)
    {
        free(pp);
        return QPixmap();
    }
    convert_yuv_to_rgb_buffer((unsigned char*)frame_buf, pp, width, height);
    QImage *frame = new QImage(pp, width, height, QImage::Format_RGB888);
    if(frame->loadFromData((uchar *)pp,len) == false)
    {
        free(pp);
        return QPixmap();
    }
    QPixmap tmpPixmap;
    if(pSize.isEmpty())
        tmpPixmap= QPixmap::fromImage(*frame);
    else
        tmpPixmap= QPixmap::fromImage(frame->scaled(pSize));
    unget_frame();
    free(pp);
    return tmpPixmap;
}

int CaptureDevice::unget_frame()
{
    if(-1 == fd)
        return -1;
    if(index != -1)
    {
        v4l2_buffer queue_buf;
        CLEAR(queue_buf);

        queue_buf.type = m_type;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf))
        {
            emit display_error(tr("VIDIOC_QBUF: %1").arg(QString(strerror(errno))));
            return -1;
        }
        return 0;
    }
    return -1;
}

int CaptureDevice::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_16;
    unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0, u, y1, v;

    for(in = 0; in < width * height * 2; in += 4)
    {
        pixel_16 =
                yuv[in + 3] << 24 |
                yuv[in + 2] << 16 |
                yuv[in + 1] <<  8 |
                yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u  = (pixel_16 & 0x0000ff00) >>  8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v  = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}

