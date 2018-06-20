#include "sys.h"
#include "oled.h"
#include "display.h"
#include "mysprintf.h"
#include <math.h>
#include "sp37data.h"



void Display_Http(void)
{
	OLED_ShowStr(HTTP_START_X,HTTP_START_PAGE,"www.ziotlab.com");
}

void Display_Version(void)
{
		OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE,"LF  V1.0");
}


#define LOGO_MOVE_TO_RIGHT	0
#define LOGO_MOVE_TO_LEFT	1	


//默认16个数据
//没有更新的地方为0
void Display_Data(u8 *data)
{
	u8 i;

	char strArr[32]={0};
	char *str = strArr;

	//第一行数据	
	*str = 'D';	str++;
	*str = '0';	str++;
	*str = ':';	str++;

	for(i=0;i<8;i++){
		str += MySprintf_x(str,data[i]);
	}

	OLED_ShowStr(DATA_START_X,DATA_START_PAGE1,strArr);

	//第二行数据
	str = strArr;
	*str = 'D';	str++;
	*str = '1';	str++;
	*str = ':';	str++;

	for(i=8;i<16;i++){
		str += MySprintf_x(str,data[i]);
	}

	OLED_ShowStr(DATA_START_X,DATA_START_PAGE2,strArr);	
	
}

//显示" Re: "  这个三个字符串
void Display_RF_Receive_Mark(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'R';	str++;
	*str = 'e';	str++;
	*str = ':';	str++;

	MySprintf_ud(str,0,4);
	OLED_ShowStr(RF_RECEIVE_MARK_START_X,RF_RECEIVE_MARK_PAGE,strArr);	
}


void Display_RF_Receive_Count_Plus(void)
{
	char strArr[32]={0};
	char *str = strArr;

	static u16 Count = 0;

	Count ++;
	if(Count > 9999){
		Count = 0;
	}
	
	MySprintf_ud(str,Count,4);
	OLED_ShowStr(RF_RECEIVE_COUNT_START_X,RF_RECEIVE_COUNT_PAGE,strArr);
}

//在没有收到ID之前，显示XXXX
void Display_User_ID_NULL(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;

	OLED_ShowStr(USER_ID_START_X,USER_ID_START_PAGE,strArr);

}

void Display_User_ID(u8 IDH,u8 IDL)
{
	char strArr[32]={0};
	char *str = strArr;

	static u8 lastIDH = 0xFF;
	static u8 lastIDL = 0xFF;

	if(lastIDH==IDH && lastIDL==IDL){
		return;
	}else{
		lastIDH = IDH;
		lastIDL = IDL;
	}

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	str += MySprintf_x(str,IDH);
	str += MySprintf_x(str,IDL);

	OLED_ShowStr(USER_ID_START_X,USER_ID_START_PAGE,strArr);
}




