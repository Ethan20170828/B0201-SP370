#ifndef _SP37_H
#define _SP37_H


//轮胎位置定义
//LF,RF,LB,RB他们的值与烧录器烧录的值要相对应
typedef enum{
	LF	= 0,
	RF  = 1,
	LB  = 2,
	RB  = 3,
}E_Tyre_Position;

unsigned char Measure_Pressure(int *Pressure_VAL);
unsigned char Measure_Acceleration(int *Acceleration_VAL);
unsigned char Measure_Temperature(int *Temperature_VAL);
unsigned char Measure_Supply_Voltage(int *Voltage_VAL);
char StartXtalOscillator(void);
char StopXtalOscillator(void);
unsigned char CRC8_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength);
unsigned char CRC_Baicheva_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength);

void Read_SP37_ID(unsigned char idata *IDH,unsigned char idata *IDL);
E_Tyre_Position Read_Tyre_Position(void);

unsigned char SP37_InitRandom(void);
void Send_RF_Init(void);
char Send_RF_Data(unsigned char idata * descriptorPtr);

#endif
		

