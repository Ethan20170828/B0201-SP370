/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                          ziotlab出品                                         */
/*   ----------------------------------------------------------------------------------------------   */






#include "Reg_SP37.h"
#include "SP37_ROMLibrary.h"
#include <uart.h>
#include <SP37.h>
#include <RF.h>

#if 0

#define  TRIP_NORMAL	0
#define  TRIP_GAS_LEAK	1
#define  PUMP_UP		2

unsigned char Check_Trip_Gas_Leak(int newPressure);		//检测轮胎气压变化
unsigned char Absolute_Value(int a,int b);				//监测，放气或者充气

#endif

void MAIN(void)
{
	unsigned char StatusByte;
	T_SP37_DATA idata t_sp37;

	//串口初始化
	Uart_Init();
	//RF433初始化
	Send_RF_Init();

	if(WUF == 0x00) {																							//每次上电 WUF == 0;
	//随机数初始化
		StatusByte = SP37_InitRandom();
		printf("SP37_InitRandom =%02X\r\n",(int)StatusByte);
		// 上电时将GPR0设置为0
        GPR0 =0;                           // 特殊寄存器，唤醒后数据不丢失
        
	}

	Read_SP37_ID(&t_sp37.IDH,&t_sp37.IDL);
	Read_ID(&t_sp37.ID_Result);

	// 计数器相关的初始化，把定时器的时间定时为5秒
	// 和典型的51单片机不太一样
	// 找到相应寄存器的地址，然后在SP370数据手册中查找，就知道相应的寄存器的含义了
	ITPR = 100; 		// 2k分频 / 100 = 50ms  
	ITPH = 0;			// ITPL = 100;
	ITPL = 100;			// 计数次数   100*50ms =5s

	while(1)
	{
		WDRES = 1;
// 采集SP370的四个数据
		Measure_Pressure(&t_sp37.Pressure);
		Measure_Temperature(&t_sp37.Temperature);
		Measure_Acceleration(&t_sp37.Acceleration);
		Measure_Supply_Voltage(&t_sp37.Voltage);		

		
#if 0
// 这种方式是想给谁学习，那么就给谁重启，检测到气压的变化后，就知道让我学习了，马上发送学习包
		switch(Check_Trip_Gas_Leak(t_sp37.Pressure))
		{
			case TRIP_GAS_LEAK:
				t_sp37.Check_Trip_status= 1;
				break;
			case PUMP_UP:
				t_sp37.Check_Trip_status = 2;
				break;
			default:
				t_sp37.Check_Trip_status = 0;
				printf("test-------------------------------------\r\n");
				break;
		}
		printf("t_sp37.Pressure =%d\r\n",t_sp37.Pressure);

		printf("t_sp37.Check_Trip_status =%d\r\n",(int)t_sp37.Check_Trip_status);
#endif

// 这里的if条件编译就是发送端添加的学习功能代码
// GPR0是普通寄存器，
// TPMS装在轮胎中第一次要学习来标定
		if(GPR0 < 4)
		{
			// 发送刚开始上电的学习数据包
			StatusByte = Send_RF1(t_sp37);	//开机首次连续发射3次，每次10包，用于学习
            GPR0 ++;
			//printf("Send_RF Succeed\r\n");
		}    
		else
		{
			// 发送SP370检测到的正常数据包
			StatusByte = Send_RF(t_sp37);			//正常发射
            GPR0 = 4;
        }
		if(!StatusByte)
			;					//printf("Send_RF Succeed\r\n");
		else
			printf("Send_RF ERROR\r\n");

		Powerdown();		//进入休眠,等待ITP时间到唤醒(Powerdown期间功耗最低)

	}
}

#if 0
unsigned char Check_Trip_Gas_Leak(int newPressure)
{
	static unsigned char Trip_Flag_leak=0;
	static unsigned char Trip_Flag_pump=0;
	static int Save_sp370 = 0;
	unsigned char ret;
	
	if(Save_sp370 != newPressure)
	{
		switch(Absolute_Value(Save_sp370,newPressure))
		{
			case TRIP_GAS_LEAK:
				Trip_Flag_leak++;
				if(Trip_Flag_leak == 3)			//5s  2.5kp 持续三次认为漏气
				{	
					Trip_Flag_leak = 2;
					ret =  TRIP_GAS_LEAK;
				}
				Trip_Flag_pump =0;
				break;
			case PUMP_UP:
				Trip_Flag_pump++;
				if(Trip_Flag_pump == 3)
				{	
					Trip_Flag_pump = 2;
					ret =  PUMP_UP;
				}
				Trip_Flag_leak =0;
				break;
			default:				
				Trip_Flag_pump=0;
				Trip_Flag_leak=0;
				ret =  TRIP_NORMAL;
				break;
		}
		Save_sp370=newPressure;
	}
	return ret;
}

unsigned char Absolute_Value(int a,int b)
{
	if((a>b) &&((a-b)/64 > 10))  //5s  2.kp
	{		
		return TRIP_GAS_LEAK;
	}
	else if((a<b) &&((b-a)/16 > 1)) //5s  1kp
	{
		return PUMP_UP;
	}
	else
		return TRIP_NORMAL;
}
#endif
