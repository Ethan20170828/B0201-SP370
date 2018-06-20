/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                         ziotlab ��Ʒ                                    		    */
/*   ----------------------------------------------------------------------------------------------   */


#include "sys.h"
#include "clock.h"
#include "delay.h"

#include "led.h"
#include "tim.h"

#include "oled.h"
#include "display.h"

#include "TDA5235.h"
#include "TDA5235_SFR.h"

#include "SP37Data.h"

//#include "lf.h"
#include "usart.h"
#include "CRC.h"
#include "stm32f10x.h" 
#include "stm32f10x_flash.h" //flash�����ӿ��ļ����ڿ��ļ��У�������Ҫ���� 

#define  Tire_LF    1
#define  Tire_RF    2
#define  Tire_LB    3
#define  Tire_RB    4
#define START_ID_ADDR	0x08000000+2048*30																											//flash �б�������
#define Save_ID_LF_ADDR 0
#define Save_ID_RF_ADDR 4
#define Save_ID_LB_ADDR 8
#define Save_ID_RB_ADDR 12


/**********************************Э����ͨѶ�շ�������***********************************/
unsigned char Tx_Data_To_Dvd[8] = 	{0x55,0xaa,0x08,0x00,0x00,0x00,0x00,0x00}; 								
unsigned char Rx_CMD_Form_Dvd[6] = 	{0}; 
unsigned char Ack_To_Dvd[6]  = 		{0x55,0xaa,0x06,0x10,0x00,0x00}; 
unsigned char Ack_OK_To_Dvd[6] = 	{0x55,0xaa,0x06,0x18,0x00,0x00}; 
unsigned char Tx_ID_To_Dvd[9] = 	{0x55,0xaa,0x09,0x00,0x00,0x00,0x00,0x00,0x00}; 
unsigned char Tx_V_To_Dvd[10] = 	{0x55,0xaa,0x0A,0x20,0x01,0x00,0x00,0x00,0x00,0x00}; 
unsigned char Replace_Tire[7] = 	{0};
unsigned char Replace_Tire_Ack[7] = {0x55,0xaa,0x07,0x30,0x00,0x00,0x00};
/**********************************Э����ͨѶ�շ�������***********************************/


unsigned char Study_Mode_Flag=0;	//����1�ǽ���ѧϰģʽ�������ǽ��뵽����ģʽ
unsigned char Study_Dir=0;			//����ѧϰģʽ֮����Ҫѧϰ���Ǹ���̥
unsigned char Status_Tire[4]={0};	//ÿ����̥״̬��¼
unsigned char Signal_Check[4]={0};	//���ÿ����̥�Ƿ��жϿ�����

T_SP37_RF_DATA pt_SP37Data[4];      //����4����̥����


/**********************************Э��ʵ�ֹ��ܺ���***********************************/
void Data_Process(void);
void Check_ID(void);
void Check_V(void);
void Enter_Study_Mode(unsigned char switch_Tire);
void Exit_Study_Mode(void);
void Tire_Change_Location(unsigned char Change_Dir1,unsigned char Change_Dir2);
void Tire_Change_Location_ACK(unsigned char ack1,unsigned char ack2);
void Send_Tire_Data_To_Dvr(void);
void Save_ID_Func(PT_SP37_RF_DATA Cache);
void Get_Save_Flash_ID(void);
void WriteFlashNByte(u32 WriteAddress,u32 *WriteData,u8 NUM) ;
int ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum) ;
void Save_ID_Flash(void);
/**********************************Э��ʵ�ֹ��ܺ���***********************************/


