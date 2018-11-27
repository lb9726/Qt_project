#include "mainwindow.h"
#include "ui_mainwindow.h"
static unsigned char Covert[20];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CountFileSize(FILENAME, Covert, 4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CountFileSize(const char *filename, unsigned char *str, int count)
{
    qDebug()<< __PRETTY_FUNCTION__;
    QFile file(filename);
    int length = file.size();
    qDebug("filelength = %d", length);
    IntToUnsignedChar(length, count, str);
}

//void MainWindow::IntToUnsignedChar(int length, int count, unsigned char *str)
//{
//    qDebug()<< __PRETTY_FUNCTION__;
//    unsigned char temp[10];
////    int i = 0;
//    int j = 1;
//    int t = 1;
//    int c = count;
//    int len;
//    int num = 0;
//    while (length)
//    {
//        for(j = 1; j < c; j++) // 计算对应的权值10的多少次方
//        {
//            t = t*10;
//        }
//        c = c - 1;
//        len = length / t;  // 计算商
//        qDebug("len = %d", len);
//        if (0 == len)
//        {
//            temp[num] = len + 0x00;
//            qDebug("temp[%d] = %x", num, temp[num]);
//        }
//        else if (len > 0)
//        {
//            if (len <= 255)
//            {
//                temp[num] = len + 0x00;
//                length = length - len*t;
//            }
//            else if (len > 255)
//            {
//                temp[num] = 0xff;
//                length = length - 255*t;
//            }
//        }
//        qDebug("temp[%d] = %x", num, temp[num]);
//        if (num <= count)
//            num++;
//        t = 1;
//    }
//    memcpy(str, temp, count);
//}

void MainWindow::IntToUnsignedChar(int length, int count, unsigned char *str)
{
    ConvertFlag = true;
    qDebug()<< __PRETTY_FUNCTION__;
    int array[10];
    int i = 0;
    int pos = 0;
    int flag = 0;
    if (length <= 255) // 如果文件大小小于255，则可以直接对数组赋值
    {
        Covert[0] = length + 0x00;
        Covert[1] = 0x00;
        Covert[2] = 0x00;
        Covert[3] = 0x00;
        return;
    }
    while (ConvertFlag)
    {
        SplitNumber(length, array, &i);
        length = GetNewLength(&i, length, array, Covert, &pos);
        if (0 >= length)
            ConvertFlag = false;
    }

    qDebug("i = %d", i);
    int j = 0;
    for (j = 0; j < i; j++)
    {
        qDebug("array[%d] = %d", j, array[j]);
    }
    for (j = 0; j < 4; j++)
    {
        qDebug("Covert[%d] = %x", j, Covert[j]);
    }
}

//
int MainWindow::GetNewLength(int *i, int length, int array[], unsigned char *str, int *pos)
{
    qDebug()<< __PRETTY_FUNCTION__;
    int j = 0;
    int num = 0;
    Quanzhi a[4];
    a[0].quan = 1000;
    a[1].quan = 100;
    a[2].quan = 10;
    a[3].quan = 1;
    int counts = 0;
    for (j = 0; j < *i; j++)
    {
        counts++;
        num = num * 10 + array[j];
        qDebug("array[%d] = %d", j, array[j]);
        qDebug("counts = %d", counts);
        qDebug("j = %d", j);
        qDebug("num = %d", num);
        if (counts % 3 == 0)  // 当三位数的时候，需要比较与255的大小
        {   qDebug("counts = %d", counts);
            qDebug("num = %d", num);
            if (num <= 255)
            {
                str[(*pos)++] = num + 0x00;
                length = length - num*(a[*pos-1].quan);
            }
            else if (num > 255)
            {
                str[(*pos)++] = 0xff;
                length = length - 255*a[*pos-1].quan;
            }
            num = 0;
            *i = 0;qDebug("length = %d", length);
            return length;
        }
        else if (j == *i - 1)
        {
            if (num <= 255)
            {
                str[(*pos)++] = num + 0x00;
                length = length - num*a[*pos-1].quan;
            }
            else if (num > 255)
            {
                str[(*pos)++] = 0xff;
                length = length - num*a[*pos-1].quan;
            }
            num = 0;
            *i = 0;qDebug("length = %d", length);
            return length;
        }
    }
}

// 将length拆分到数组中，并使得是顺序的显示
int MainWindow::SplitNumber(int length, int array[], int *i)
{
    qDebug()<< __PRETTY_FUNCTION__;
    while(length)
    {
        array[*i] = length % 10;
        length = length / 10;
        qDebug("array[%d] = %d", *i, array[*i]);
        (*i)++;
    }
    int j = 0;
    int k = *i - 1; // 长度减掉1
    int temp;
    for (j = 0; j < *i/2; j++,k--) // 将数组倒置一下
    {
        temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }
    for(j = 0; j < *i; j++)
    {
        qDebug("ar[%d] = %d", j, array[j]);
    }
    return 0;
}

int MainWindow::Micifang(int zhishu, int dishu)
{
    int i = 0;
    int result = 1;
    for (i = 0; i < zhishu; i++)
    {
        result = result *dishu;
    }
    return result;
}
