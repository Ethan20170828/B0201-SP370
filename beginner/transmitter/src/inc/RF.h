#ifndef _RF_H
#define _RF_H

// 这个数据结构伴随着整个发射端程序，在程序运行过程中，各种的参数就暂存到这个数据结构中
typedef struct SP37_DATA {
	ID_Struct ID_Result;	//用于存储芯片唯一序列号(UniqueId)
	unsigned char IDH;		// 人为设置的，识别每个用户的产品，对用户做一些管理
	unsigned char IDL;		// 用户级的ID，给客户授权生产的
	E_Tyre_Position Tyre_Position;	// 轮胎位置编码
	int Pressure;
	int Temperature;
	int Acceleration;
	int Voltage;
	unsigned char CRC;
}T_SP37_DATA;

char Send_RF(T_SP37_DATA idata t_sp37_data);

#endif


