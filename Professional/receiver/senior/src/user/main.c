/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                         ziotlab 出品                                    		    */
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
#include "stm32f10x_flash.h" //flash操作接口文件（在库文件中），必须要包含 

#define  Tire_LF    1
#define  Tire_RF    2
#define  Tire_LB    3
#define  Tire_RB    4
#define START_ID_ADDR	0x08000000+2048*30																											//flash 中保存数据
#define Save_ID_LF_ADDR 0
#define Save_ID_RF_ADDR 4
#define Save_ID_LB_ADDR 8
#define Save_ID_RB_ADDR 12


/**********************************协议中通讯收发缓冲区***********************************/
unsigned char Tx_Data_To_Dvd[8] = 	{0x55,0xaa,0x08,0x00,0x00,0x00,0x00,0x00}; 								
unsigned char Rx_CMD_Form_Dvd[6] = 	{0}; 
unsigned char Ack_To_Dvd[6]  = 		{0x55,0xaa,0x06,0x10,0x00,0x00}; 
unsigned char Ack_OK_To_Dvd[6] = 	{0x55,0xaa,0x06,0x18,0x00,0x00}; 
unsigned char Tx_ID_To_Dvd[9] = 	{0x55,0xaa,0x09,0x00,0x00,0x00,0x00,0x00,0x00}; 
unsigned char Tx_V_To_Dvd[10] = 	{0x55,0xaa,0x0A,0x20,0x01,0x00,0x00,0x00,0x00,0x00}; 
unsigned char Replace_Tire[7] = 	{0};
unsigned char Replace_Tire_Ack[7] = {0x55,0xaa,0x07,0x30,0x00,0x00,0x00};
/**********************************协议中通讯收发缓冲区***********************************/


unsigned char Study_Mode_Flag=0;	//等于1是进入学习模式，等于是进入到正常模式
unsigned char Study_Dir=0;			//进入学习模式之后，需要学习的那个轮胎
unsigned char Status_Tire[4]={0};	//每个轮胎状态记录
unsigned char Signal_Check[4]={0};	//监测每个轮胎是否有断开连接

T_SP37_RF_DATA pt_SP37Data[4];      //保存4个轮胎数据


/**********************************协议实现功能函数***********************************/
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
/**********************************协议实现功能函数***********************************/


