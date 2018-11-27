#include "protectadc.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <QDebug>
#include <QCoreApplication>

#define ADC_DATA_PATH(x) QString("/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage%1_raw").arg(x)
#define RAW_COUNTS 5

ProtectADC::ProtectADC(QThread *parent) : QThread(parent)
{

}

void ProtectADC::readAdcValue()
{
    int fd, ret;
    qreal vref = 3.36;
    double voltage, get_value;
    char data[20];
    for (int i = 0; i < RAW_COUNTS; ++i)
    {
        const char* tmp = ADC_DATA_PATH(i).toLatin1().data();
        printf("tmp = %s\n", tmp);
        fd = open(tmp, O_RDONLY);
        if (fd < 0)
        {
            perror("open AI8_DATA_PATH error");
            return;
        }
        ret = read(fd, data, sizeof(data));
        if (ret < 0)
        {
            perror("read data error");
            return;
        }
        /* collect data and calculate the voltage */
        get_value = atof(data);

        voltage = vref * (get_value / 4096);
        printf("get value: %f\n", get_value);
        printf("voltage: %.3f\n", voltage);

        close(fd);
    }
}

void ProtectADC::run()
{
    while (1)
    {
        readAdcValue();
        msleep(15);
    }
}
