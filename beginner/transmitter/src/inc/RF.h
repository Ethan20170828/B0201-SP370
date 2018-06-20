#ifndef _RF_H
#define _RF_H

// ������ݽṹ��������������˳����ڳ������й����У����ֵĲ������ݴ浽������ݽṹ��
typedef struct SP37_DATA {
	ID_Struct ID_Result;	//���ڴ洢оƬΨһ���к�(UniqueId)
	unsigned char IDH;		// ��Ϊ���õģ�ʶ��ÿ���û��Ĳ�Ʒ�����û���һЩ����
	unsigned char IDL;		// �û�����ID�����ͻ���Ȩ������
	E_Tyre_Position Tyre_Position;	// ��̥λ�ñ���
	int Pressure;
	int Temperature;
	int Acceleration;
	int Voltage;
	unsigned char CRC;
}T_SP37_DATA;

char Send_RF(T_SP37_DATA idata t_sp37_data);

#endif