int main(void)
{
	unsigned char i,j,ret;

	Clock_Init();
	NVIC_Configuration();		//中断配置

	// 刚开始上电的时候会延时一段时间(出现屏幕闪烁几次现象)，原因是在main函数中调用了delay_init函数
	delay_init();
	// 屏幕闪烁结束了 LED灯开始亮了
    LED_Init();
    
	USART1_Init(19200);			//USART1初始化,波特率19200	

// Tim3是定时器，定时4S向中控发送数据
	Tim3_Init(50000,64);        // 50ms的溢出中断 //系统灯闪烁
	TIM3_Cmd(ENABLE);
//	TIM3_Cmd(DISABLE);


    OLED_Init(); 
// OLED屏幕初始化完了，先屏幕上可以看到东西了
// 可以看到网站、版本号信息以及轮胎的ID号
    Display_Http();
	Display_Version();

	// 刚开始启动上电时，这四个轮胎的ID显示都是00000000
    Display_Unique_ID1(pt_SP37Data[0].UniqueID);
    Display_Unique_ID2(pt_SP37Data[1].UniqueID);
    Display_Unique_ID3(pt_SP37Data[2].UniqueID);
    Display_Unique_ID4(pt_SP37Data[3].UniqueID);

// 接收端的TDA5235进行初始化
	TDA5235_Init();
// 接收端的TDA5235的寄存器进行初始化
	TDA5235_SFR_Init();

	// 从STM32内部的flash中读取出原来存储到flash中的ID码
	// 发射端4个ID，经过学习后，接收端和4个发射端建立起来关联了，接收端程序会把4个发射端的16个字节的
	// 唯一ID存到flash中，学习的时候就是把轮胎的唯一ID存到接收端的STM32的flash中，接收到一包SP370的数
	// 据后，与存储到的ID相对应，看看是否一致。
	Get_Save_Flash_ID();

// while循环就是处理接收端和中控的沟通、接收端和发射端的沟通的代码
	while(1)
	{	
		// 如果接收到中控命令，则直接解析和响应代码；
		// 接收端和中控之间的沟通；
		// 串口中接收到一帧完整数据后，就是把ReceiveState全局变量值置为1，进入到if中；
		// 中控给接收端发送协议指令，如果ReceiveState=1，表示串口接收到协议指令；
		// 如果中控给发射端发送的协议指令不对，则接收端没有响应，不会进入相应的模式，
		// 中控会接收到接收端检测到的胎压的数据值。
		if(ReceiveState == 1)
		{
			// ReceiveState等于0，表示中控没有给接收端发送数据了，然后接收端对中控
			// 发过来的这一包数据(指令)进行解析，来判断我们要进行哪种模式(譬如学习模式、查询ID模式等)
			ReceiveState = 0;

			// 结合协议进行分析
			// 这个函数就是对串口接收到中控发过来的一帧数据进行解析、运行
			Data_Process();
		}
		// 这一段是通过SP370与TDA5235进行通信；
		// 如果TDA5235有接收到发射端数据,进行处理；
		// 接收端和发射端之间的沟通；
		// TDA5235_FIFO_NUM不等于，表示接收端接收到了发射端发送的数据，并且存放到接收端的缓冲区中
		if(TDA5235_FIFO_NUM){
			// 发射端发送到接收端的数据(TDA5235_FIFO)赋值给Cache_SP37Data
			PT_SP37_RF_DATA Cache_SP37Data = (PT_SP37_RF_DATA)TDA5235_FIFO;
#if 1
			// 接收端的正常工作模式，接收端从发射端SP370正常接收胎压数据，在显示屏上正常显示数据
			if(!Study_Mode_Flag)		// 正常工作模式
			{
				for(i=0;i<4;i++)
				{
					for(j=0;j<4;j++)
					{
						// pt_SP37Data[i].UniqueID[j]就是我们学习完后四个轮胎的ID
						// Cache_SP37Data->UniqueID[j]刚刚收到的那个轮胎的ID
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
			// 接收端在学习模式下
			else			// 学习模式
			{
				// 结合协议进行学习
				// 把学习模式中正在学习的SP370的唯一ID保存起来
				// 学习模式下，发射端给接收端传递的信息中只有轮胎的ID码
				Save_ID_Func(Cache_SP37Data);
			}
#endif

			LED_ReceiveReverse();		//STATUS灯闪烁
			TDA5235_FIFO_NUM = 0;	//清零，表示一包数据发送完事，等待发射端发送下一包数据
		}
	}
}

// 这时候数据已经通过接收端发送到串口的buffer中了
// 这个函数是对中控向接收端发送的协议进行解析处理
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
// 先判断协议中的同步是否相等。
	if ((USART1_RxBuffer[0] == 0x55) && (USART1_RxBuffer[1] == 0xaa))
	{
		// USART1_RxBuffer串口缓冲区作为起始，USART1_RX_NUM作为长度
		// 如果校验的十六进制数不对，CRC_Calc函数的返回值是FLASE
		// 那么在显示屏幕上显示1
		if (FLASE == CRC_Calc(USART1_RxBuffer, USART1_RX_NUM))
		{
            pt_SP37Data[0].UniqueID[0] = 0x1;
            Display_Unique_ID1(pt_SP37Data[0].UniqueID);
			return;
		}

		for (i=0; i<USART1_RX_NUM; i++)
		{
			// 中控向接收端发送6字节指令的情况
			if (USART1_RX_NUM == 6) 
			{
				// 中控向接收端发送指令，指令先存储到串口缓冲区中
				// 这一步就是将存储到串口缓冲区中的指令放到Rx_CMD_Form_Dvd全局部变量数组中
				Rx_CMD_Form_Dvd[i] = USART1_RxBuffer[i];
			}
			// 中控向接收端发送7字节指令的情况
			else if(USART1_RX_NUM == 7)
			{
				// 轮胎调换
				// 中控向接收端发送指令，指令先存储到串口缓冲区中
				// 这一步就是将存储到串口缓冲区中的指令放到Replace_Tire全局变量数组中
				Replace_Tire[i] = USART1_RxBuffer[i];
			}
		}

		if(USART1_RX_NUM == 6)
		{
			// 上位机发送给接收端的指令是 0x55 0xaa 0x06 0x07 0x00，表示接收端进入查询四个传感器的ID模式
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x07)&&(Rx_CMD_Form_Dvd[4] == 0x00))
			{
				// USART_SendData(USART1, 0x77);
				// 查询4个传感器的ID
				Check_ID();
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x06 0x02 0x01，表示接收端进入查询四个传感器的电压模式
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x02)&&(Rx_CMD_Form_Dvd[4] == 0x01))
			{
				// 查询电压
				Check_V();
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x06 0x06 0x00，表示接收端进入推出传感器的学习模式
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x06)&&(Rx_CMD_Form_Dvd[4] == 0x00))
			{
				// 推出传感器的学习模式
				Exit_Study_Mode();
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x06 0x01，表示接收端进入传感器的学习模式
			if((Rx_CMD_Form_Dvd[2] == 0x06) && (Rx_CMD_Form_Dvd[3] == 0x01))
			{
				temp = Rx_CMD_Form_Dvd[4];
				// 进入学习状态，传参temp表示我要学习哪个轮胎
				// Rx_CMD_Form_Dvd的第四位是00表示左前、01表示右前、10表示左后、11表示右后
				Enter_Study_Mode(temp);
			}				
		}
		// 等于，进入轮胎调换
		if(USART1_RX_NUM == 7)
		{
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x00 0x01，表示接收端进入左前和右前对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x01))
			{
				Tire_Change_Location(Tire_LF,Tire_RF);
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x00 0x10，表示接收端进入左前和左后对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x10))
			{
				Tire_Change_Location(Tire_LF,Tire_LB);
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x00 0x11，表示接收端进入左前和右后对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x00)&&(Replace_Tire[5] == 0x11))
			{
				Tire_Change_Location(Tire_LF,Tire_RB);
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x01 0x10，表示接收端进入右前和左后对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x01)&&(Replace_Tire[5] == 0x10))
			{
				Tire_Change_Location(Tire_RF,Tire_LB);
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x01 0x11，表示接收端进入右前和右后对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x01)&&(Replace_Tire[5] == 0x11))
			{				
				Tire_Change_Location(Tire_RF,Tire_RB);
			}
			// 上位机发送给接收端的指令是 0x55 0xaa 0x07 0x03 0x10 0x11，表示接收端进入左后和右后对调模式
			if((Replace_Tire[2] == 0x07) && (Replace_Tire[3] == 0x03)&&(Replace_Tire[4] == 0x10)&&(Replace_Tire[5] == 0x11))
			{				
				Tire_Change_Location(Tire_LB,Tire_RB);
			}
			// 轮胎给上位机发送一个回应，主要是指令的bit4和bit5
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
	// switch_Tire表示你传递过来的哪个轮胎，通过switch case来进行判断
	switch (switch_Tire)
	{
		case 0x00:
				Study_Dir = Tire_LF;	// 左前
			break;
		case 0x01:
				Study_Dir = Tire_RF;	// 右前
			break;
		case 0x10:
				Study_Dir = Tire_LB;	// 左后
			break;
		case 0x11:
				Study_Dir = Tire_RB;	// 右后
			break;
		default:
				Study_Dir = 0;
			break;
	}
	// Ack_To_Dvd表示接收端向中控回复数据，bit4表示回复的学习的那个轮胎
	Ack_To_Dvd[4] = switch_Tire;
	// bit5是CRC校验码
	Ack_To_Dvd[5] = Calculate_CRC(Ack_To_Dvd,5);