int main(void)
{
	unsigned char i,j,ret;

	Clock_Init();
	NVIC_Configuration();		//�ж�����

	// �տ�ʼ�ϵ��ʱ�����ʱһ��ʱ��(������Ļ��˸��������)��ԭ������main�����е�����delay_init����
	delay_init();
	// ��Ļ��˸������ LED�ƿ�ʼ����
    LED_Init();
    
	USART1_Init(19200);			//USART1��ʼ��,������19200	

// Tim3�Ƕ�ʱ������ʱ4S���пط�������
	Tim3_Init(50000,64);        // 50ms������ж� //ϵͳ����˸
	TIM3_Cmd(ENABLE);
//	TIM3_Cmd(DISABLE);


    OLED_Init(); 
// OLED��Ļ��ʼ�����ˣ�����Ļ�Ͽ��Կ���������
// ���Կ�����վ���汾����Ϣ�Լ���̥��ID��
    Display_Http();
	Display_Version();

	// �տ�ʼ�����ϵ�ʱ�����ĸ���̥��ID��ʾ����00000000
    Display_Unique_ID1(pt_SP37Data[0].UniqueID);
    Display_Unique_ID2(pt_SP37Data[1].UniqueID);
    Display_Unique_ID3(pt_SP37Data[2].UniqueID);
    Display_Unique_ID4(pt_SP37Data[3].UniqueID);

// ���ն˵�TDA5235���г�ʼ��
	TDA5235_Init();
// ���ն˵�TDA5235�ļĴ������г�ʼ��
	TDA5235_SFR_Init();

	// ��STM32�ڲ���flash�ж�ȡ��ԭ���洢��flash�е�ID��
	// �����4��ID������ѧϰ�󣬽��ն˺�4������˽������������ˣ����ն˳�����4������˵�16���ֽڵ�
	// ΨһID�浽flash�У�ѧϰ��ʱ����ǰ���̥��ΨһID�浽���ն˵�STM32��flash�У����յ�һ��SP370����
	// �ݺ���洢����ID���Ӧ�������Ƿ�һ�¡�
	Get_Save_Flash_ID();

// whileѭ�����Ǵ�����ն˺��пصĹ�ͨ�����ն˺ͷ���˵Ĺ�ͨ�Ĵ���
	while(1)
	{	
		// ������յ��п������ֱ�ӽ�������Ӧ���룻
		// ���ն˺��п�֮��Ĺ�ͨ��
		// �����н��յ�һ֡�������ݺ󣬾��ǰ�ReceiveStateȫ�ֱ���ֵ��Ϊ1�����뵽if�У�
		// �пظ����ն˷���Э��ָ����ReceiveState=1����ʾ���ڽ��յ�Э��ָ�
		// ����пظ�����˷��͵�Э��ָ��ԣ�����ն�û����Ӧ�����������Ӧ��ģʽ��
		// �пػ���յ����ն˼�⵽��̥ѹ������ֵ��
		if(ReceiveState == 1)
		{
			// ReceiveState����0����ʾ�п�û�и����ն˷��������ˣ�Ȼ����ն˶��п�
			// ����������һ������(ָ��)���н��������ж�����Ҫ��������ģʽ(Ʃ��ѧϰģʽ����ѯIDģʽ��)
			ReceiveState = 0;

			// ���Э����з���
			// ����������ǶԴ��ڽ��յ��пط�������һ֡���ݽ��н���������
			Data_Process();
		}
		// ��һ����ͨ��SP370��TDA5235����ͨ�ţ�
		// ���TDA5235�н��յ����������,���д���
		// ���ն˺ͷ����֮��Ĺ�ͨ��
		// TDA5235_FIFO_NUM�����ڣ���ʾ���ն˽��յ��˷���˷��͵����ݣ����Ҵ�ŵ����ն˵Ļ�������
		if(TDA5235_FIFO_NUM){
			// ����˷��͵����ն˵�����(TDA5235_FIFO)��ֵ��Cache_SP37Data
			PT_SP37_RF_DATA Cache_SP37Data = (PT_SP37_RF_DATA)TDA5235_FIFO;
#if 1
			// ���ն˵���������ģʽ�����ն˴ӷ����SP370��������̥ѹ���ݣ�����ʾ����������ʾ����
			if(!Study_Mode_Flag)		// ��������ģʽ
			{
				for(i=0;i<4;i++)
				{
					for(j=0;j<4;j++)
					{
						// pt_SP37Data[i].UniqueID[j]��������ѧϰ����ĸ���̥��ID
						// Cache_SP37Data->UniqueID[j]�ո��յ����Ǹ���̥��ID
						if(Cache_SP37Data->UniqueID[j] == pt_SP37Data[i].UniqueID[j])
						{
							ret = TURE;
						}
						else
						{
							ret = FLASE;
							break;
						}
					}
					if(ret == TURE)
					{	
						Signal_Check[i] = 1;
						pt_SP37Data[i].UniqueID[0]	= Cache_SP37Data->UniqueID[0];
						pt_SP37Data[i].UniqueID[1]	= Cache_SP37Data->UniqueID[1];
						pt_SP37Data[i].UniqueID[2]	= Cache_SP37Data->UniqueID[2];
						pt_SP37Data[i].UniqueID[3]	= Cache_SP37Data->UniqueID[3];
						pt_SP37Data[i].Check_Trip_status	= Cache_SP37Data->Check_Trip_status;
						pt_SP37Data[i].Pressure[0]		= Cache_SP37Data->Pressure[0];
						pt_SP37Data[i].Pressure[1]		= Cache_SP37Data->Pressure[1];
						pt_SP37Data[i].Temperature[0]	= Cache_SP37Data->Temperature[0];
						pt_SP37Data[i].Temperature[1]	= Cache_SP37Data->Temperature[1];
						pt_SP37Data[i].Acceleration[0]	= Cache_SP37Data->Acceleration[0];
						pt_SP37Data[i].Acceleration[1]	= Cache_SP37Data->Acceleration[1];
						pt_SP37Data[i].Voltage[0]		= Cache_SP37Data->Voltage[0];
						pt_SP37Data[i].Voltage[1]		= Cache_SP37Data->Voltage[1];
						pt_SP37Data[i].CRC8			= Cache_SP37Data->CRC8;
						break;
					}
				}
				#if 0
				printf("UniqueID0:    %02X%02X%02X%02X\r\n",	pt_SP37Data[0].UniqueID[0],
															pt_SP37Data[0].UniqueID[1],
															pt_SP37Data[0].UniqueID[2],
															pt_SP37Data[0].UniqueID[3]);
				
				printf("UniqueID1:    %02X%02X%02X%02X\r\n",	pt_SP37Data[1].UniqueID[0],
															pt_SP37Data[1].UniqueID[1],
															pt_SP37Data[1].UniqueID[2],
															pt_SP37Data[1].UniqueID[3]);
				printf("UniqueID2:    %02X%02X%02X%02X\r\n",	pt_SP37Data[2].UniqueID[0],
															pt_SP37Data[2].UniqueID[1],
															pt_SP37Data[2].UniqueID[2],
															pt_SP37Data[2].UniqueID[3]);
				printf("UniqueID3:    %02X%02X%02X%02X\r\n",	pt_SP37Data[3].UniqueID[0],
															pt_SP37Data[3].UniqueID[1],
															pt_SP37Data[3].UniqueID[2],
															pt_SP37Data[3].UniqueID[3]);
				#endif
                Display_Unique_ID1(pt_SP37Data[0].UniqueID);
                Display_Unique_ID2(pt_SP37Data[1].UniqueID);
                Display_Unique_ID3(pt_SP37Data[2].UniqueID);
                Display_Unique_ID4(pt_SP37Data[3].UniqueID);
			}
			// ���ն���ѧϰģʽ��
			else			// ѧϰģʽ
			{
				// ���Э�����ѧϰ
				// ��ѧϰģʽ������ѧϰ��SP370��ΨһID��������
				// ѧϰģʽ�£�����˸����ն˴��ݵ���Ϣ��ֻ����̥��ID��
				Save_ID_Func(Cache_SP37Data);
			}
#endif

			LED_ReceiveReverse();		//STATUS����˸
			TDA5235_FIFO_NUM = 0;	//���㣬��ʾһ�����ݷ������£��ȴ�����˷�����һ������
		}
	}
}

