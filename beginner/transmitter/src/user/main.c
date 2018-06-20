/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                          ziotlab出品                                      					*/
/*   ----------------------------------------------------------------------------------------------   */


#include "Reg_SP37.h"
#include "SP37_ROMLibrary.h"
#include <uart.h>
#include <SP37.h>
#include <RF.h>

void MAIN(void)
{
	unsigned char StatusByte;
	// 定义这个结构体，可以将零散变量整理到一起，
	T_SP37_DATA idata t_sp37;

// Wakeup Flag Register简称WUF
// 如果不是冷启动，那么这个函数中的内容被直接跳过
	if(WUF == 0x00){			// 冷启动上电时,进行配置
		//串口初始化
		Uart_Init();
		//RF433(射频)初始化，初始化的是相应射频的寄存器
		Send_RF_Init();
		//随机数初始化，目的是让每个轮胎都生成一个伪随机数
		StatusByte = SP37_InitRandom();
	}

	Read_SP37_ID(&t_sp37.IDH,&t_sp37.IDL);			// 读取用户的自定义ID
	Read_ID(&t_sp37.ID_Result);						// 读取SP370内部4字节的唯一ID
	t_sp37.Tyre_Position = Read_Tyre_Position();	// 读取轮胎的位置信息；在我们这套体系中不用读取轮胎的位置信息，因此可删除这部分

	while(1)
	{
		WDRES = 1;
		// 下面四个是进行数据(压力、温度、加速度、电压)的获取，将读取到的数据放到t_sp37结构体中
		Measure_Pressure(&t_sp37.Pressure);
		Measure_Temperature(&t_sp37.Temperature);
		Measure_Acceleration(&t_sp37.Acceleration);
		Measure_Supply_Voltage(&t_sp37.Voltage);
		
		StatusByte = Send_RF(t_sp37);		// 数据读取出来后，调用Send_RF函数将数据传递出去

		if(!StatusByte)
			printf("Send_RF Succeed\r\n");
		else
			printf("Send_RF ERROR\r\n");

		Wait100usMultiples(100);	// 官方提供的库里的函数；等待10ms，循环一次
	}
}

