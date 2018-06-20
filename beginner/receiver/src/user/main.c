/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                          ziotlab ��Ʒ                                      */
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
/* ��������֤SP370��RF����                                                   */
/* �ϵ�󲻶Ϸ���"����SP370"�ı���(10Hz)                                         */
/* ����н��յ�����SP370��RF���ݣ�������Ļ����ʾ,����Printf���յ�������          */
/* ***************************************************************************** */

int main(void)
{
	Clock_Init();
	NVIC_Configuration();		//  �ж�����
	delay_init();
	LED_Init();
	USART1_Init(115200);		//  USART1��ʼ��,������115200	

	Tim3_Init(1000,64);         // 	1ms������ж� //ϵͳ����˸������������ǵ�̥ѹ�Ƿ�������ʾ���Ƿ�����
	TIM3_Cmd(ENABLE);

	OLED_Init();

	// ����ļ���Display_xxx������Ŀ���ǿ���ʱ�ȳ�ʼ����Щ
	//Display_Logo();  
	Display_Http();
	Display_Version();
	// ������������������Display_xxx�������ϵ��ֵ�ǲ���ģ������Ƚ�����ĸ���ʼ���ˣ���ʼ����˳�������Ļ����ʾ���Ⱥ�˳��
	//Display_User_ID_NULL();
	Display_Unique_ID_NULL();
	Display_RF_Receive_Mark();
	// ���������ĸ�Display_xxx���Ƚ����е�ֵ��ֵΪ0���ȴ����ݵĴ��ݣ�ÿ����һ�����ݣ�����Ļ����Ӧ��λ��ˢ��һ����Ļ
	Display_Pressure(0);
	Display_Temperature(0);
	Display_Acceleration(0);
	Display_Voltage(0);

	// ��TDA5235оƬ���г�ʼ���Լ���оƬ�ڲ��ļĴ������г�ʼ��
	TDA5235_Init();
	TDA5235_SFR_Init();

	// ���ϲ����ǽ����ն˵�����Ӳ���������˳�ʼ��
	while(1)	// whileѭ����ÿѭ��һ�ξ��Ǵ���һ������
	{
		//����н��յ�����(Ҳ����TDA5235_FIFO_NUM������0),���д������յ������Ǵӷ���˷������������
		if(TDA5235_FIFO_NUM){	// ���жϴ�������л��TDA5235_FIFO_NUM���и�ֵ(TDA5235ͨ��һ��GPIO��������һ���жϣ�
								// ���жϴ�������ж�ȡTDA5235��FIFO������֮�������ڸ��ݶ�ȡ����FIFO����������TDA5235_FIFO_NUM��ֵ)��Ӱ�����������while(1)
			PT_SP37_RF_DATA pt_SP37Data = (PT_SP37_RF_DATA)TDA5235_FIFO;	// ��TDA5235_FIFOǿ�����ͣ�Ȼ��ֵ��pt_SP37Data

			LED_ReceiveReverse();				//STATUS����˸
			Display_RF_Receive_Count_Plus();	// ��ʾһ�������˶��ٰ�����
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
			TDA5235_FIFO_NUM = 0;	//����
		}
	}
}


/*	ͷ�ļ�·��
.\CORE;..\FWLib\inc;.\INCLUDE
*/

