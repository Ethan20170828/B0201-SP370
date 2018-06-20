#ifndef _SP37DATA_H
#define _SP37DATA_H


//轮胎位置定义
typedef enum{
	LF	= 0,
	RF  = 1,
	LB  = 2,
	RB  = 3,
	Unknown,
}E_Tyre_Position;

//由于在main.c中使用PT_SP37_RF_DATA pt_SP37Data = (PT_SP37_RF_DATA)TDA5235_FIFO;
//结构体指针直接指向TDA5235_FIFO数组空间
//所以下面结构体顺序和大小,需要与"发射的数据包结构"相匹配
typedef struct SP37_RF_DATA {
	u8 UniqueID[4];
	u8 TyrePosition;
	u8 Pressure[2];
	u8 Temperature[2];
	u8 Acceleration[2];
	u8 Voltage[2];
	u8 CRC8;
	u8 RESERVED[16];
}T_SP37_RF_DATA,*PT_SP37_RF_DATA;

float SP37_Cal_Pressure(u8 rawVal);
float SP37_Cal_Pressure_V2(u8 *rawVal);
float SP37_Cal_Temperature(u8 *rawVal);
float SP37_Cal_Acceleration(u8 *rawVal);
float SP37_Cal_Voltage(u8 *rawVal);


#endif


