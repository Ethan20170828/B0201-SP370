#include "sys.h"
#include "CRC.h"
#include "SP37Data.h"

// 这个文件主要是接收到SP370的数据，然后对数据进行解析处理；
// 这些数据怎么来的?，是通过TDA5235收到数据，然后放到FIFO里，然后在FIFO里经过校验后，确认数据无问题，最后传过来
//详情SP37_Rom_Lib_Guide_V1.0.pdf  Page:34
//计算公式:1.373 kPa/LSB + 100kPa
//计算范围100Kpa~450Kpa
float SP37_Cal_Pressure(u8 rawVal)
{
	return rawVal*1.373+100;
}

float SP37_Cal_Pressure_V2(u8 *rawVal)		// 传过来的是2个字节
{
	s16 rawValTemp = rawVal[0] << 8 | rawVal[1];	// 把0左移8位然后与1进行位或，先传高位

	return rawValTemp/16.0;		// 得到胎压数据；也就是1/16=0.0625
}


//详情SP37_Rom_Lib_Guide_V1.0.pdf
//8000H = -2048.0 g
//0000H = 0.0 g
//7FFFH = 2047.9375 g
//( = 2048 g - 1 LSB where 1 LSB = 1/16 g)
float SP37_Cal_Acceleration(u8 *rawVal)
{
	s16 rawValTemp = rawVal[0] << 8 | rawVal[1];

	return rawValTemp/16.0;
}

//详情SP37_Rom_Lib_Guide_V1.0.pdf
//8000H = -256.0 ℃
//0000H = 0.0 ℃
//7FFFH = 255.9921875 ℃
//( = 256 ℃ - 1 LSB where 1 LSB = 1/128 ℃)
float SP37_Cal_Temperature(u8 *rawVal)
{
	s16 rawValTemp = rawVal[0] << 8 | rawVal[1];

	return rawValTemp/128.0;
}


//详情SP37_Rom_Lib_Guide_V1.0.pdf
//8000H = -4096.0 mV (Only theoretical number)
//0000H = 0.0 mV
//7FFFH = 4095.875 mV
//( = 4096 mV - 1 LSB where 1 LSB = 1/8 mV)
//返回V
float SP37_Cal_Voltage(u8 *rawVal)
{
	s16 rawValTemp = rawVal[0] << 8 | rawVal[1];
	
	return rawValTemp/8.0/1000;
}



