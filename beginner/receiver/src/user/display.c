#include "sys.h"
#include "oled.h"
#include "display.h"

#include "mysprintf.h"

#include <math.h>

#include "sp37data.h"

// 这个文件属于上层

void Display_Http(void)		// 显示官网信息
{
// HTTP_START_X起始的横坐标，HTTP_START_PAGE起始的纵坐标，"www.songxiaohui.com"要显示的字符串信息
	OLED_ShowStr(HTTP_START_X,HTTP_START_PAGE,"www.songxiaohui.com");
}

void Display_Version(void)	// 显示版本号
{
	//OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE,"SP37 Receiver V2.5");
	//OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE," .Send 125Khz LF. ");
	OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE,"RF  V1.0");
}


#define LOGO_MOVE_TO_RIGHT	0
#define LOGO_MOVE_TO_LEFT	1	



//默认16个数据
//没有更新的地方为0
void Display_Data(u8 *data)		// 显示数据
{
	u8 i;

	char strArr[32]={0};
	char *str = strArr;

	// 第一行数据，说白了就是往strArr字符数组中填充数据，数据填充完了，
	// 将数据传入OLED_ShowStr函数中，然后在OLED屏幕上显示出来
	*str = 'D';	str++;
	*str = '0';	str++;
	*str = ':';	str++;

	for(i=0;i<8;i++){
		str += MySprintf_x(str,data[i]);	// MySprintf_x函数是将data[i]十进制的数据转换为str字符串
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

//显示" Re: "  这个三个字符串，这个是刚开机时显示的，也就是还没有传输数据时显示的，也就是显示为0；MySprintf_ud函数中的第二个传参
void Display_RF_Receive_Mark(void)
{
	// 字符串存放在栈上
	char strArr[32]={0};
	char *str = strArr;

	*str = 'R';	str++;
	*str = 'e';	str++;
	*str = ':';	str++;

	MySprintf_ud(str,0,4);
	OLED_ShowStr(RF_RECEIVE_MARK_START_X,RF_RECEIVE_MARK_PAGE,strArr);	
}

// 显示Re: 后面的数字，累积增加的，也就是告诉我们当前收到了多少包数据
void Display_RF_Receive_Count_Plus(void)
{
	char strArr[32]={0};
	char *str = strArr;

	static u16 Count = 0;

	Count++;
	if(Count > 9999){
		Count = 0;
	}
	
	MySprintf_ud(str,Count,4);
	OLED_ShowStr(RF_RECEIVE_COUNT_START_X,RF_RECEIVE_COUNT_PAGE,strArr);
}

//在没有收到ID之前，显示XXXX，当接收到一包数据后，xxxx变为相应的数据包值
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



//在没有收到ID之前，显示XXXXXXXX
void Display_Unique_ID_NULL(void)
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

	OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);

}

void Display_Unique_ID(u8 *UniqueID)
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

	OLED_ShowStr(UNIQUE_ID_START_X,UNIQUE_ID_START_PAGE,strArr);
}


void Display_Tyre_Position(u8 tyrePosition)
{
	char strArr[32]={0};
	char *str = strArr;

	u8 lastTyrePosition = Unknown;

	if(lastTyrePosition==tyrePosition){
		return;
	}else{
		lastTyrePosition = tyrePosition;
	}

	switch(tyrePosition){
	case LF:
		*str = 'L';	str++;
		*str = 'F';	str++;
		break;
	case RF:
		*str = 'R'; str++;
		*str = 'F'; str++;
		break;
	case LB:
		*str = 'L'; str++;
		*str = 'B'; str++;
		break;
	case RB:
		*str = 'R'; str++;
		*str = 'B'; str++;
		break;
	default:break;
	}


	OLED_ShowStr(TYRE_POSOTION_START_X,TYRE_POSOTION_START_PAGE,strArr);
}


void Display_Pressure(float pressure)
{
	// 将字符串存放在栈上
	char strArr[32]={0};		// 定义字符数组
	char *str = strArr;			// 定义字符串

	static float lastVal = -1;
	if(fabs(lastVal - pressure) < 0.0001 ){
		return;
	}else{
		lastVal = pressure;
	} 
	// *str = 'P';是往str所指向的地址中存数据
	*str = 'P';	str++;	// str++就是把str指向的地址往后移一个格子，为后面的字符填充做准备
	*str = ':';	str++;
	// 下面代码解析: str是指现在所指向的地址，pressure是测到的压力值，
	str += MySprintf_f(str,pressure,3,4);		// pressure参数是接收到的数据

	*str = ' ';	str++;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;

	OLED_ShowStr(PRESSURE_START_X,PRESSURE_START_PAGE,strArr);
	
}


void Display_Temperature(float temperature)
{
	char strArr[32]={0};
	char *str = strArr;

	static float lastVal = -1;
	if(fabs(lastVal - temperature) < 0.0001 ){
		return;
	}else{
		lastVal = temperature;
	}

	*str = 'T';	str++;
	*str = ':';	str++;

	str += MySprintf_f(str,temperature,3,4);

	*str = ' ';	str++;
	
	*str = 95+32;	str++;		//特殊字符℃

	OLED_ShowStr(TEMPERATURE_START_X,TEMPERATURE_START_PAGE,strArr);
	
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



