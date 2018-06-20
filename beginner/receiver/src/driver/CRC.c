#include<CRC.h>

// 下面定义的两个值，接收方和发送方必须一样
#define POLYNOMIAL 0x2F		// 多项式
#define CRC8_INIT  0xAA		// 初值

//最大长度0xFF  BlockLength<=0xFF；每个CRC校验的代码都是一样的，唯一不一样的就是初值和多项式
// CRC校验就是数据通信领域中最常用的一种差错校验码
// 发射端和接收端的CRC校验码必须一样，才能进行数据通信
// CRC的算法是公开的，但是多项式和初值的值是我们这个项目标准协议所规定的
unsigned char CRC8_Calc(unsigned char *BlockStart,unsigned char BlockLength)
{
	unsigned char i,j;
	unsigned char CRC_VAL = CRC8_INIT;	// 初值
	
	for (j=0; j<BlockLength; j++){		// 对指向的数据中的每个字节进行校验
		CRC_VAL = *(BlockStart + j) ^ CRC_VAL;
		for (i=0; i<8; i++){			// 同时需要计算8次
			if (CRC_VAL & 0x80){		// 判断最高位是否为1
				CRC_VAL <<= 1;			// 最高位为1，向左移一位，然后进行异或；类似于CRC_VAL = CRC_VAL << 1
				CRC_VAL ^= POLYNOMIAL;	// 多项式
			}
			else{
				CRC_VAL <<= 1;			// 最高位为0，向左移一位，不需要进行异或；类似于CRC_VAL = CRC_VAL << 1
			}
		}
	}
	return CRC_VAL;						// 循环结束，返回CRC校验值
}