// 接收端通过串口向主控返回指令
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

// 中控给接收端发送提出学习指令后，接收端会给中控返回一包指令
	Ack_Exit_Study[0] = 0x55;
	Ack_Exit_Study[1] = 0xaa;
	Ack_Exit_Study[2] = 0x06;
	Ack_Exit_Study[3] = 0x16;
	Ack_Exit_Study[4] = 0x00;
	Ack_Exit_Study[5] = Calculate_CRC(Ack_Exit_Study,5);

// 下面是接收端的指令通过串口发送给中控，和Check_ID是一样的	
	tx_num=6;
	i=0;
	while(tx_num--)
	{
			USART_SendData(USART1,Ack_Exit_Study[i++]);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	}
}


volatile FLASH_Status FLASHStatus = FLASH_COMPLETE; //Flash操作状态变量 

// 这个函数是典型的STM32中往Flash写数据的封装
// 读取Flash中N个Byte
// ReadAddress是读取Flash中哪一个地址
// ReadBuf将地址读取到什么地方去
// ReadNum读取多少个字节
int ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum) 
{ 
	int DataNum = 0; 
	ReadAddress = (u32)START_ID_ADDR + ReadAddress; 

	while(DataNum < ReadNum) 
	{
		// 用指针的解引用来读取内存
		*(ReadBuf + DataNum) = *(__IO u8*) ReadAddress++; 
		DataNum++;
	}
	return DataNum; 
} 

