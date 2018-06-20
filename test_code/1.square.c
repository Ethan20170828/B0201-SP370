#include <stdio.h>


int pow(int val, int pow);

int main(void)
{
	pow(3, 4);
	
	return 0;
}


int pow(int val, int pow)
{
	int i;
	int tempVal = val;

	//0
	if(!pow)	
		return 1;

	for(i=0;i<pow-1;i++)
	{
		tempVal *= val;
	}
	
	printf("tempVal = %d.\n", tempVal);

	return 0;
}