// ��ʱ�������Ѿ�ͨ�����ն˷��͵����ڵ�buffer����
// ��������Ƕ��п�����ն˷��͵�Э����н�������
void Data_Process(void)
{
	unsigned char i;
	unsigned char temp;
/*
	USART_SendData(USART1, 0x66);
	USART_SendData(USART1, USART1_RxBuffer[0]);
	USART_SendData(USART1, USART1_RxBuffer[1]);
	USART_SendData(USART1, USART1_RxBuffer[2]);
	USART_SendData(USART1, USART1_RxBuffer[3]);
	USART_SendData(USART1, USART1_RxBuffer[4]);
	USART_SendData(USART1, USART1_RxBuffer[5]);
*/
// ���ж�Э���е�ͬ���Ƿ���ȡ�
	if ((USART1_RxBuffer[0] == 0x55) && (USART1_RxBuffer[1] == 0xaa))
	{
		// USART1_RxBuffer���ڻ�������Ϊ��ʼ��USART1_RX_NUM��Ϊ����
		// ���У���ʮ�����������ԣ�CRC_Calc�����ķ���ֵ��FLASE
		// ��ô����ʾ��Ļ����ʾ1
		if (FLASE == CRC_Calc(USART1_RxBuffer, USART1_RX_NUM))
		{
            pt_SP37Data[0].UniqueID[0] = 0x1;
            Display_Unique_ID1(pt_SP37Data[0].UniqueID);
			return;
		}

		for (i=0; i<USART1_RX_NUM; i++)
		{
			// �п�����ն˷���6�ֽ�ָ������
			if (USART1_RX_NUM == 6) 
			{
				// �п�����ն˷���ָ�ָ���ȴ洢�����ڻ�������
				// ��һ�����ǽ��洢�����ڻ������е�ָ��ŵ�Rx_CMD_Form_Dvdȫ�ֲ�����������
				Rx_CMD_Form_Dvd[i] = USART1_RxBuffer[i];
			}
			// �п�����ն˷���7�ֽ�ָ������
			else if(USART1_RX_NUM == 7)
			{
				// ��̥����
				// �п�����ն˷���ָ�ָ���ȴ洢�����ڻ�������
				// ��һ�����ǽ��洢�����ڻ������е�ָ��ŵ�Replace_Tireȫ�ֱ���������
				Replace_Tire[i] = USART1_RxBuffer[i];
			}
		}

		if(USART1_RX_NUM == 6)
		{
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x06 0x07 0x00����ʾ���ն˽����ѯ�ĸ���������IDģʽ
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x07)&&(Rx_CMD_Form_Dvd[4] == 0x00))
			{
				// USART_SendData(USART1, 0x77);
				// ��ѯ4����������ID
				Check_ID();
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x06 0x02 0x01����ʾ���ն˽����ѯ�ĸ��������ĵ�ѹģʽ
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x02)&&(Rx_CMD_Form_Dvd[4] == 0x01))
			{
				// ��ѯ��ѹ
				Check_V();
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x06 0x06 0x00����ʾ���ն˽����Ƴ���������ѧϰģʽ
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x06)&&(Rx_CMD_Form_Dvd[4] == 0x00))
			{
				// �Ƴ���������ѧϰģʽ
				Exit_Study_Mode();
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x06 0x01����ʾ���ն˽��봫������ѧϰģʽ
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x01))
			{
				temp = Rx_CMD_Form_Dvd[4];
				// ����ѧϰ״̬������temp��ʾ��Ҫѧϰ�ĸ���̥
				// Rx_CMD_Form_Dvd�ĵ���λ��00��ʾ��ǰ��01��ʾ��ǰ��10��ʾ���11��ʾ�Һ�
				Enter_Study_Mode(temp);
			}				
		}
		// ���ڣ�������̥����
		if(USART1_RX_NUM == 7)
		{
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x00 0x01����ʾ���ն˽�����ǰ����ǰ�Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x01))
			{
				Tire_Change_Location(Tire_LF,Tire_RF);
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x00 0x10����ʾ���ն˽�����ǰ�����Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x10))
			{
				Tire_Change_Location(Tire_LF,Tire_LB);
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x00 0x11����ʾ���ն˽�����ǰ���Һ�Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x11))
			{
				Tire_Change_Location(Tire_LF,Tire_RB);
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x01 0x10����ʾ���ն˽�����ǰ�����Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x01)&&(Replace_Tire[5] == 0x10))
			{
				Tire_Change_Location(Tire_RF,Tire_LB);
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x01 0x11����ʾ���ն˽�����ǰ���Һ�Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x01)&&(Replace_Tire[5] == 0x11))
			{				
				Tire_Change_Location(Tire_RF,Tire_RB);
			}
			// ��λ�����͸����ն˵�ָ���� 0x55 0xaa 0x07 0x03 0x10 0x11����ʾ���ն˽��������Һ�Ե�ģʽ
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x10)&&(Replace_Tire[5] == 0x11))
			{				
				Tire_Change_Location(Tire_LB,Tire_RB);
			}
			// ��̥����λ������һ����Ӧ����Ҫ��ָ���bit4��bit5
			Tire_Change_Location_ACK(Replace_Tire[4],Replace_Tire[5]);
		}		
	}

	for(i=0;i<USART1_RX_NUM;i++)
	{
		Replace_Tire[i]=0;
	}	
	USART1_RX_NUM = 0;
}

