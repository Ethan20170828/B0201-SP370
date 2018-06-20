#include<CRC.h>


#define POLYNOMIAL 0x2F
#define CRC8_INIT  0xAA

// ��󳤶�0xFF  BlockLength<=0xFF
// ���У���ǰ���SP370����˵�У�鷽�����н��ܣ��������ܶԽ���
unsigned char CRC8_Calc(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char i,j;
	unsigned char CRC_VAL = CRC8_INIT;	
	
	for (j=0; j<BlockLength; j++){
		CRC_VAL = *(BlockStart + j) ^ CRC_VAL;
		for (i=0; i<8; i++){
			if (CRC_VAL & 0x80){
				CRC_VAL <<= 1;
				CRC_VAL ^= POLYNOMIAL;
			}
			else{
				CRC_VAL <<= 1;
			}
		}
	}
	return CRC_VAL;
}


// ��������CRC�������ǵ�Э�����Ӧ�����CRC����У�鴮���Ǳߵ�����(��������/DVD�������ַ�ʽУ��)
unsigned char CRC_Calc(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char Check_Data;
	unsigned char i;

	// �ѵ�0��ȡ����
	Check_Data = BlockStart[0];
	// Ȼ�����ν�����򣬵õ�CRCУ���ֵ
	// У�鷽�����ǰ�����λ����Э�����У���
	for(i=1;i<BlockLength-1;i++)
	{
		Check_Data = Check_Data ^ BlockStart[i];
	}
	if(Check_Data == BlockStart[BlockLength-1])
	{
		return TURE;
	}
	else
		return FLASE;
}

// ����CRCУ��ģ������ǵ���λ������Э�����Ӧ��
unsigned char Calculate_CRC(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char Check_Data;
	unsigned char i;
	
	Check_Data = BlockStart[0];
	// forѭ���ǽ�BlockStart������������ݽ�������CRCУ�飬���õ�CRCУ��ֵ��Ҳ����Check_Data
	for(i=1;i<BlockLength;i++)
	{
		Check_Data = Check_Data ^ BlockStart[i];
	}
	return Check_Data;
}



