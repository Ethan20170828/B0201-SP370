#include "Reg_SP37.h"
#include "SP37_ROMLibrary.h"
#include "SP37.h"
#include <absacc.h>		//���Ե�ַ���ʰ���ͷ�ļ�


// ����ֵunsigned char��ʾ�ɹ�(0)��ʧ��(1)
// ����ֵ��ͨ������Ͳ�����������
// ע�⴫�����Ĳ���Pressure_VAL���ͱ�����Press_Result�ṹ�����͵ı���
// �⵽����һ��16λ�Ķ���������Ȼ���ٳ���0.0625(2048/32768(0x8000))������ʵ�ʵ�ѹ��ֵ
unsigned char Measure_Pressure(int *Pressure_VAL)	// ����Ͳ���int *Pressure_VAL�����⵽��ѹ��ֵ����ȥ
{
	//Input Parameter:2 Samples
	unsigned int SensorConfig = 0x0481;

	//Input Parameter: SampleRate
	unsigned char SampleRate = 0x00;

	unsigned char StatusByte;

// *(PressResult+2)ָ����Raw_Temperature����
	struct {	// �ṹ���еı�������Press_Result�ṹ���ƫ����(ָ����ָ�����͵�ƫ������+0/+1/+2/)
		signed int Compensated_Pressure;	// �����������ѹ��
		signed int Raw_Pressure;			// δ����������ѹ��
		signed int Raw_Temperature;			// ԭ�����¶�
	}idata Press_Result;		// idata��ʾ������������嵽idata����(�ڲ���sram��)

	// Meas_Pressure������SP37_ROMLibrary.hͷ�ļ��������ˣ������Ǽ�extern���ⲿ������
	// �亯����Ӧ�ò��ҹٷ��ṩ�ı�׼��(SP37_Rom_Lib_Guide_V1.0)�������õ���
	StatusByte = Meas_Pressure(SensorConfig,SampleRate,&Press_Result.Compensated_Pressure);
	// ����Meas_Pressure�����ķ���ֵ���ж��ǳɹ��˻���ʧ����
	if(!StatusByte)
	{
		// ����⵽��ѹ����ָ������õķ�ʽ����ȥ
		*Pressure_VAL = Press_Result.Compensated_Pressure;
		return 1;		// ��ʾ�ɹ�
	}
	else
	{
		// Pressure measurement was not successful, underflow or
		// overflow of ADC result, Sensor Fault Wire Bond Check,
		// or VMIN warning occurred
		return 0;		// ��ʾʧ��
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

// ��������˵��ⲿ���񣬷���˿�ʼ������Ƶ�ź�
// �����ڷ�����һ�����ݺ���һ������֮����Թرվ�����ʡ�������
char StartXtalOscillator(void)
{
	// Return value of start xtal oscillator is stored in StatusByte
	unsigned char StatusByte;
	// Input parameters for start xtal oscillator
	unsigned char Delay = 33;		// �ȴ��ȶ���delay
	// Start xtal oscillator function call
	StatusByte = StartXtalOsc(Delay);
	return StatusByte;
}

// Defines
#define bitmask_TMOD_CLK 0x08
#define bitmask_RFS_RFSE 0x02

// �رշ���˵��ⲿ���񣬷���˹رշ�����Ƶ�ź�
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
//��ʼֵ0x00
//polynomial (x8+x2+x+1) => 0x07
unsigned char CRC8_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength)
{
	return CRC8_Calc(CRC8_INIT,BlockStart,BlockLength);
}
*/

#define CRC_Baicheva_INIT 0xAA		//German Association of the Automotive Industry (VDA)
//��ʼֵ0xAA
//polynomial (x8+x5+x3+x2+x+1) => 0x2F
//BlockLength <= 119bits/8 = 14		С��119bit���
unsigned char CRC_Baicheva_Calculate(unsigned char idata *BlockStart, unsigned char BlockLength)
{
	return CRC_Baicheva_Calc(CRC_Baicheva_INIT,BlockStart,BlockLength);
}

//�Զ���ID�洢�ھ��Ե�ַCBYTE[0x57E0]��[0x57E1]
//�˴�Ϊ�û�ID,��������¼��ָ������¼
void Read_SP37_ID(unsigned char idata *IDH,unsigned char idata *IDL)
{
	*IDH = CBYTE[0x57E0];
	*IDL = CBYTE[0x57E1];
}

//�ýӿ����ڷ�����̥λ��ʶ���룬����һ����̥ʶ��ʵ�ַ���
// һ������LF�豸����̥λ�ý��б궨��Ȼ��д��SP370��FLASH��ĳһ�̶�λ����
E_Tyre_Position Read_Tyre_Position(void)
{
	return (E_Tyre_Position)CBYTE[0x57E2];		// CBYTE���Ƕ��ڴ��ַ���н���
}


//��������SP37 �����������������
//ʹ��LSB of the Sensor ID��Ϊ������,����ʹ��ÿһ���豸����ʱ��һ��
//��������ֵ
unsigned char SP37_InitRandom(void)
{
	ID_Struct ID_Result;
	unsigned char ID_LSB;

// ��ȡ�����������ΨһID�룬Read_ID�����Ĵ���������Ͳ���
	Read_ID(&ID_Result);

// ID_LSB�������Ƕ�������ΨһID���������ֽ�
	ID_LSB = (unsigned char)ID_Result.ID;	// ID����������4�ֽڵģ���������ֻ�������һ����Ȼ��ֵ��ID_LSB

	//��������Ӳ���Ϊ0
	if( ID_LSB != 0){
		RNGD = ID_LSB;
	}

	return RNGD;
}

void Send_RF_Init(void)
{
	//����RF
	//Data not inverted | ASK modulation | 433MHz | 3 PA output stages
	//RFTX = (1<<7) | (1<<5) | (1<<2) | (3<<0);
	// FSK switch open, FSK with internal pulling capacitors, data inv., 433.92 MHz, 1PA stages
	// RF-Transmitter Configuration Register���RFTX
	RFTX = (0<<7) | (1<<6) | (0<<5) | (1<<2) | (0<<0);	// �������ֲ�SP370_Datasheet_V1.0��RFTX����(61ҳ)

	//XTAL1 = 0x7F;				// +/- 35kHz Deviation
	//XTAL0 = 0x17;	;	
	// ��δ���������ֲ�SP370_Datasheet_V1.0��3.8��3.9�����й�
	XTAL1 = 0xFF;		// +/- 35kHz Deviation
	XTAL0 = 0x0;		// ����������ֵ��Ŀ���ǽ�433.92MHz������(��Ƶ����Ƶ)��Ƶ����չ�����
						// ����ܹ��ܺõĽ������ָ�Ƶ�͵�Ƶ
}

//������9600Bit/s
#define BAUDRATE 9600
//����ֵ0:�ɹ�
//��������ֵ ʧ��
//-1: StartXtalOscillator->error
//-2: VCO_Tuning->error
//-3: Send_RF_Telegram->error
//-4: StopXtalOscillator->error
// ����һ��RF���ݰ���������: 
// ���ⲿ����(StartXtalOscillator)->Send_RF_Telegram(��������)->StopXtalOscillator
// ѡ�����ַ�ʽ�������ݵ�Ŀ���ǵ͹���
// Send_RF_Data������ִ�в�����2.37.1 Description������Ҫ��
char Send_RF_Data(unsigned char idata * descriptorPtr)
{
	signed char StatusByte;
	
	StatusByte = StartXtalOscillator();		// ����򿪣���ʼ��������

	//signed char VCO_Tuning(void)
	StatusByte = VCO_Tuning();				// ��PLL�ȶ�
	if(StatusByte)
		return -2;		// ��ʾ����ʧ���ˣ�ֻ�з���ֵ��0��ʱ���ʾ�����ɹ�

// ���ݲ���BAUDRATEӰ��Manchester���������ͬ���źţ�ͬ�����ն�Ҳ�ǰ������BAUDRATE�������ݽ����Լ�����
	StatusByte = Send_RF_Telegram(BAUDRATE,descriptorPtr);
	if(StatusByte)
		return -3;

	StatusByte = StopXtalOscillator();		// ����رգ��������ݴ���
	if(StatusByte == -2)
		return -4;

	return 0;
}

