#include "SP37_ROMLibrary.h"
#include<SP37.h>
#include<RF.h>



//	1+  1+       1+          2+           2+      2+	1 = 10
//IDH+IDL+Pressure+Temperature+Acceleration+Voltage+CRC = 10
//#define RF_DATA_BYTES 10

//	     4+            1+       2+          2+           2+      2+  1 = 14
//UniqueId+Tyre_Position+Pressure+Temperature+Acceleration+Voltage+CRC = 14
#define RF_DATA_BYTES 14


//DATAλ�������еĵ�9��
#define RF_DATA_HEAD  9
//����ֵ0:�ɹ�
//��������ֵ ʧ��
//-1: StartXtalOscillator->error
//-2: VCO_Tuning->error
//-3: Send_RF_Telegram->error
//-4: StopXtalOscillator->error
char Send_RF(T_SP37_DATA idata t_sp37_data)
{	
	//ԭʼ����
	unsigned char idata RF_Array[]={
		0x00,                    //START of TABLE

		//�������÷������ݰ�ʱ ��ʱһ�����ʱ��
		//���ʱ����֮SP37�ڲ��������������
		0xDD,                    //Delay Pattern descriptor
		0,                       //DURATION = 0ms
		100,                     //RANDOM = 100,�������0-100ms�������ʱ


		//�����������ݰ�
		0x00,                    //TYPE for Manchester
		
		(2+1+RF_DATA_BYTES)*8,   //DATA_LANGTH = (WAKE UP + SYNS + DATA)*8bit

		0x00,
		0x00,                    //WAKE UP
		
		0x15,                    //SYNC

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
		
		0xF1,                    //END	�����Ҫ�ظ���������ݣ������޸Ĵ�ֵ
                          	    //����SP37_Rom_Lib_Guide.pdf
	};

	RF_Array[RF_DATA_HEAD + 0] = (unsigned char)(t_sp37_data.ID_Result.ID>>24);  //ID���λ
	RF_Array[RF_DATA_HEAD + 1] = (unsigned char)(t_sp37_data.ID_Result.ID>>16);
	RF_Array[RF_DATA_HEAD + 2] = (unsigned char)(t_sp37_data.ID_Result.ID>>8);
	RF_Array[RF_DATA_HEAD + 3] = (unsigned char)(t_sp37_data.ID_Result.ID);      //ID���λ
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

// ����ѧϰ������
char Send_RF1(T_SP37_DATA idata t_sp37_data)
{	
	//ԭʼ����
	unsigned char idata RF_Array[]={
		0x00,                    //START of TABLE

		//�������÷������ݰ�ʱ ��ʱһ�����ʱ��
		//���ʱ����֮SP37�ڲ��������������
		0xDD,                    //Delay Pattern descriptor
		0,                       //DURATION = 0ms
		100,                     //RANDOM = 100,�������0-100ms�������ʱ


		//�����������ݰ�
		0x00,                    //TYPE for Manchester
		
		(2+1+RF_DATA_BYTES)*8,   //DATA_LANGTH = (WAKE UP + SYNS + DATA)*8bit

		0x00,
		0x00,                    //WAKE UP
		
		0x15,                    //SYNC

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
		// 0xFA��ʾѭ������10������
		0xFA,                    //END	�����Ҫ�ظ���������ݣ������޸Ĵ�ֵ
                            	 //����SP37_Rom_Lib_Guide.pdf
	};

	RF_Array[RF_DATA_HEAD + 0] = (unsigned char)(t_sp37_data.ID_Result.ID>>24);  //ID���λ
	RF_Array[RF_DATA_HEAD + 1] = (unsigned char)(t_sp37_data.ID_Result.ID>>16);
	RF_Array[RF_DATA_HEAD + 2] = (unsigned char)(t_sp37_data.ID_Result.ID>>8);
	RF_Array[RF_DATA_HEAD + 3] = (unsigned char)(t_sp37_data.ID_Result.ID);      //ID���λ
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


