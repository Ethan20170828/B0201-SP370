#include "SP37_ROMLibrary.h"
#include<SP37.h>
#include<RF.h>



//	1+  1+       1+          2+           2+      2+	1 = 10
//IDH+IDL+Pressure+Temperature+Acceleration+Voltage+CRC = 10
//#define RF_DATA_BYTES 10

//	     4+            1+       2+          2+           2+      2+  1 = 14
//UniqueId+Tyre_Position+Pressure+Temperature+Acceleration+Voltage+CRC = 14
#define RF_DATA_BYTES 14


//DATA位于数组中的第9个
#define RF_DATA_HEAD  9
//返回值0:成功
//返回其他值 失败
//-1: StartXtalOscillator->error
//-2: VCO_Tuning->error
//-3: Send_RF_Telegram->error
//-4: StopXtalOscillator->error
char Send_RF(T_SP37_DATA idata t_sp37_data)
{	
	//原始数据
	unsigned char idata RF_Array[]={
		0x00,                    //START of TABLE

		//以下配置发射数据包时 延时一个随机时间
		//随机时间来之SP37内部的随机数发生器
		0xDD,                    //Delay Pattern descriptor
		0,                       //DURATION = 0ms
		100,                     //RANDOM = 100,将会产生0-100ms的随机延时


		//以下配置数据包
		0x00,                    //TYPE for Manchester
		
		(2+1+RF_DATA_BYTES)*8,   //DATA_LANGTH = (WAKE UP + SYNS + DATA)*8bit

		0x00,
		0x00,                    //WAKE UP
		
		0x15,                    //SYNC

		0x00,                    //DATA(RF_DATA_BYTES 个 DATA) 这里默认14个数据
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		
		0xF1,                    //END	如果需要重复发多包数据，可以修改此值
                          	    //详情SP37_Rom_Lib_Guide.pdf
	};

	RF_Array[RF_DATA_HEAD + 0] = (unsigned char)(t_sp37_data.ID_Result.ID>>24);  //ID最高位
	RF_Array[RF_DATA_HEAD + 1] = (unsigned char)(t_sp37_data.ID_Result.ID>>16);
	RF_Array[RF_DATA_HEAD + 2] = (unsigned char)(t_sp37_data.ID_Result.ID>>8);
	RF_Array[RF_DATA_HEAD + 3] = (unsigned char)(t_sp37_data.ID_Result.ID);      //ID最低位
	RF_Array[RF_DATA_HEAD + 4] = (unsigned char)t_sp37_data.Check_Trip_status;
	RF_Array[RF_DATA_HEAD + 5] = t_sp37_data.Pressure >> 8;
	RF_Array[RF_DATA_HEAD + 6] = t_sp37_data.Pressure;
	RF_Array[RF_DATA_HEAD + 7] = t_sp37_data.Temperature >> 8;
	RF_Array[RF_DATA_HEAD + 8] = t_sp37_data.Temperature;
	RF_Array[RF_DATA_HEAD + 9] = t_sp37_data.Acceleration>>8;
	RF_Array[RF_DATA_HEAD + 10] = t_sp37_data.Acceleration;
	RF_Array[RF_DATA_HEAD + 11] = t_sp37_data.Voltage>>8;
	RF_Array[RF_DATA_HEAD + 12] = t_sp37_data.Voltage;
	RF_Array[RF_DATA_HEAD + 13] = CRC_Baicheva_Calculate(&RF_Array[RF_DATA_HEAD],13);	

	return Send_RF_Data(RF_Array);
}

// 发送学习包函数
char Send_RF1(T_SP37_DATA idata t_sp37_data)
{	
	//原始数据
	unsigned char idata RF_Array[]={
		0x00,                    //START of TABLE

		//以下配置发射数据包时 延时一个随机时间
		//随机时间来之SP37内部的随机数发生器
		0xDD,                    //Delay Pattern descriptor
		0,                       //DURATION = 0ms
		100,                     //RANDOM = 100,将会产生0-100ms的随机延时


		//以下配置数据包
		0x00,                    //TYPE for Manchester
		
		(2+1+RF_DATA_BYTES)*8,   //DATA_LANGTH = (WAKE UP + SYNS + DATA)*8bit

		0x00,
		0x00,                    //WAKE UP
		
		0x15,                    //SYNC

		0x00,                    //DATA(RF_DATA_BYTES 个 DATA) 这里默认14个数据
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		// 0xFA表示循环发送10包数据
		0xFA,                    //END	如果需要重复发多包数据，可以修改此值
                            	 //详情SP37_Rom_Lib_Guide.pdf
	};

	RF_Array[RF_DATA_HEAD + 0] = (unsigned char)(t_sp37_data.ID_Result.ID>>24);  //ID最高位
	RF_Array[RF_DATA_HEAD + 1] = (unsigned char)(t_sp37_data.ID_Result.ID>>16);
	RF_Array[RF_DATA_HEAD + 2] = (unsigned char)(t_sp37_data.ID_Result.ID>>8);
	RF_Array[RF_DATA_HEAD + 3] = (unsigned char)(t_sp37_data.ID_Result.ID);      //ID最低位
	RF_Array[RF_DATA_HEAD + 4] = (unsigned char)t_sp37_data.Check_Trip_status;
	RF_Array[RF_DATA_HEAD + 5] = t_sp37_data.Pressure >> 8;
	RF_Array[RF_DATA_HEAD + 6] = t_sp37_data.Pressure;
	RF_Array[RF_DATA_HEAD + 7] = t_sp37_data.Temperature >> 8;
	RF_Array[RF_DATA_HEAD + 8] = t_sp37_data.Temperature;
	RF_Array[RF_DATA_HEAD + 9] = t_sp37_data.Acceleration>>8;
	RF_Array[RF_DATA_HEAD + 10] = t_sp37_data.Acceleration;
	RF_Array[RF_DATA_HEAD + 11] = t_sp37_data.Voltage>>8;
	RF_Array[RF_DATA_HEAD + 12] = t_sp37_data.Voltage;
	RF_Array[RF_DATA_HEAD + 13] = CRC_Baicheva_Calculate(&RF_Array[RF_DATA_HEAD],13);	

	return Send_RF_Data(RF_Array);
}


