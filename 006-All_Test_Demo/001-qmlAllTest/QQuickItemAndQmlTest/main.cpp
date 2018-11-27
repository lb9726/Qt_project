#include "framework.h"

int main(int argc, char *argv[])
{
    FrameWork* tmpFrameWork = FrameWork::getInstance();
    tmpFrameWork->registerMyQmlType();
    tmpFrameWork->runApplication(argc, argv);
}
