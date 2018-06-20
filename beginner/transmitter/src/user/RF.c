#include "SP37_ROMLibrary.h"
#include<SP37.h>
#include<RF.h>


/*
unsigned char Scale_Pressure_For_RF(signed int idata Measurement_Result)
{		
	return Scale_Pressure(&Measurement_Result);
}
*/

//	1+  1+       1+          2+           2+      2+	1 = 10
//IDH+IDL+Pressure+Temperature+Acceleration+Voltage+CRC = 10
//#define RF_DATA_BYTES 10

//	     4+            1+       2+          2+           2+      2+  1 = 14
//UniqueId+Tyre_Position+Pressure+Temperature+Acceleration+Voltage+CRC = 14
#define RF_DATA_BYTES 14


//DATAλ�������еĵ�9��
#define RF_DATA_HEAD  9

// �����������£���һ�����ݴ��εı������з�װ���ڶ���������������ͳ�ȥ
char Send_RF(T_SP37_DATA idata t_sp37_data)
{	
	//ԭʼ���ݣ������е������ǲ���һ����Э����еģ��������ֲ�2.37.1.2 Pattern Descriptor table����
	unsigned char idata RF_Array[]={
		0x00,                    //START of TABLE

		//�������÷������ݰ�ʱ ��ʱһ�����ʱ��
		//���ʱ������SP370�ڲ��������������
		0xDD,                    //Delay Pattern descriptor
		0,                       //DURATION = 0ms
		100,                     //RANDOM = 100,�������0-100ms�������ʱ


		//�����������ݰ�
		0x00,                    //TYPE for Manchester & FSK
		
		(2+1+RF_DATA_BYTES)*8,   //DATA_LANGTH = (WAKE UP + SYNS + DATA)*8bit

		0x00,
		0x00,                    //WAKE UP
		
		0x15,                    //SYNC
		// �����14��0x00������ռλ��Ȼ���ڶ����������
		0x00,                    //DATA(RF_DATA_BYTES �� DATA) ����Ĭ��14������
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
		
		0xF1,                    //END	�����Ҫ�ظ���������ݣ������޸Ĵ�ֵ��F1��ʾ�ظ���1��
                             	//����SP37_Rom_Lib_Guide.pdf
	};
// RF_DATA_HEAD����9����Ϊ�ڵ�һ����Ч������RF_Array[9]
// ����⵽�����ݷֱ���䵽RF_Array���飬Ȼ������RF_Array��ΪSend_RF_Data�����Ĵ���
// ��һ�����ݷ��͵����նˡ�
	RF_Array[RF_DATA_HEAD + 0] = (unsigned char)(t_sp37_data.ID_Result.ID>>24);  //ID���λ
	RF_Array[RF_DATA_HEAD + 1] = (unsigned char)(t_sp37_data.ID_Result.ID>>16);
	RF_Array[RF_DATA_HEAD + 2] = (unsigned char)(t_sp37_data.ID_Result.ID>>8);
	RF_Array[RF_DATA_HEAD + 3] = (unsigned char)(t_sp37_data.ID_Result.ID);      //ID���λ
	RF_Array[RF_DATA_HEAD + 4] = (unsigned char)t_sp37_data.Tyre_Position;
	RF_Array[RF_DATA_HEAD + 5] = t_sp37_data.Pressure >> 8;
	RF_Array[RF_DATA_HEAD + 6] = t_sp37_data.Pressure;
	RF_Array[RF_DATA_HEAD + 7] = t_sp37_data.Temperature >> 8;
	RF_Array[RF_DATA_HEAD + 8] = t_sp37_data.Temperature;
	RF_Array[RF_DATA_HEAD + 9] = t_sp37_data.Acceleration>>8;
	RF_Array[RF_DATA_HEAD + 10] = t_sp37_data.Acceleration;
	RF_Array[RF_DATA_HEAD + 11] = t_sp37_data.Voltage>>8;
	RF_Array[RF_DATA_HEAD + 12] = t_sp37_data.Voltage;

	RF_Array[RF_DATA_HEAD + 13] = CRC_Baicheva_Calculate(&RF_Array[RF_DATA_HEAD],13);	// CRC�ǿ⺯�����ṩ��

	return Send_RF_Data(RF_Array);	// �����������Send_RF_Data���з�������
}

