#include <stdio.h>


unsigned int  getNumOfVal(int val);

int main(void)
{
	getNumOfVal(200);
	
	return 0;
}


unsigned int  getNumOfVal(int val)
{
	unsigned int count = 0;
	unsigned int i = 0;

	if(val<0)	
		val = -val;

	while (val / 10 > 0) {
		count++;			// count先加1，在循环
		val = val / 10;
	}
	
	i = count + 1;
	
	printf("i = %d.\n", i);

	return 0;
}















