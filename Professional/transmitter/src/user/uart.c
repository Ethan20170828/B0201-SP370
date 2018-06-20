#include<Reg_SP37.h>
#include<uart.h>

//��PP2�˿��������
//�õ�����Դ
//1.TIME0
//2.PP2

//System clock = 12Mhz
//����Ϊ������19200

static void PP2(unsigned char HIGH_Or_LOW);

#define HIGH 1
#define LOW	 0

void Uart_Init(void)
{
	//PP2����Ϊ���
	//Ĭ�������ߵ�ƽ
	P1DIR &= ~(1<<2);
	P1OUT |= (1<<2);
	P1SENS &= ~(1<<2);

	PP2(HIGH);
}

static void PP2(unsigned char HIGH_Or_LOW)
{
	if(HIGH_Or_LOW)
		P1OUT |= (1<<2);	
	else
		P1OUT &= ~(1<<2);
}

char putchar (unsigned char char_to_send)
{
	unsigned char i;
	unsigned char TMOD_old, TL0_old, TH0_old;

	//����֮ǰ�����ã��Ա���ԭ
	TMOD_old = TMOD;
	TH0_old = TH0;
	TL0_old = TL0;

	TMOD = (1<<4) | (2<<0);	// System clock divided by SFR DIVIC and 8
													// ����System clock/8ʱ���ṩ����ʱ�� System clock = 12Mhz
													// Mode 2:two 8 bit timers w/ reload
	TH0 = 0x4E;							//19200 ���������ֵԼ����78.125
	TL0 = 0x4E;
	
	//��ʼ��(ǰ��ʱ�䲻�ȶ�)
	PP2(HIGH);
	T0RUN = 1;
	IDLE = 1;
	T0FULL = 0;

	//���� ��ʼλ
	PP2(LOW);
	IDLE = 1;
	T0FULL = 0;
	
	//����8λ����
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

	//����ֹͣλ
	PP2(HIGH);
	IDLE = 1;
	T0FULL = 0;

	//��ԭ����
	T0RUN = 0;
	TMOD = TMOD_old;
	TH0 = TH0_old;
	TL0 = TL0_old;

	return 0;
}