void Display_Unique_ID1(u8 *UniqueID)
{
	char strArr[32]={0};
	char *str = strArr;

	static u8 lastUniqueID[4] = {0xFF,0XFF,0xFF,0xFF};

	if(	lastUniqueID[0]==UniqueID[0] && 
		lastUniqueID[1]==UniqueID[1] &&
		lastUniqueID[2]==UniqueID[2] && 
		lastUniqueID[3]==UniqueID[3]){
		return;
	}else{
		lastUniqueID[0]=UniqueID[0];
		lastUniqueID[1]=UniqueID[1];
		lastUniqueID[2]=UniqueID[2];
		lastUniqueID[3]=UniqueID[3];
	}

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	str += MySprintf_x(str,UniqueID[0]);
	str += MySprintf_x(str,UniqueID[1]);
	str += MySprintf_x(str,UniqueID[2]);
	str += MySprintf_x(str,UniqueID[3]);

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(19,3,strArr);
}
void Display_Unique_ID2(u8 *UniqueID)
{
	char strArr[32]={0};
	char *str = strArr;

	static u8 lastUniqueID[4] = {0xFF,0XFF,0xFF,0xFF};

	if(	lastUniqueID[0]==UniqueID[0] && 
		lastUniqueID[1]==UniqueID[1] &&
		lastUniqueID[2]==UniqueID[2] && 
		lastUniqueID[3]==UniqueID[3]){
		return;
	}else{
		lastUniqueID[0]=UniqueID[0];
		lastUniqueID[1]=UniqueID[1];
		lastUniqueID[2]=UniqueID[2];
		lastUniqueID[3]=UniqueID[3];
	}

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	str += MySprintf_x(str,UniqueID[0]);
	str += MySprintf_x(str,UniqueID[1]);
	str += MySprintf_x(str,UniqueID[2]);
	str += MySprintf_x(str,UniqueID[3]);

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(19,4,strArr);
}
void Display_Unique_ID3(u8 *UniqueID)
{
	char strArr[32]={0};
	char *str = strArr;

	static u8 lastUniqueID[4] = {0xFF,0XFF,0xFF,0xFF};

	if(	lastUniqueID[0]==UniqueID[0] && 
		lastUniqueID[1]==UniqueID[1] &&
		lastUniqueID[2]==UniqueID[2] && 
		lastUniqueID[3]==UniqueID[3]){
		return;
	}else{
		lastUniqueID[0]=UniqueID[0];
		lastUniqueID[1]=UniqueID[1];
		lastUniqueID[2]=UniqueID[2];
		lastUniqueID[3]=UniqueID[3];
	}

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	str += MySprintf_x(str,UniqueID[0]);
	str += MySprintf_x(str,UniqueID[1]);
	str += MySprintf_x(str,UniqueID[2]);
	str += MySprintf_x(str,UniqueID[3]);

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(19,5,strArr);
}
void Display_Unique_ID4(u8 *UniqueID)
{
	char strArr[32]={0};
	char *str = strArr;

	static u8 lastUniqueID[4] = {0xFF,0XFF,0xFF,0xFF};

	if(	lastUniqueID[0]==UniqueID[0] && 
		lastUniqueID[1]==UniqueID[1] &&
		lastUniqueID[2]==UniqueID[2] && 
		lastUniqueID[3]==UniqueID[3]){
		return;
	}else{
		lastUniqueID[0]=UniqueID[0];
		lastUniqueID[1]=UniqueID[1];
		lastUniqueID[2]=UniqueID[2];
		lastUniqueID[3]=UniqueID[3];
	}

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	str += MySprintf_x(str,UniqueID[0]);
	str += MySprintf_x(str,UniqueID[1]);
	str += MySprintf_x(str,UniqueID[2]);
	str += MySprintf_x(str,UniqueID[3]);

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(19,6,strArr);
}



//在没有收到ID之前，显示XXXXXXXX
void Display_Unique_ID_NULL1(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(0,0,strArr);

}

void Display_Pressure1(float pressure)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - pressure) < 1 ){
		return;
	}else{
		lastVal = pressure;
	} 

	*str = 'P';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,3);

	//*str = ' ';	str++;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;

	//OLED_ShowStr(PRESSURE_START_X,PRESSURE_START_PAGE,strArr);
	OLED_ShowStr(0,1,strArr);
	
}


void Display_Temperature1(float temperature)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - temperature) < 1 ){
		return;
	}else{
		lastVal = temperature;
	}

	*str = 'T';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,2);

	//*str = ' ';	
	//str++;
	
	*str = 95+32;	str++;		//特殊字符℃

	//OLED_ShowStr(TEMPERATURE_START_X,TEMPERATURE_START_PAGE,strArr);
	OLED_ShowStr(63,1,strArr);
	
}

//在没有收到ID之前，显示XXXXXXXX
void Display_Unique_ID_NULL2(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(0,2,strArr);

}

void Display_Pressure2(float pressure)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - pressure) < 1 ){
		return;
	}else{
		lastVal = pressure;
	} 

	*str = 'P';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,3);

	//*str = ' ';	str++;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;

	//OLED_ShowStr(PRESSURE_START_X,PRESSURE_START_PAGE,strArr);
	OLED_ShowStr(0,3,strArr);
	
}


