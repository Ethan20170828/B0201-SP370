#ifndef _RF_H
#define _RF_H

typedef struct SP37_DATA {
	ID_Struct ID_Result;	//”√”⁄¥Ê¥¢–æ∆¨Œ®“ª–Ú¡–∫≈(UniqueId)
	unsigned char IDH;
	unsigned char IDL;
	//E_Tyre_Position Tyre_Position;
	unsigned char Check_Trip_status;
	int Pressure;
	int Temperature;
	int Acceleration;
	int Voltage;
	unsigned char CRC;
}T_SP37_DATA;

char Send_RF(T_SP37_DATA idata t_sp37_data);
char Send_RF1(T_SP37_DATA idata t_sp37_data);

#endif


