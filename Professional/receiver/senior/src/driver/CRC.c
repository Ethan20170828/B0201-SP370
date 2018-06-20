#include<CRC.h>


#define POLYNOMIAL 0x2F
#define CRC8_INIT  0xAA

// 最大长度0xFF  BlockLength<=0xFF
// 这个校验是按照SP370发射端的校验方法进行解密，这样才能对接上
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


// 用来计算CRC，与我们的协议相对应，这个CRC用于校验串口那边的数据(串口助手/DVD按照这种方式校验)
unsigned char CRC_Calc(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char Check_Data;
	unsigned char i;

	// 把第0个取出来
	Check_Data = BlockStart[0];
	// 然后依次进行异或，得到CRC校验的值
	// 校验方法就是按照上位机的协议进行校验的
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

// 计算CRC校验的，与我们的上位机串口协议相对应的
unsigned char Calculate_CRC(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char Check_Data;
	unsigned char i;
	
	Check_Data = BlockStart[0];
	// for循环是将BlockStart数组的所有数据进行依次CRC校验，最后得到CRC校验值，也就是Check_Data
	for(i=1;i<BlockLength;i++)
	{
		Check_Data = Check_Data ^ BlockStart[i];
	}
	return Check_Data;
}