// 这个函数是典型的STM32中往Flash中写数据的封装，网上有很多参考资料
void WriteFlashNByte(u32 WriteAddress,u32 *WriteData,u8 NUM) 
{
	u8 i;
	// FLASH_UnlockBank1和FLASH_ClearFlag是为写Flash做准备的
	FLASH_UnlockBank1(); 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 

	// 擦除Flash，以页为单位
	FLASHStatus = FLASH_ErasePage(START_ID_ADDR); 

	if(FLASHStatus == FLASH_COMPLETE) 
	{ 
		for(i=0;i<NUM;i++)
		// 往Flash中写
		// FLASH_ProgramWord函数是STM32官方提供的函数接口，与Linux中的内核开发者写的驱动函数、Linux
		// 中的API原理很像，都是他们封装提供了一个函数接口，我们开发者调用这个接口来实现我们想要
		// 的功能，这就是程序分层的好处，各个阶级的程序员做本身该做的事。
		FLASHStatus = FLASH_ProgramWord(START_ID_ADDR + WriteAddress+i*4, WriteData[i]); //flash.c 中API函数 
	} 
	FLASH_LockBank1(); 
} 

// 从flash中读取ID码
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
				// 将temp中存放已经读取的数据放到pt_SP37Data中
				// 大小端
				pt_SP37Data[i].UniqueID[3-j] = temp[i*4+j];
			}
		}
	}
}

