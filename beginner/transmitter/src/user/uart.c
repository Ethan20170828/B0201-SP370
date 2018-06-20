#include<Reg_SP37.h>
#include<uart.h>

//在PP2端口输出数据
//用到的资源
//1.TIME0
//2.PP2

//System clock = 12Mhz，这是SP370内部的51单片机的时钟频率，在SP360的datasheet中有(3.7 Clock Controller)
//配置为波特率19200

static void PP2(unsigned char HIGH_Or_LOW);

#define HIGH 1
#define LOW	 0
// Uart_Init函数是英飞凌官方在示例代码中提供的
void Uart_Init(void)
{
	//PP2配置为输出
	//默认上拉高电平
	P1DIR &= ~(1<<2);
	P1OUT |= (1<<2);
	P1SENS &= ~(1<<2);

	PP2(HIGH);
}
// PP2引脚是对外发高或低电平的
static void PP2(unsigned char HIGH_Or_LOW)
{
	if(HIGH_Or_LOW)
		P1OUT |= (1<<2);	
	else
		P1OUT &= ~(1<<2);
}
// 通过串口向外发送一个字符，和典型的串口发送信息不一样，所以函数中的内部信息不需要仔细去研究
char putchar (unsigned char char_to_send)
{
	unsigned char i;
	unsigned char TMOD_old, TL0_old, TH0_old;

	//保存之前的设置，以备还原
	TMOD_old = TMOD;
	TH0_old = TH0;
	TL0_old = TL0;

	TMOD = (1<<4) | (2<<0);	// System clock divided by SFR DIVIC and 8
							// 利用System clock/8时钟提供给定时器 System clock = 12Mhz
							// Mode 2:two 8 bit timers w/ reload
	TH0 = 0x4E;				//19200 计算出来的值约等于78.125
	TL0 = 0x4E;
	
	//初始化(前期时间不稳定)
	PP2(HIGH);
	T0RUN = 1;
	IDLE = 1;
	T0FULL = 0;

	//发送 起始位
	PP2(LOW);
	IDLE = 1;
	T0FULL = 0;
	
	//发送8位数据
	for(i=0;i<8;i++)
	{
		if(char_to_send & 0x01)
			PP2(HIGH);
		else
			PP2(LOW);
		char_to_send >>= 1;
		IDLE = 1;
		T0FULL = 0;
	}

	//发送停止位
	PP2(HIGH);
	IDLE = 1;
	T0FULL = 0;

	//还原设置
	T0RUN = 0;
	TMOD = TMOD_old;
	TH0 = TH0_old;
	TL0 = TL0_old;

	return 0;
}

