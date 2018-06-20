#include "Reg_SP37.h"
#include "SP37_ROMLibrary.h"
#include "SP37.h"
#include <absacc.h>		//绝对地址访问包含头文件

//成功返回1，
//失败返回0,
//8000H = -2048.0 kPa (Only theoretical)
//0000H = 0.0 kPa
//7FFFH = 2047.9375 kPa
//( = 2048 kPa - 1 LSB where 1 LSB = 1/16 kPa)
unsigned char Measure_Pressure(int *Pressure_VAL)
{
	//Input Parameter:2 Samples
	unsigned int SensorConfig = 0x0481;

	//Input Parameter: SampleRate
	unsigned char SampleRate = 0x00;

	unsigned char StatusByte;
	struct {
		signed int Compensated_Pressure;
		signed int Raw_Pressure;
		signed int Raw_Temperature;
	}idata Press_Result;

	StatusByte = Meas_Pressure(SensorConfig,SampleRate,&Press_Result.Compensated_Pressure);

	if(!StatusByte)
	{
		*Pressure_VAL = Press_Result.Compensated_Pressure;
		return 1;
	}
	else
	{
		// Pressure measurement was not successful, underflow or
		// overflow of ADC result, Sensor Fault Wire Bond Check,
		// or VMIN warning occurred
		return 0;
	}
}

//成功返回1;
//否则0;
//8000H = -2048.0 g
//0000H = 0.0 g
//7FFFH = 2047.9375 g
//( = 2048 g - 1 LSB where 1 LSB = 1/16 g)
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

//成功返回1;
//否则0;
//8000H = -256.0 °C
//0000H = 0.0 °C
//7FFFH = 255.9921875 °C
//( = 256 °C - 1 LSB where 1 LSB = 1/128 °C)
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

//成功返回1;
//否则0;
//8000H = -4096.0 mV (Only theoretical number)
//0000H = 0.0 mV
//7FFFH = 4095.875 mV
//( = 4096 mV - 1 LSB where 1 LSB = 1/8 mV)
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


//StatusByte:
//0: XTAL started and delay passed
//-1: XTAL already on (no action)
char StartXtalOscillator(void)
{
	// Return value of start xtal oscillator is stored in StatusByte
	unsigned char StatusByte;
	// Input parameters for start xtal oscillator
	unsigned char Delay = 33;
	// Start xtal oscillator function call
	StatusByte = StartXtalOsc(Delay);
	return StatusByte;
}

// Defines
#define bitmask_TMOD_CLK 0x08
#define bitmask_RFS_RFSE 0x02
//StatusByte:
//0: XTAL stopped
//-1: XTAL already off (no action)
//-2: XTAL not stopped because an ongoing RF transmission)
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


//重新设置SP37 随机数发生器的种子
//使用LSB of the Sensor ID作为其种子,可以使得每一个设备的延时不一致
//返回种子值
unsigned char SP37_InitRandom(void)
{
	ID_Struct ID_Result;
	unsigned char ID_LSB;

	Read_ID(&ID_Result);

	ID_LSB = (unsigned char)ID_Result.ID;

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
	RFTX = (0<<7) | (1<<6) | (0<<5) | (1<<2) | (0<<0);

	//XTAL1 = 0x7F;				// +/- 35kHz Deviation
	//XTAL0 = 0x17;	;	

	XTAL1 = 0xFF;				// +/- 35kHz Deviation
	XTAL0 = 0x0;
}

//波特率9600Bit/s
#define BAUDRATE 9600
//返回值0:成功
//返回其他值 失败
//-1: StartXtalOscillator->error
//-2: VCO_Tuning->error
//-3: Send_RF_Telegram->error
//-4: StopXtalOscillator->error
char Send_RF_Data(unsigned char idata * descriptorPtr)
{
	signed char StatusByte;
	
	StatusByte = StartXtalOscillator();
	
	//signed char VCO_Tuning(void)
	StatusByte = VCO_Tuning();
	if(StatusByte)
		return -2;

	StatusByte = Send_RF_Telegram(BAUDRATE,descriptorPtr);
	if(StatusByte)
		return -3;

	StatusByte = StopXtalOscillator();
	if(StatusByte == -2)
		return -4;

	return 0;
}

