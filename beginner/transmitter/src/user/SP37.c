#include "Reg_SP37.h"
#include "SP37_ROMLibrary.h"
#include "SP37.h"
#include <absacc.h>		//绝对地址访问包含头文件


// 返回值unsigned char表示成功(0)或失败(1)
// 返回值是通过输出型参数传出来的
// 注意传进来的参数Pressure_VAL类型必须是Press_Result结构体类型的变量
// 测到的是一个16位的二进制数，然后再乘以0.0625(2048/32768(0x8000))，等于实际的压力值
unsigned char Measure_Pressure(int *Pressure_VAL)	// 输出型参数int *Pressure_VAL，将测到的压力值传出去
{
	//Input Parameter:2 Samples
	unsigned int SensorConfig = 0x0481;

	//Input Parameter: SampleRate
	unsigned char SampleRate = 0x00;

	unsigned char StatusByte;

// *(PressResult+2)指的是Raw_Temperature变量
	struct {	// 结构体中的变量都是Press_Result结构体的偏移量(指的是指针类型的偏移量；+0/+1/+2/)
		signed int Compensated_Pressure;	// 经过补偿后的压力
		signed int Raw_Pressure;			// 未经过补偿的压力
		signed int Raw_Temperature;			// 原来的温度
	}idata Press_Result;		// idata表示将这个变量定义到idata域中(内部的sram中)

	// Meas_Pressure函数在SP37_ROMLibrary.h头文件中声明了，而且是加extern的外部声明，
	// 其函数体应用查找官方提供的标准库(SP37_Rom_Lib_Guide_V1.0)函数来得到。
	StatusByte = Meas_Pressure(SensorConfig,SampleRate,&Press_Result.Compensated_Pressure);
	// 根据Meas_Pressure函数的返回值来判断是成功了还是失败了
	if(!StatusByte)
	{
		// 将检测到的压力以指针解引用的方式传出去
		*Pressure_VAL = Press_Result.Compensated_Pressure;
		return 1;		// 表示成功
	}
	else
	{
		// Pressure measurement was not successful, underflow or
		// overflow of ADC result, Sensor Fault Wire Bond Check,
		// or VMIN warning occurred
		return 0;		// 表示失败
	}
}


unsigned char Measure_Acceleration(int *Acceleration_VAL)
{
	//new raw temperature | Average value | 8 Samples
	unsigned int SensorConfig = (0<<8) | (0<<3) | (3<<0);
	//00H: No delay
	unsigned char SampRate = 0x00;
	unsigned char StatusByte;

	struct {
		signed int Compensated_Acceleration;
		signed int Raw_Acceleration_Data;
		signed int Raw_Temperature;
	} idata AccelResult;

	StatusByte = Meas_Acceleration(SensorConfig,SampRate,&AccelResult.Compensated_Acceleration);
	if(!StatusByte)
	{
		*Acceleration_VAL = AccelResult.Compensated_Acceleration;
		return 1;
	}
	else
	{
		// Acceleration measurement was not successful, underflow or
		// overflow of ADC result, Sensor Fault Wire Bond Check,
		// Sensor Fault Diagnosis Resistor, or VMIN warning occurred
		return 0;
	}		
}


unsigned char Measure_Temperature(int *Temperature_VAL)
{
	unsigned char StatusByte;
	struct{
		signed int Compensated_Temperature;
		signed int Raw_Temperature;
	}idata Temp_Result;

	StatusByte = Meas_Temperature(&Temp_Result.Compensated_Temperature);
	if(!StatusByte)
	{
		*Temperature_VAL = Temp_Result.Compensated_Temperature;
		return 1;
	}
	else
	{
		return 0;
	}
}


unsigned char Measure_Supply_Voltage(int *Voltage_VAL)
{	
	unsigned char StatusByte;

	struct{
		signed int Voltage;
		signed int Raw_voltage;
	}idata Volt_Result;

	StatusByte = Meas_Supply_Voltage(&Volt_Result.Voltage);
	if(!StatusByte)
	{
		*Voltage_VAL = Volt_Result.Voltage;
		return 1;
	}
	else
	{
		return 0;
	}
}

// 开启发射端的外部晶振，发射端开始发射射频信号
// 这样在发射完一包数据和下一包数据之间可以关闭晶振，起到省电的作用
char StartXtalOscillator(void)
{
	// Return value of start xtal oscillator is stored in StatusByte
	unsigned char StatusByte;
	// Input parameters for start xtal oscillator
	unsigned char Delay = 33;		// 等待稳定的delay
	// Start xtal oscillator function call
	StatusByte = StartXtalOsc(Delay);
	return StatusByte;
}

// Defines
#define bitmask_TMOD_CLK 0x08
#define bitmask_RFS_RFSE 0x02