void Display_Temperature2(float temperature)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - temperature) < 1 ){
		return;
	}else{
		lastVal = temperature;
	}

	*str = 'T';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,2);

	//*str = ' ';	
	//str++;
	
	*str = 95+32;	str++;		//特殊字符℃

	//OLED_ShowStr(TEMPERATURE_START_X,TEMPERATURE_START_PAGE,strArr);
	OLED_ShowStr(63,3,strArr);
	
}
//在没有收到ID之前，显示XXXXXXXX
void Display_Unique_ID_NULL3(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(0,4,strArr);

}

void Display_Pressure3(float pressure)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - pressure) < 1 ){
		return;
	}else{
		lastVal = pressure;
	} 

	*str = 'P';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,3);

	//*str = ' ';	str++;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;

	//OLED_ShowStr(PRESSURE_START_X,PRESSURE_START_PAGE,strArr);
	OLED_ShowStr(0,5,strArr);
	
}


void Display_Temperature3(float temperature)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - temperature) < 1 ){
		return;
	}else{
		lastVal = temperature;
	}

	*str = 'T';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,2);

	//*str = ' ';	
	//str++;
	
	*str = 95+32;	str++;		//特殊字符℃

	//OLED_ShowStr(TEMPERATURE_START_X,TEMPERATURE_START_PAGE,strArr);
	OLED_ShowStr(63,5,strArr);
	
}
//在没有收到ID之前，显示XXXXXXXX
void Display_Unique_ID_NULL4(void)
{
	char strArr[32]={0};
	char *str = strArr;

	*str = 'I';	str++;
	*str = 'D';	str++;
	*str = ':';	str++;
	
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;
	*str = 'X';	str++;

	//OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
	OLED_ShowStr(0,6,strArr);

}

void Display_Pressure4(float pressure)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - pressure) < 1 ){
		return;
	}else{
		lastVal = pressure;
	} 

	*str = 'P';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,3);

	//*str = ' ';	str++;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;

	//OLED_ShowStr(PRESSURE_START_X,PRESSURE_START_PAGE,strArr);
	OLED_ShowStr(0,7,strArr);
	
}


void Display_Temperature4(float temperature)
{
	char strArr[32]={0};
	char *str = strArr;

	int lastVal = -1;
	if(fabs(lastVal - temperature) < 1 ){
		return;
	}else{
		lastVal = temperature;
	}

	*str = 'T';	str++;
	*str = ':';	str++;

	str += MySprintf_d(str,lastVal,2);

	//*str = ' ';	
	//str++;
	
	*str = 95+32;	str++;		//特殊字符℃

	//OLED_ShowStr(TEMPERATURE_START_X,TEMPERATURE_START_PAGE,strArr);
	OLED_ShowStr(63,7,strArr);
	
}

void Display_Acceleration(float acceleration)
{
	char strArr[32]={0};
	char *str = strArr;

	static float lastVal = -1;
	if(fabs(lastVal - acceleration) < 0.0001 ){
		return;
	}else{
		lastVal = acceleration;
	}

	*str = 'A';	str++;
	*str = ':';	str++;

	str += MySprintf_f(str,acceleration,3,4);

	*str = ' ';	str++;
	*str = 'g';	str++;
	

	OLED_ShowStr(ACCELERATION_START_X,ACCELERATION_START_PAGE,strArr);
	
}


void Display_Voltage(float voltage)
{
	char strArr[32]={0};
	char *str = strArr;

	static float lastVal = -1;
	if(fabs(lastVal - voltage) < 0.0001 ){
		return;
	}else{
		lastVal = voltage;
	}

	*str = 'V';	str++;
	*str = ':';	str++;

	str += MySprintf_f(str,voltage,3,4);

	*str = ' ';	str++;
	*str = 'v';	str++;
	

	OLED_ShowStr(VOLTAGE_START_X,VOLTAGE_START_PAGE,strArr);
	
}

