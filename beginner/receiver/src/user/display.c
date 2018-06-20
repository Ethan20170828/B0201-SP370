#include "sys.h"
#include "oled.h"
#include "display.h"

#include "mysprintf.h"

#include <math.h>

#include "sp37data.h"

// ����ļ������ϲ�

void Display_Http(void)		// ��ʾ������Ϣ
{
// HTTP_START_X��ʼ�ĺ����꣬HTTP_START_PAGE��ʼ�������꣬"www.songxiaohui.com"Ҫ��ʾ���ַ�����Ϣ
	OLED_ShowStr(HTTP_START_X,HTTP_START_PAGE,"www.songxiaohui.com");
}

void Display_Version(void)	// ��ʾ�汾��
{
	//OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE,"SP37 Receiver V2.5");
	//OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE," .Send 125Khz LF. ");
	OLED_ShowStr(VERSION_START_X,VERSION_START_PAGE,"RF  V1.0");
}


#define LOGO_MOVE_TO_RIGHT	0
#define LOGO_MOVE_TO_LEFT	1	



//Ĭ��16������
//û�и��µĵط�Ϊ0
void Display_Data(u8 *data)		// ��ʾ����
{
	u8 i;

	char strArr[32]={0};
	char *str = strArr;

	// ��һ�����ݣ�˵���˾�����strArr�ַ�������������ݣ�����������ˣ�
	// �����ݴ���OLED_ShowStr�����У�Ȼ����OLED��Ļ����ʾ����
	*str = 'D';	str++;
	*str = '0';	str++;
	*str = ':';	str++;

	for(i=0;i<8;i++){
		str += MySprintf_x(str,data[i]);	// MySprintf_x�����ǽ�data[i]ʮ���Ƶ�����ת��Ϊstr�ַ���
	}

	OLED_ShowStr(DATA_START_X,DATA_START_PAGE1,strArr);

	//�ڶ�������
	str = strArr;
	*str = 'D';	str++;
	*str = '1';	str++;
	*str = ':';	str++;

	for(i=8;i<16;i++){
		str += MySprintf_x(str,data[i]);
	}

	OLED_ShowStr(DATA_START_X,DATA_START_PAGE2,strArr);	
	
}

//��ʾ" Re: "  ��������ַ���������Ǹտ���ʱ��ʾ�ģ�Ҳ���ǻ�û�д�������ʱ��ʾ�ģ�Ҳ������ʾΪ0��MySprintf_ud�����еĵڶ�������
void Display_RF_Receive_Mark(void)
{
	// �ַ��������ջ��
	char strArr[32]={0};
	char *str = strArr;

	*str = 'R';	str++;
	*str = 'e';	str++;
	*str = ':';	str++;

	MySprintf_ud(str,0,4);
	OLED_ShowStr(RF_RECEIVE_MARK_START_X,RF_RECEIVE_MARK_PAGE,strArr);	
}

// ��ʾRe: ��������֣��ۻ����ӵģ�Ҳ���Ǹ������ǵ�ǰ�յ��˶��ٰ�����
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

//��û���յ�ID֮ǰ����ʾXXXX�������յ�һ�����ݺ�xxxx��Ϊ��Ӧ�����ݰ�ֵ
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



//��û���յ�ID֮ǰ����ʾXXXXXXXX
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
	// ���ַ��������ջ��
	char strArr[32]={0};		// �����ַ�����
	char *str = strArr;			// �����ַ���

	static float lastVal = -1;
	if(fabs(lastVal - pressure) < 0.0001 ){
		return;
	}else{
		lastVal = pressure;
	} 
	// *str = 'P';����str��ָ��ĵ�ַ�д�����
	*str = 'P';	str++;	// str++���ǰ�strָ��ĵ�ַ������һ�����ӣ�Ϊ������ַ������׼��
	*str = ':';	str++;
	// ����������: str��ָ������ָ��ĵ�ַ��pressure�ǲ⵽��ѹ��ֵ��
	str += MySprintf_f(str,pressure,3,4);		// pressure�����ǽ��յ�������

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
	
	*str = 95+32;	str++;		//�����ַ���

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



