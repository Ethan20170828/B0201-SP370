#include <stdio.h>



int pow(int val, int pow);
unsigned int getNumOfVal(int val);
unsigned int DecConver2StrAddF(char* str,unsigned int length,unsigned int val);

int main(void)
{
	char strArr[32]={0};
	char *str = strArr;
	
	DecConver2StrAddF(str, 3, 4);
	
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
	
	return tempVal;
}

//计算是几位数
unsigned int  getNumOfVal(int val)
{
	unsigned int count = 0;
	//unsigned int i = 0;

	if(val<0)	
		val = -val;

	while (val / 10 > 0) {
		count++;			// count先加1，在循环
		val = val / 10;
	}
	
	//i = count + 1;
	
	//printf("i = %d.\n", i);

	return count + 1;
}


//数字转字符串(长度不足前面补空格)
//返回字符串长度
unsigned int DecConver2StrAddF(char* str,unsigned int length,unsigned int val)
{
	unsigned int i;
	//unsigned int j;
	unsigned int valLength = getNumOfVal(val);
	char *pStart = str;

	//不足补0
	if(valLength < length){
		for(i=0;i<length - valLength;i++){
			*str = ' ';
			str++;
		}
	}

	for(i=0;i<valLength;i++){
		unsigned int tempVal = pow(10,valLength-1-i);
		*str = val/tempVal + '0';
		val %= tempVal;
		str++;
	}

	*str = '\0';
	
	//j = str - pStart;
	
	printf("str = %s.\n", str);
	printf("pStart = %s.\n", pStart);

	return 0;
}

















