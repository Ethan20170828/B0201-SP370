/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                          ziotlab 出品                                      */
/*   ----------------------------------------------------------------------------------------------   */
/* ************************************************************************************************** */


#include "sys.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tim.h"

#include "oled.h"
#include "display.h"

#include "TDA5235.h"
#include "TDA5235_SFR.h"

#include "SP37Data.h"


/* ***************************************************************************** */
/* 此例程验证SP370的RF功能                                                   */
/* 上电后不断发射"符合SP370"的报文(10Hz)                                         */
/* 如果有接收到来自SP370的RF数据，将在屏幕上显示,并且Printf接收到的数据          */
/* ***************************************************************************** */

int main(void)
{
	Clock_Init();
	NVIC_Configuration();		//  中断配置
	delay_init();
	LED_Init();
	USART1_Init(115200);		//  USART1初始化,波特率115200	

	Tim3_Init(1000,64);         // 	1ms的溢出中断 //系统灯闪烁，用来检测我们的胎压是否正常显示，是否死机
	TIM3_Cmd(ENABLE);

	OLED_Init();

	// 下面的几个Display_xxx函数，目的是开机时先初始化这些
	//Display_Logo();  
	Display_Http();
	Display_Version();
	// 上面两个和下面两个Display_xxx函数，上电后值是不变的，所以先将这个四个初始化了，初始化的顺序就是屏幕中显示的先后顺序
	//Display_User_ID_NULL();
	Display_Unique_ID_NULL();
	Display_RF_Receive_Mark();
	// 下面的这个四个Display_xxx，先将所有的值赋值为0，等待数据的传递，每传递一个数据，在屏幕上相应的位置刷新一下屏幕
	Display_Pressure(0);
	Display_Temperature(0);
	Display_Acceleration(0);
	Display_Voltage(0);

	// 对TDA5235芯片进行初始化以及对芯片内部的寄存器进行初始化
	TDA5235_Init();
	TDA5235_SFR_Init();

	// 以上部分是将接收端的所有硬件都进行了初始化
	while(1)	// while循环中每循环一次就是处理一包数据
	{
		//如果有接收到数据(也就是TDA5235_FIFO_NUM不等于0),进行处理；接收的数据是从发射端发射过来的数据
		if(TDA5235_FIFO_NUM){	// 在中断处理程序中会对TDA5235_FIFO_NUM进行赋值(TDA5235通过一个GPIO来给我们一个中断，
								// 在中断处理程序中读取TDA5235的FIFO，读完之后我们在根据读取到的FIFO的内容来给TDA5235_FIFO_NUM赋值)来影响我们这里的while(1)
			PT_SP37_RF_DATA pt_SP37Data = (PT_SP37_RF_DATA)TDA5235_FIFO;	// 将TDA5235_FIFO强制类型，然后赋值给pt_SP37Data

			LED_ReceiveReverse();				//STATUS灯闪烁
			Display_RF_Receive_Count_Plus();	// 显示一共接收了多少包数据
			//Display_Logo_Move();

			//Display_User_ID(pt_SP37Data->IDH,pt_SP37Data->IDL);
			Display_Unique_ID(pt_SP37Data->UniqueID);
			Display_Tyre_Position(pt_SP37Data->TyrePosition);
			Display_Pressure(SP37_Cal_Pressure_V2(pt_SP37Data->Pressure));
			Display_Temperature(SP37_Cal_Temperature(pt_SP37Data->Temperature));
			Display_Acceleration(SP37_Cal_Acceleration(pt_SP37Data->Acceleration));
			Display_Voltage(SP37_Cal_Voltage(pt_SP37Data->Voltage));

			printf("*****************************\r\n");
			printf("Get a message from SP370:\r\n");
			printf("UniqueID:    %02X%02X%02X%02X\r\n",	pt_SP37Data->UniqueID[0],
														pt_SP37Data->UniqueID[1],
														pt_SP37Data->UniqueID[2],
														pt_SP37Data->UniqueID[3]);
			printf("TyrePosition:%d\r\n",pt_SP37Data->TyrePosition);
			printf("Pressure:    %f\r\n",SP37_Cal_Pressure_V2(pt_SP37Data->Pressure));
			printf("Temperature: %f\r\n",SP37_Cal_Temperature(pt_SP37Data->Temperature));
			printf("Acceleration:%f\r\n",SP37_Cal_Acceleration(pt_SP37Data->Acceleration));
			printf("Voltage:     %f\r\n",SP37_Cal_Voltage(pt_SP37Data->Voltage));
			printf("*****************************\r\n");
			TDA5235_FIFO_NUM = 0;	//清零
		}
	}
}


/*	头文件路径
.\CORE;..\FWLib\inc;.\INCLUDE
*/