void Check_ID(void)
{
	unsigned char i,j,tx_num;

// 循环1次，表示一个轮胎的ID码，一共循环4次，得到4个轮胎的ID码
	for(i=0;i<4;i++)
	{
		// 接收端向中控返回4个轮胎的ID码
		Tx_ID_To_Dvd[3] = i+1;			// 四次循环
		// 每个轮胎的ID由4个bit位组成
		// pt_SP37Data[i].UniqueID[0]的值是发射端发送给接收端的值
		Tx_ID_To_Dvd[4] = pt_SP37Data[i].UniqueID[0];
		Tx_ID_To_Dvd[5] = pt_SP37Data[i].UniqueID[1];
		Tx_ID_To_Dvd[6] = pt_SP37Data[i].UniqueID[2];			
		Tx_ID_To_Dvd[7] = pt_SP37Data[i].UniqueID[3];
		// 将得到的所有数据进行校验，最后一个bit位得到校验码
		// Calculate_CRC函数传参，第一个是校验的数组的起始地址，第二个参数校验的大小
		Tx_ID_To_Dvd[8] = Calculate_CRC(Tx_ID_To_Dvd,8);

		// tx_num局部变量表示Tx_ID_To_Dvd数组中一共有9个数据
		tx_num=9;
		j=0;
		// while循环是将Tx_ID_To_Dvd数组的9个数据依次通过串口发送到中控台，我们通过串口助手
		// 就能看到接收端返回出来的指令了
		while(tx_num--)
		{
				USART_SendData(USART1,Tx_ID_To_Dvd[j++]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		}
	}
}

// 这个函数从接收端返回到中控的指令方法和Check_ID函数原理是一样的
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

// 传参是你要更换的哪两个轮胎
void Tire_Change_Location(unsigned char Change_Dir1,unsigned char Change_Dir2)
{
	// Temp局部变量属于一个中转变量
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

// 把胎压的数据发送到中控中
void Send_Tire_Data_To_Dvr(void)
{
	unsigned char i,tx_num,NUM;
	// NUM表示是第几个轮胎
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

		// 将发射端发送到接收端的压力和温度上报到中控，这样可以让用户很清楚的看到轮胎的压力和温度值
		Tx_Data_To_Dvd[4] = (unsigned char)SP37_Cal_Pressure_V2(pt_SP37Data[NUM].Pressure)/3.44;
		Tx_Data_To_Dvd[5] = (unsigned char)SP37_Cal_Temperature(pt_SP37Data[NUM].Temperature)+50;

		// 接收端接收到发射端的电压值，不需要将电压值传递到中控部分，只需要检测是否低电压即可
		if((SP37_Cal_Voltage(pt_SP37Data[NUM].Voltage)*10) < 25)		//低电压检测
		{
			// 0x10就是bit4为1，协议中规定了bit4为1时，是电池电压低压警报
			Status_Tire[NUM] = Status_Tire[NUM] | 0x10;
		}
		else
		{
			// 0xef就是bit4为0，协议中规定了bit4为0时，是电池电压正常，向中控发送电压正常
			Status_Tire[NUM] = Status_Tire[NUM] & 0xef;
		}

		// 漏气检测，和电压检测的原理是一样的
		if(pt_SP37Data[NUM].Check_Trip_status == 1)						//漏气状态
		{
			Status_Tire[NUM] = Status_Tire[NUM] | 0x08;
		}
		else
		{
			Status_Tire[NUM] = Status_Tire[NUM] & 0xf7;
		}

		// 轮胎状态检测
		Tx_Data_To_Dvd[6] = Status_Tire[NUM]; 
		// CRC校验
		Tx_Data_To_Dvd[7] = Calculate_CRC(Tx_Data_To_Dvd,7);

		tx_num=8;
		i=0;
		while(tx_num--)
		{
			// 通过串口向中控发送数据
				USART_SendData(USART1,Tx_Data_To_Dvd[i++]);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		}
	}	
}


/*对码连续收到三次相同的ID 并且与当前四个轮胎不相同记录下来到相应的轮胎位置*/
void Save_ID_Func(PT_SP37_RF_DATA Cache)
{
	unsigned char i,tx_num;
	static unsigned char Cont=0;	
	static unsigned char Temp[4];

// 第一行判断左前，第二行判断右前，第三行判断左后，第四行判断右后
// if的条件不成立，才进入学习模式
// 因为这四个之间是与的关系，只要有一个不成立了，那么if条件判断就不成立了，所以就需要进入学习模式
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
		// 这里面就开始学习了
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

// 从SRAM中往flash中写数据
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

/*	头文件路径
.\CORE;..\FWLib\inc;.\INCLUDE
*/


