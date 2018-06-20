#include<CRC.h>

// ���涨�������ֵ�����շ��ͷ��ͷ�����һ��
#define POLYNOMIAL 0x2F		// ����ʽ
#define CRC8_INIT  0xAA		// ��ֵ

//��󳤶�0xFF  BlockLength<=0xFF��ÿ��CRCУ��Ĵ��붼��һ���ģ�Ψһ��һ���ľ��ǳ�ֵ�Ͷ���ʽ
// CRCУ���������ͨ����������õ�һ�ֲ��У����
// ����˺ͽ��ն˵�CRCУ�������һ�������ܽ�������ͨ��
// CRC���㷨�ǹ����ģ����Ƕ���ʽ�ͳ�ֵ��ֵ�����������Ŀ��׼Э�����涨��
unsigned char CRC8_Calc(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char i,j;
	unsigned char CRC_VAL = CRC8_INIT;	// ��ֵ
	
	for (j=0; j<BlockLength; j++){		// ��ָ��������е�ÿ���ֽڽ���У��
		CRC_VAL = *(BlockStart + j) ^ CRC_VAL;
		for (i=0; i<8; i++){			// ͬʱ��Ҫ����8��
			if (CRC_VAL & 0x80){		// �ж����λ�Ƿ�Ϊ1
				CRC_VAL <<= 1;			// ���λΪ1��������һλ��Ȼ��������������CRC_VAL = CRC_VAL << 1
				CRC_VAL ^= POLYNOMIAL;	// ����ʽ
			}
			else{
				CRC_VAL <<= 1;			// ���λΪ0��������һλ������Ҫ�������������CRC_VAL = CRC_VAL << 1
			}
		}
	}
	return CRC_VAL;						// ѭ������������CRCУ��ֵ
}


