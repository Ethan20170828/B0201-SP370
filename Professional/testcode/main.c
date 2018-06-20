#include <stdio.h>


int main(void)
{
	unsigned int i = 0;
	unsigned char Check_Data;
	unsigned char val[10];
	
	val[0] = 0x55;
	val[1] = 0xaa;
	val[2] = 0x06;
	val[3] = 0x01;
	val[4] = 0x00;
//	val[5] = 0x55;

	Check_Data = val[0];
	for(i=1; i<5; i++)
	{
		Check_Data = Check_Data ^ val[i];
	}
	
	printf("result: 0x[%x]\n", Check_Data);
	
	return 0;
}