void Tire_Change_Location_ACK(unsigned char ack1,unsigned char ack2)
{
	unsigned char i,tx_num;	
	
	Replace_Tire_Ack[4] = ack1;
	Replace_Tire_Ack[5] = ack2;
	Replace_Tire_Ack[6] = Calculate_CRC(Replace_Tire_Ack,6);

	tx_num=7;
	i=0;
	while(tx_num--)
	{
			USART_SendData(USART1,Replace_Tire_Ack[i++]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}

void Enter_Study_Mode(unsigned char switch_Tire)
{
	unsigned char i,tx_num;
	
	Study_Mode_Flag = 1;
	// switch_Tire��ʾ�㴫�ݹ������ĸ���̥��ͨ��switch case�������ж�
	switch (switch_Tire)
	{
		case 0x00:
				Study_Dir = Tire_LF;	// ��ǰ
			break;
		case 0x01:
				Study_Dir = Tire_RF;	// ��ǰ
			break;
		case 0x10:
				Study_Dir = Tire_LB;	// ���
			break;
		case 0x11:
				Study_Dir = Tire_RB;	// �Һ�
			break;
		default:
				Study_Dir = 0;
			break;
	}
	// Ack_To_Dvd��ʾ���ն����пػظ����ݣ�bit4��ʾ�ظ���ѧϰ���Ǹ���̥
	Ack_To_Dvd[4] = switch_Tire;
	// bit5��CRCУ����
	Ack_To_Dvd[5] = Calculate_CRC(Ack_To_Dvd,5);

// ���ն�ͨ�����������ط���ָ��
	tx_num=6;
	i=0;
	while(tx_num--)
	{
			USART_SendData(USART1,Ack_To_Dvd[i++]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}

void Exit_Study_Mode(void)
{
	unsigned char i,tx_num;
	unsigned char Ack_Exit_Study[6]={0};
	Study_Mode_Flag=0;

// �пظ����ն˷������ѧϰָ��󣬽��ն˻���пط���һ��ָ��
	Ack_Exit_Study[0] = 0x55;
	Ack_Exit_Study[1] = 0xaa;
	Ack_Exit_Study[2] = 0x06;
	Ack_Exit_Study[3] = 0x16;
	Ack_Exit_Study[4] = 0x00;
	Ack_Exit_Study[5] = Calculate_CRC(Ack_Exit_Study,5);

// �����ǽ��ն˵�ָ��ͨ�����ڷ��͸��пأ���Check_ID��һ����	
	tx_num=6;
	i=0;
	while(tx_num--)
	{
			USART_SendData(USART1,Ack_Exit_Study[i++]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}


volatile FLASH_Status FLASHStatus = FLASH_COMPLETE; //Flash����״̬���� 

// ��������ǵ��͵�STM32����Flashд���ݵķ�װ
// ��ȡFlash��N��Byte
// ReadAddress�Ƕ�ȡFlash����һ����ַ
// ReadBuf����ַ��ȡ��ʲô�ط�ȥ
// ReadNum��ȡ���ٸ��ֽ�
int ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum) 
{ 
	int DataNum = 0; 
	ReadAddress = (u32)START_ID_ADDR + ReadAddress; 

	while(DataNum < ReadNum) 
	{
		// ��ָ��Ľ���������ȡ�ڴ�
		*(ReadBuf + DataNum) = *(__IO u8*) ReadAddress++; 
		DataNum++;
	}
	return DataNum; 
} 

// ��������ǵ��͵�STM32����Flash��д���ݵķ�װ�������кܶ�ο�����
void WriteFlashNByte(u32 WriteAddress,u32 *WriteData,u8 NUM) 
{
	u8 i;
	// FLASH_UnlockBank1��FLASH_ClearFlag��ΪдFlash��׼����
	FLASH_UnlockBank1(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 

	// ����Flash����ҳΪ��λ
	FLASHStatus = FLASH_ErasePage(START_ID_ADDR); 

	if(FLASHStatus == FLASH_COMPLETE) 
	{ 
		for(i=0;i<NUM;i++)
		// ��Flash��д
		// FLASH_ProgramWord������STM32�ٷ��ṩ�ĺ����ӿڣ���Linux�е��ں˿�����д������������Linux
		// �е�APIԭ����񣬶������Ƿ�װ�ṩ��һ�������ӿڣ����ǿ����ߵ�������ӿ���ʵ��������Ҫ
		// �Ĺ��ܣ�����ǳ���ֲ�ĺô��������׼��ĳ���Ա������������¡�
		FLASHStatus = FLASH_ProgramWord(START_ID_ADDR + WriteAddress+i*4, WriteData[i]); //flash.c ��API���� 
	} 
	FLASH_LockBank1(); 
} 

// ��flash�ж�ȡID��
void Get_Save_Flash_ID(void)
{
	unsigned char i,j;
	int ret;
	unsigned char temp[16];
	
	ret = ReadFlashNBtye(0,temp,16);
	if(ret == 16)
	{
		for(i=0;i<4;i++){
			for(j=0;j<4;j++){
				// ��temp�д���Ѿ���ȡ�����ݷŵ�pt_SP37Data��
				// ��С��
				pt_SP37Data[i].UniqueID[3-j] = temp[i*4+j];
			}
		}
	}
}

void Check_ID(void)
{
	unsigned char i,j,tx_num;

// ѭ��1�Σ���ʾһ����̥��ID�룬һ��ѭ��4�Σ��õ�4����̥��ID��
	for(i=0;i<4;i++)
	{
		// ���ն����пط���4����̥��ID��
		Tx_ID_To_Dvd[3] = i+1;			// �Ĵ�ѭ��
		// ÿ����̥��ID��4��bitλ���
		// pt_SP37Data[i].UniqueID[0]��ֵ�Ƿ���˷��͸����ն˵�ֵ
		Tx_ID_To_Dvd[4] = pt_SP37Data[i].UniqueID[0];
		Tx_ID_To_Dvd[5] = pt_SP37Data[i].UniqueID[1];
		Tx_ID_To_Dvd[6] = pt_SP37Data[i].UniqueID[2];			
		Tx_ID_To_Dvd[7] = pt_SP37Data[i].UniqueID[3];
		// ���õ����������ݽ���У�飬���һ��bitλ�õ�У����
		// Calculate_CRC�������Σ���һ����У����������ʼ��ַ���ڶ�������У��Ĵ�С
		Tx_ID_To_Dvd[8] = Calculate_CRC(Tx_ID_To_Dvd,8);

		// tx_num�ֲ�������ʾTx_ID_To_Dvd������һ����9������
		tx_num=9;
		j=0;
		// whileѭ���ǽ�Tx_ID_To_Dvd�����9����������ͨ�����ڷ��͵��п�̨������ͨ����������
		// ���ܿ������ն˷��س�����ָ����
		while(tx_num--)
		{
				USART_SendData(USART1,Tx_ID_To_Dvd[j++]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		}
	}
}

// ��������ӽ��ն˷��ص��пص�ָ�����Check_ID����ԭ����һ����
void Check_V()
{
	unsigned char j,tx_num;
 
	Tx_V_To_Dvd[5] = SP37_Cal_Voltage(pt_SP37Data[0].Voltage) *10;
	Tx_V_To_Dvd[6] = SP37_Cal_Voltage(pt_SP37Data[1].Voltage) *10;
	Tx_V_To_Dvd[7] = SP37_Cal_Voltage(pt_SP37Data[2].Voltage) *10;
	Tx_V_To_Dvd[8] = SP37_Cal_Voltage(pt_SP37Data[3].Voltage) *10;
	Tx_V_To_Dvd[9] = Calculate_CRC(Tx_V_To_Dvd,9);
	
	tx_num=10;
	j=0;
	while(tx_num--)
	{
			USART_SendData(USART1,Tx_V_To_Dvd[j++]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}

// ��������Ҫ��������������̥
void Tire_Change_Location(unsigned char Change_Dir1,unsigned char Change_Dir2)
{
	// Temp�ֲ���������һ����ת����
	unsigned char Temp[4] = {0};
	
	Temp[0] = pt_SP37Data[Change_Dir1-1].UniqueID[0];
	Temp[1] = pt_SP37Data[Change_Dir1-1].UniqueID[1];
	Temp[2] = pt_SP37Data[Change_Dir1-1].UniqueID[2];
	Temp[3] = pt_SP37Data[Change_Dir1-1].UniqueID[3];
	pt_SP37Data[Change_Dir1-1].UniqueID[0] = pt_SP37Data[Change_Dir2-1].UniqueID[0];
	pt_SP37Data[Change_Dir1-1].UniqueID[1] = pt_SP37Data[Change_Dir2-1].UniqueID[1];
	pt_SP37Data[Change_Dir1-1].UniqueID[2] = pt_SP37Data[Change_Dir2-1].UniqueID[2];
	pt_SP37Data[Change_Dir1-1].UniqueID[3] = pt_SP37Data[Change_Dir2-1].UniqueID[3];
	
	pt_SP37Data[Change_Dir2-1].UniqueID[0] = Temp[0];
	pt_SP37Data[Change_Dir2-1].UniqueID[1] = Temp[1];
	pt_SP37Data[Change_Dir2-1].UniqueID[2] = Temp[2];
	pt_SP37Data[Change_Dir2-1].UniqueID[3] = Temp[3];
	
	pt_SP37Data[Change_Dir1-1].Check_Trip_status	= 0;
	pt_SP37Data[Change_Dir1-1].Pressure[0]			= 0;
	pt_SP37Data[Change_Dir1-1].Pressure[1]			= 0;
	pt_SP37Data[Change_Dir1-1].Temperature[0]		= 0;
	pt_SP37Data[Change_Dir1-1].Temperature[1]		= 0;
	pt_SP37Data[Change_Dir1-1].Acceleration[0]		= 0;
	pt_SP37Data[Change_Dir1-1].Acceleration[1]		= 0;
	pt_SP37Data[Change_Dir1-1].Voltage[0]			= 0;
	pt_SP37Data[Change_Dir1-1].Voltage[1]			= 0;
	pt_SP37Data[Change_Dir1-1].CRC8					= 0;
                                                   ;
	pt_SP37Data[Change_Dir2-1].Check_Trip_status	= 0;
	pt_SP37Data[Change_Dir2-1].Pressure[0]			= 0;
	pt_SP37Data[Change_Dir2-1].Pressure[1]			= 0;
	pt_SP37Data[Change_Dir2-1].Temperature[0]		= 0;
	pt_SP37Data[Change_Dir2-1].Temperature[1]		= 0;
	pt_SP37Data[Change_Dir2-1].Acceleration[0]		= 0;
	pt_SP37Data[Change_Dir2-1].Acceleration[1]		= 0;
	pt_SP37Data[Change_Dir2-1].Voltage[0]			= 0;
	pt_SP37Data[Change_Dir2-1].Voltage[1]			= 0;
	pt_SP37Data[Change_Dir2-1].CRC8					= 0;

	Save_ID_Flash();
}

// ��̥ѹ�����ݷ��͵��п���
void Send_Tire_Data_To_Dvr(void)
{
	unsigned char i,tx_num,NUM;
	// NUM��ʾ�ǵڼ�����̥
	for(NUM=0;NUM<4;NUM++)
	{
		switch(NUM)
		{
			case 0:
				Tx_Data_To_Dvd[3] = 0x00;
				break;
			case 1:
				Tx_Data_To_Dvd[3] = 0x01;
				break;
			case 2:
				Tx_Data_To_Dvd[3] = 0x10;
				break;
			case 3:
				Tx_Data_To_Dvd[3] = 0x11;
				break;
			default:
				break;
		}

		// ������˷��͵����ն˵�ѹ�����¶��ϱ����пأ������������û�������Ŀ�����̥��ѹ�����¶�ֵ
		Tx_Data_To_Dvd[4] = (unsigned char)SP37_Cal_Pressure_V2(pt_SP37Data[NUM].Pressure)/3.44;
		Tx_Data_To_Dvd[5] = (unsigned char)SP37_Cal_Temperature(pt_SP37Data[NUM].Temperature)+50;

		// ���ն˽��յ�����˵ĵ�ѹֵ������Ҫ����ѹֵ���ݵ��пز��֣�ֻ��Ҫ����Ƿ�͵�ѹ����
		if((SP37_Cal_Voltage(pt_SP37Data[NUM].Voltage)*10) < 25)		//�͵�ѹ���
		{
			// 0x10����bit4Ϊ1��Э���й涨��bit4Ϊ1ʱ���ǵ�ص�ѹ��ѹ����
			Status_Tire[NUM] = Status_Tire[NUM] | 0x10;
		}
		else
		{
			// 0xef����bit4Ϊ0��Э���й涨��bit4Ϊ0ʱ���ǵ�ص�ѹ���������пط��͵�ѹ����
			Status_Tire[NUM] = Status_Tire[NUM] & 0xef;
		}

		// ©����⣬�͵�ѹ����ԭ����һ����
		if(pt_SP37Data[NUM].Check_Trip_status == 1)						//©��״̬
		{
			Status_Tire[NUM] = Status_Tire[NUM] | 0x08;
		}
		else
		{
			Status_Tire[NUM] = Status_Tire[NUM] & 0xf7;
		}

		// ��̥״̬���
		Tx_Data_To_Dvd[6] = Status_Tire[NUM]; 
		// CRCУ��
		Tx_Data_To_Dvd[7] = Calculate_CRC(Tx_Data_To_Dvd,7);

		tx_num=8;
		i=0;
		while(tx_num--)
		{
			// ͨ���������пط�������
				USART_SendData(USART1,Tx_Data_To_Dvd[i++]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		}
	}	
}


/*���������յ�������ͬ��ID �����뵱ǰ�ĸ���̥����ͬ��¼��������Ӧ����̥λ��*/
void Save_ID_Func(PT_SP37_RF_DATA Cache)
{
	unsigned char i,tx_num;
	static unsigned char Cont=0;	
	static unsigned char Temp[4];

// ��һ���ж���ǰ���ڶ����ж���ǰ���������ж���󣬵������ж��Һ�
// if���������������Ž���ѧϰģʽ
// ��Ϊ���ĸ�֮������Ĺ�ϵ��ֻҪ��һ���������ˣ���ôif�����жϾͲ������ˣ����Ծ���Ҫ����ѧϰģʽ
	if(((pt_SP37Data[0].UniqueID[0] !=  Cache->UniqueID[0]) || (pt_SP37Data[0].UniqueID[1] != Cache->UniqueID[1]) || (pt_SP37Data[0].UniqueID[2] !=  Cache->UniqueID[2]) || (pt_SP37Data[0].UniqueID[3] !=  Cache->UniqueID[3]))
	&&((pt_SP37Data[1].UniqueID[0] !=  Cache->UniqueID[0]) || (pt_SP37Data[1].UniqueID[1] != Cache->UniqueID[1]) || (pt_SP37Data[1].UniqueID[2] !=  Cache->UniqueID[2]) || (pt_SP37Data[1].UniqueID[3] !=  Cache->UniqueID[3]))
	&&((pt_SP37Data[2].UniqueID[0] !=  Cache->UniqueID[0]) || (pt_SP37Data[2].UniqueID[1] != Cache->UniqueID[1]) || (pt_SP37Data[2].UniqueID[2] !=  Cache->UniqueID[2]) || (pt_SP37Data[2].UniqueID[3] !=  Cache->UniqueID[3]))
	&&((pt_SP37Data[3].UniqueID[0] !=  Cache->UniqueID[0]) || (pt_SP37Data[3].UniqueID[1] != Cache->UniqueID[1]) || (pt_SP37Data[3].UniqueID[2] !=  Cache->UniqueID[2]) || (pt_SP37Data[3].UniqueID[3] !=  Cache->UniqueID[3])))
	{
		if((Temp[0] == Cache->UniqueID[0])&&(Temp[1] == Cache->UniqueID[1])&&(Temp[2] == Cache->UniqueID[2])&&(Temp[3] == Cache->UniqueID[3]))
		{
			Cont++;
		}
		else
		{
			Temp[0] = Cache->UniqueID[0];
			Temp[1] = Cache->UniqueID[1];
			Temp[2] = Cache->UniqueID[2];
			Temp[3] = Cache->UniqueID[3];
		}
		// ������Ϳ�ʼѧϰ��
		if(Cont == 3)
		{
			Cont = 0;
			switch (Study_Dir)
			{
				case Tire_LF:
					
					pt_SP37Data[0].UniqueID[0]= Cache->UniqueID[0];
					pt_SP37Data[0].UniqueID[1]= Cache->UniqueID[1];
					pt_SP37Data[0].UniqueID[2]= Cache->UniqueID[2];
					pt_SP37Data[0].UniqueID[3]= Cache->UniqueID[3];
					Save_ID_Flash();
					Ack_OK_To_Dvd[4] = 0x00;
					Ack_OK_To_Dvd[5] = Calculate_CRC(Ack_OK_To_Dvd,5);
					
					tx_num=6;
					i=0;
					while(tx_num--)
					{
							USART_SendData(USART1,Ack_OK_To_Dvd[i++]);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
					}
					break;
				case Tire_RF:
					
					pt_SP37Data[1].UniqueID[0]= Cache->UniqueID[0];
					pt_SP37Data[1].UniqueID[1]= Cache->UniqueID[1];
					pt_SP37Data[1].UniqueID[2]= Cache->UniqueID[2];
					pt_SP37Data[1].UniqueID[3]= Cache->UniqueID[3];
					Save_ID_Flash();
					Ack_OK_To_Dvd[4] = 0x01;
					Ack_OK_To_Dvd[5] = Calculate_CRC(Ack_OK_To_Dvd,5);

					tx_num=6;
					i=0;
					while(tx_num--)
					{
							USART_SendData(USART1,Ack_OK_To_Dvd[i++]);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
					}
					break;
				case Tire_LB:
					
					pt_SP37Data[2].UniqueID[0]= Cache->UniqueID[0];
					pt_SP37Data[2].UniqueID[1]= Cache->UniqueID[1];
					pt_SP37Data[2].UniqueID[2]= Cache->UniqueID[2];
					pt_SP37Data[2].UniqueID[3]= Cache->UniqueID[3];
					Save_ID_Flash();
					Ack_OK_To_Dvd[4] = 0x10;
					Ack_OK_To_Dvd[5] = Calculate_CRC(Ack_OK_To_Dvd,5);

					tx_num=6;
					i=0;
					while(tx_num--)
					{
							USART_SendData(USART1,Ack_OK_To_Dvd[i++]);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
					}
					break;
				case Tire_RB:

					pt_SP37Data[3].UniqueID[0]= Cache->UniqueID[0];
					pt_SP37Data[3].UniqueID[1]= Cache->UniqueID[1];
					pt_SP37Data[3].UniqueID[2]= Cache->UniqueID[2];
					pt_SP37Data[3].UniqueID[3]= Cache->UniqueID[3];
					Save_ID_Flash();
					Ack_OK_To_Dvd[4] = 0x11;
					Ack_OK_To_Dvd[5] = Calculate_CRC(Ack_OK_To_Dvd,5);
					
					tx_num=6;
					i=0;
					while(tx_num--)
					{
							USART_SendData(USART1,Ack_OK_To_Dvd[i++]);
							while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
					}
					break;
				default:					
					break;
			}				
		}
	}
}

// ��SRAM����flash��д����
void Save_ID_Flash(void)
{
	unsigned char i,j;
	u32 Write_add_buf[4]={0};
	
	Study_Dir = 0;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Write_add_buf[i] = pt_SP37Data[i].UniqueID[j] | (Write_add_buf[i]<<8);
		}	
	}		
	WriteFlashNByte(0,Write_add_buf,4);
}

/*	ͷ�ļ�·��
.\CORE;..\FWLib\inc;.\INCLUDE
*/