// 关闭发射端的外部晶振，发射端关闭发射射频信号
char StopXtalOscillator(void)
{
	// Return value of stop xtal oscillator is stored in StatusByte
	signed char StatusByte;
	while (!(RFS & bitmask_RFS_RFSE)); // Wait for RF Transmission to end
	RFC &= ~0x03; // Disable PA & PLL
	TMOD &= ~bitmask_TMOD_CLK; // Clear TMOD_CLK
	StatusByte = StopXtalOsc(); // Stop XTAL oscillator
	return StatusByte;
}

/*
#define CRC8_INIT 0x00
//初始值0x00
//polynomial (x8+x2+x+1) => 0x07
unsigned char CRC8_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength)
{
	return CRC8_Calc(CRC8_INIT,BlockStart,BlockLength);
}
*/

#define CRC_Baicheva_INIT 0xAA		//German Association of the Automotive Industry (VDA)
//初始值0xAA
//polynomial (x8+x5+x3+x2+x+1) => 0x2F
//BlockLength <= 119bits/8 = 14		小于119bit最佳
unsigned char CRC_Baicheva_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength)
{
	return CRC_Baicheva_Calc(CRC_Baicheva_INIT,BlockStart,BlockLength);
}

//自定义ID存储于绝对地址CBYTE[0x57E0]和[0x57E1]
//此处为用户ID,可以由烧录器指定并烧录
void Read_SP37_ID(unsigned char idata *IDH,unsigned char idata *IDL)
{
	*IDH = CBYTE[0x57E0];
	*IDL = CBYTE[0x57E1];
}

//该接口用于返回轮胎位置识别码，这是一种轮胎识别实现方法
// 一般是用LF设备对轮胎位置进行标定，然后写入SP370的FLASH中某一固定位置中
E_Tyre_Position Read_Tyre_Position(void)
{
	return (E_Tyre_Position)CBYTE[0x57E2];		// CBYTE就是对内存地址进行解析
}


//重新设置SP37 随机数发生器的种子
//使用LSB of the Sensor ID作为其种子,可以使得每一个设备的延时不一致
//返回种子值
unsigned char SP37_InitRandom(void)
{
	ID_Struct ID_Result;
	unsigned char ID_LSB;

// 读取这个传感器的唯一ID码，Read_ID函数的传参是输出型参数
	Read_ID(&ID_Result);

// ID_LSB就是我们读出来的唯一ID编码的最低字节
	ID_LSB = (unsigned char)ID_Result.ID;	// ID读出来的是4字节的，我们这里只保留最后一个，然后赋值给ID_LSB

	//随机数种子不能为0
	if( ID_LSB != 0){
		RNGD = ID_LSB;
	}

	return RNGD;
}

void Send_RF_Init(void)
{
	//配置RF
	//Data not inverted | ASK modulation | 433MHz | 3 PA output stages
	//RFTX = (1<<7) | (1<<5) | (1<<2) | (3<<0);
	// FSK switch open, FSK with internal pulling capacitors, data inv., 433.92 MHz, 1PA stages
	// RF-Transmitter Configuration Register简称RFTX
	RFTX = (0<<7) | (1<<6) | (0<<5) | (1<<2) | (0<<0);	// 在数据手册SP370_Datasheet_V1.0的RFTX部分(61页)

	//XTAL1 = 0x7F;				// +/- 35kHz Deviation
	//XTAL0 = 0x17;	;	
	// 这段代码和数据手册SP370_Datasheet_V1.0的3.8和3.9部分有关
	XTAL1 = 0xFF;		// +/- 35kHz Deviation
	XTAL0 = 0x0;		// 设置这两个值，目的是将433.92MHz的两端(高频、低频)的频率扩展到最大，
						// 这个能够很好的进行区分高频和低频
}

//波特率9600Bit/s
#define BAUDRATE 9600
//返回值0:成功
//返回其他值 失败
//-1: StartXtalOscillator->error
//-2: VCO_Tuning->error
//-3: Send_RF_Telegram->error
//-4: StopXtalOscillator->error
// 发送一包RF数据包的流程是: 
// 打开外部晶振(StartXtalOscillator)->Send_RF_Telegram(发送数据)->StopXtalOscillator
// 选择这种方式发射数据的目的是低功耗
// Send_RF_Data函数的执行步骤在2.37.1 Description里面有要求，
char Send_RF_Data(unsigned char idata * descriptorPtr)
{
	signed char StatusByte;
	
	StatusByte = StartXtalOscillator();		// 晶振打开，开始发送数据

	//signed char VCO_Tuning(void)
	StatusByte = VCO_Tuning();				// 等PLL稳定
	if(StatusByte)
		return -2;		// 表示启动失败了，只有返回值是0的时候表示启动成功

// 传递参数BAUDRATE影响Manchester编码里面的同步信号，同样接收端也是按照这个BAUDRATE进行数据接收以及解码
	StatusByte = Send_RF_Telegram(BAUDRATE,descriptorPtr);
	if(StatusByte)
		return -3;

	StatusByte = StopXtalOscillator();		// 晶振关闭，结束数据传递
	if(StatusByte == -2)
		return -4;

	return 0;
}

