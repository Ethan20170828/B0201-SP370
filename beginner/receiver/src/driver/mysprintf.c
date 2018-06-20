#include "sys.h"
#include "mysprintf.h"

//计算平方
s32 pow(s32 val,u8 pow)		// 计算val的pow次方
{
	u8 i;
	s32 tempVal=val;

	
	//0
	if(!pow)	
		return 1;

	for(i=0;i<pow-1;i++){
		tempVal *= val;
	}

	return tempVal;
}


//计算是几位数
static u8 getNumOfVal(s32 val)
{
	u8 count = 0;

	if(val<0)	
		val = -val;

	while (val / 10 > 0) {
		count++;		// count先加1，在循环
		val = val / 10;
	}

	return count+1;		// 如果参数val=200，则while循环的出来时count值为2，所以这里需要加1，来满足val初值为三位数
}


//数字转字符串(长度不足前面补空格)，主要是将整数部分进行数字转字符串
//返回字符串长度
static u16 DecConver2StrAddF(char* str,u8 length,u32 val)	// 传参的length是表示整数部分的长度(我们这里是两位数或者三位数)
{
	u8 i;
	u8 valLength = getNumOfVal(val);	// 计算有几位数
	char *pStart = str;

	//不足补0
	if(valLength < length){
		for(i=0;i<length - valLength;i++){
			*str = ' ';
			str++;
		}
	}

	
	for(i=0;i<valLength;i++){
		u32 tempVal = pow(10,valLength-1-i);	// 得到10的valLength-1-i次方，是一个值
		*str = val/tempVal + '0';
		val %= tempVal;
		str++;
	}

	*str = '\0';

	return str - pStart;
	
}


//数字转字符串(长度不足后面补0)
//返回字符串长度
static u16 DecConver2StrAddB(char* str,u8 length,u32 val)	// 传参的length是表示小数部分的长度(我们这里是四位数)
{
	u8 i;
	u8 valLength = getNumOfVal(val);
	char *pStart = str;					// 把传进来的str字符串的首地址赋值给pStart，然后str的地址会增加，直到增加遇到'\0'，停止
	
	for(i=0;i<valLength;i++){
		u32 tempVal = pow(10,valLength-1-i);	// 得到10的valLength-1-i次方，是一个值
		*str = val/tempVal + '0';
		val %= tempVal;
		str++;
	}

	//不足后面补0
	if(valLength < length){
		for(i=0;i<length - valLength;i++){
			*str = '0';
			str++;
		}
	}

	*str = '\0';

	return str - pStart;		// 返回字符串的长度，pStart是str赋值给的传进来的指向的字符串的首地址，str是传进来的字符串，通过str本身地址的移动，直到遇到'\0'，说明本次计算字符串长度结束，通过str目前的地址减去它赋值给pStart时的地址，来计算出传进来的字符串的长度
	
}

//将Char类型转化为字符串(十六进制)
//返回字符串长度
u16 MySprintf_x(char* str,u8 data)
{	
	*str = data >> 4;
	if(*str<10){
		*str += '0';
	}else if(*str <= 0x0F){
		*str = *str - 10 + 'A';
	}else{
		*str = ' ';
	}
	str++;
	
	
	*str = data & 0x0F;
	if(*str<10){
		*str += '0';
	}else if(*str <= 0x0F){
		*str = *str - 10 + 'A';
	}else{
		*str = ' ';
	}
	str++;
	
	return 2;
}


//将无符号整数转化为字符串
//返回字符串长度
u16 MySprintf_ud(char* str,u32 val,u8 length)
{		
	return DecConver2StrAddF(str,length,(u32)val);
}

//将整数转化为字符串
//返回字符串长度
u16 MySprintf_d(char* str,s32 val,u8 length)
{	
	//负数处理
	if(val < 0)	{val = -val;	*str = '-';		str++;}
	else		{*str = ' ';		str++;}
	
	return DecConver2StrAddF(str,length,(u32)val) + 1;
}

//将浮点数转化为字符串
//返回字符串长度
u16 MySprintf_f(char* str,float val,u8 lenInt,u8 lenDec)
{	
	u8 StrIntLength,StrDecLength;

	u32 IntVal,decVal;

	//负数处理
	if(val < 0)	{val = -val;	*str = '-';		str++;}
	else		{*str = ' ';		str++;}

	IntVal = (u32)val;	// 对传过来的值的整数部分进行取整
	decVal = (u32)((val - IntVal)*pow(10,lenDec));			// 对传进来的值的小数点后面的数取整
	// 参数lenInt表示整数部分的长度
	StrIntLength =  DecConver2StrAddF(str,lenInt,IntVal);	// 计算小数点前面的整数部分

	//添加小数点
	str += StrIntLength;	// 意思是: str是指向当前地址，StrIntLength是所占用的地址长度，str += StrIntLength;加上StrIntLength之后str所指向的地址位置
	*str = '.';				// 对str当前所指向的地址中赋值字符.
	str++;					// str指向的地址向后移一个
	// 参数lenDec表示小数部分的长度
	StrDecLength =  DecConver2StrAddB(str,lenDec,decVal);	// 计算小数点后面的小数部分

	return StrIntLength + StrDecLength + 2;
}




