/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/* ************************************************************************************************** */
/*   ----------------------------------------------------------------------------------------------   */
/*                                          ziotlab��Ʒ                                         */
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

unsigned char Check_Trip_Gas_Leak(int newPressure);		//�����̥��ѹ�仯
unsigned char Absolute_Value(int a,int b);				//��⣬�������߳���

#endif

void MAIN(void)
{
	unsigned char StatusByte;
	T_SP37_DATA idata t_sp37;

	//���ڳ�ʼ��
	Uart_Init();
	//RF433��ʼ��
	Send_RF_Init();

	if(WUF == 0x00) {																							//ÿ���ϵ� WUF == 0;
	//�������ʼ��
		StatusByte = SP37_InitRandom();
		printf("SP37_InitRandom =%02X\r\n",(int)StatusByte);
		// �ϵ�ʱ��GPR0����Ϊ0
        GPR0 =0;                           // ����Ĵ��������Ѻ����ݲ���ʧ
        
	}

	Read_SP37_ID(&t_sp37.IDH,&t_sp37.IDL);
	Read_ID(&t_sp37.ID_Result);

	// ��������صĳ�ʼ�����Ѷ�ʱ����ʱ�䶨ʱΪ5��
	// �͵��͵�51��Ƭ����̫һ��
	// �ҵ���Ӧ�Ĵ����ĵ�ַ��Ȼ����SP370�����ֲ��в��ң���֪����Ӧ�ļĴ����ĺ�����
	ITPR = 100; 		// 2k��Ƶ / 100 = 50ms  
	ITPH = 0;			// ITPL = 100;
	ITPL = 100;			// ��������   100*50ms =5s

	while(1)
	{
		WDRES = 1;
// �ɼ�SP370���ĸ�����
		Measure_Pressure(&t_sp37.Pressure);
		Measure_Temperature(&t_sp37.Temperature);
		Measure_Acceleration(&t_sp37.Acceleration);
		Measure_Supply_Voltage(&t_sp37.Voltage);		

		
#if 0
// ���ַ�ʽ�����˭ѧϰ����ô�͸�˭��������⵽��ѹ�ı仯�󣬾�֪������ѧϰ�ˣ����Ϸ���ѧϰ��
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

// �����if����������Ƿ��Ͷ���ӵ�ѧϰ���ܴ���
// GPR0����ͨ�Ĵ�����
// TPMSװ����̥�е�һ��Ҫѧϰ���궨
		if(GPR0 < 4)
		{
			// ���͸տ�ʼ�ϵ��ѧϰ���ݰ�
			StatusByte = Send_RF1(t_sp37);	//�����״���������3�Σ�ÿ��10��������ѧϰ
            GPR0 ++;
			//printf("Send_RF Succeed\r\n");
		}    
		else
		{
			// ����SP370��⵽���������ݰ�
			StatusByte = Send_RF(t_sp37);			//��������
            GPR0 = 4;
        }
		if(!StatusByte)
			;					//printf("Send_RF Succeed\r\n");
		else
			printf("Send_RF ERROR\r\n");

		Powerdown();		//��������,�ȴ�ITPʱ�䵽����(Powerdown�ڼ书�����)

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
				if(Trip_Flag_leak == 3)			//5s  2.5kp ����������Ϊ©��
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
