#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define IO_PATH(str)  "/sys/class/gpio/gpio"#str"/value"
#define LED_ON  '1'
#define LED_OFF '0'

static const char *Output[] = {IO_PATH(32),IO_PATH(33),IO_PATH(34),IO_PATH(35),IO_PATH(36),IO_PATH(37),
						 IO_PATH(38),IO_PATH(39),IO_PATH(40),IO_PATH(41),IO_PATH(42),IO_PATH(43)};
static const char *Intput[] = {IO_PATH(128),IO_PATH(129),IO_PATH(130),IO_PATH(131),IO_PATH(132),IO_PATH(133),
						 IO_PATH(134),IO_PATH(135),IO_PATH(136),IO_PATH(137),IO_PATH(48),IO_PATH(49)};

//static unsigned char ButtonBuf[] = {0,1,2,3,4,5,6,7,8,9,10,11};

void SetButtonStatus(unsigned char index,unsigned char sta)
{
	int fd;
	fd = open(Output[index],O_RDWR);
	if(fd < 0){
		perror("open error:");
		return;
	}	
	write(fd,&sta,1);
	close(fd);
}
//亮灯情况下读取按钮状态值方法
// index : 0-11 对应BC0 - BC11
unsigned char LightOnCheckInputStatus(unsigned char index)
{
	int fd;
	char sta;
	SetButtonStatus(index,LED_OFF);	
	fd = open(Intput[index],O_RDONLY);	
	if(fd < 0){
		perror("open error:");
		SetButtonStatus(index,LED_ON);	
		return 100;
	}
	read(fd,&sta,1);
	close(fd);
	SetButtonStatus(index,LED_ON);	
//	printf("Read sta:BC%d is %c\n",ButtonBuf[index],sta);
	return sta;
}
//正常情况下读取按钮状态值方法
// index : 0-11 对应BC0 - BC11
unsigned char NormalInputStatus(unsigned char index)
{
	int fd;
	char sta;
	fd = open(Intput[index],O_RDONLY);	
	if(fd < 0){
		perror("open error:");
		SetButtonStatus(index,LED_ON);	
		return 100;
	}
	read(fd,&sta,1);
	close(fd);
	return sta;
}

int main(int argc, char ** argv)
{
	int i;
	int CtrlLightNum[12];
	char ButtonPressFlag[12];

	for(i = 0; i < 12; i++)
	{
		SetButtonStatus(i,LED_OFF);
		CtrlLightNum[i] = 0;
		ButtonPressFlag[i] = 0;
	}
	
	while(1)
	{
		for(i = 0; i < 12; i++){

			if(CtrlLightNum[i] == '0' ){//按下 ,亮灯情况下

				if(LightOnCheckInputStatus(i) == '0'){
					ButtonPressFlag[i] = 1;//记录按下动作
				}else{//松开
					if(ButtonPressFlag[i]){
						ButtonPressFlag[i] = 0;//清除按下操作
						SetButtonStatus(i,LED_OFF);//灭灯
						//发送给控制柜，按下信息
						//if(控制柜灭灯)
						//{
						printf("Cancel\n");
						CtrlLightNum[i] = '1';
						//}
					}
				}

			}else{//非亮灯情况下
		
				if(ButtonPressFlag[i]){
					if(LightOnCheckInputStatus(i) == '1'){//松开
						//SetButtonStatus(i,LED_ON);//点灯
						ButtonPressFlag[i] = 0;//清除按下动作
						//测试需要
						CtrlLightNum[i] = '0';
						//发送给控制柜
						//if(控制柜点灯){
						//	CtrlLightNum[i] = '0';
						//}else{
						//  CtrlLightNum[i] = '1';
						//}
					}
				}else{
					if(NormalInputStatus(i) == '0'){//按下动作
						ButtonPressFlag[i] = 1;//记录按下动作
						SetButtonStatus(i,LED_ON);//点灯
					}
				}	
			}
		}
		usleep(10000);
	}
	return 0;
}

