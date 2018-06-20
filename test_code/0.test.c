#include <stdio.h>


int main(void)
{
	char strArr[32]={0};		// 定义字符数组
	char *str = strArr;
	
	*str = 'P';	str++;
	*str = ':';	str++;
	
	str += 1;
	
	*str = 'k';	str++;
	*str = 'P';	str++;
	*str = 'a';	str++;
	
	printf("strArr = %s\n", strArr);
	//printf("a = %s.\n", a);
	
	return 0;
}




