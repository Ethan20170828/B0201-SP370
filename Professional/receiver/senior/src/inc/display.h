#ifndef _DISPLAY_H
#define _DISPLAY_H

#define FBMPXsize				16
#define FBMPYsize				16

#define LOGO_START_X 			0
#define LOGO_START_PAGE 		0

#define HTTP_START_X			19
#define HTTP_START_PAGE			1

#define VERSION_START_X			0
#define VERSION_START_PAGE		0


#define USER_ID_START_X			85
#define USER_ID_START_PAGE		2

//显示4个ID,所以一些坐标需要修整
//UniqueID与UserID不可同时显示(因为占用屏幕的同一个区域)
#define UNIQUE_ID_START_X		61
#define UNIQUE_ID_START_PAGE	0

#define TYRE_POSOTION_START_X	 61
#define TYRE_POSOTION_START_PAGE 3


/*
#define LOGO_MOVE_X_MIN			0
#define LOGO_MOVE_X_MAX			(USER_ID_START_X - FBMPXsize)
#define LOGO_MOVE_PAGE			2
*/
#define LOGO_MOVE_X_MIN			0
#define LOGO_MOVE_X_MAX			(UNIQUE_ID_START_X - FBMPXsize)
#define LOGO_MOVE_PAGE			2


#define DATA_START_X				0
#define DATA_START_PAGE1			4		//占据两行
#define DATA_START_PAGE2			5		//占据两行



//接收到的数据包数目
//接收到的数据包数目
#define RF_RECEIVE_MARK_START_X		85		
#define RF_RECEIVE_MARK_PAGE		3


#define RF_RECEIVE_COUNT_START_X	103		
#define RF_RECEIVE_COUNT_PAGE		3


#define SP37DATA_START_X			20
#define SP37DATA_FIRST_PAGE			4

#define PRESSURE_START_X			(SP37DATA_START_X)
#define PRESSURE_START_PAGE			(SP37DATA_FIRST_PAGE)

#define TEMPERATURE_START_X			(SP37DATA_START_X)
#define TEMPERATURE_START_PAGE		(SP37DATA_FIRST_PAGE + 1)

#define ACCELERATION_START_X		(SP37DATA_START_X)
#define ACCELERATION_START_PAGE		(SP37DATA_FIRST_PAGE + 2)

#define VOLTAGE_START_X				(SP37DATA_START_X)
#define VOLTAGE_START_PAGE			(SP37DATA_FIRST_PAGE + 3)

void Display_Logo(void);
void Display_Http(void);
void Display_Version(void);

//void Display_Logo_Move(void);


void Display_Data(u8 *data);

void Display_RF_Receive_Mark(void);
void Display_RF_Receive_Count_Plus(void);

void Display_User_ID_NULL(void);
void Display_User_ID(u8 IDH,u8 IDL);
void Display_Unique_ID_NULL1(void);
void Display_Unique_ID_NULL2(void);
void Display_Unique_ID_NULL3(void);
void Display_Unique_ID_NULL4(void);
void Display_Unique_ID1(u8 *UniqueID);
void Display_Unique_ID2(u8 *UniqueID);
void Display_Unique_ID3(u8 *UniqueID);
void Display_Unique_ID4(u8 *UniqueID);

void Display_Pressure1(float pressure);
void Display_Pressure2(float pressure);
void Display_Pressure3(float pressure);
void Display_Pressure4(float pressure);
void Display_Temperature1(float temperature);
void Display_Temperature2(float temperature);
void Display_Temperature3(float temperature);
void Display_Temperature4(float temperature);
void Display_Acceleration(float acceleration);
void Display_Voltage(float voltage);



#endif


