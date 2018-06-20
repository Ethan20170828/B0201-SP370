#include "sys.h"
#include "mysprintf.h"

//����ƽ��
s32 pow(s32 val,u8 pow)		// ����val��pow�η�
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


//�����Ǽ�λ��
static u8 getNumOfVal(s32 val)
{
	u8 count = 0;

	if(val<0)	
		val = -val;

	while (val / 10 > 0) {
		count++;		// count�ȼ�1����ѭ��
		val = val / 10;
	}

	return count+1;		// �������val=200����whileѭ���ĳ���ʱcountֵΪ2������������Ҫ��1��������val��ֵΪ��λ��
}


//����ת�ַ���(���Ȳ���ǰ�油�ո�)����Ҫ�ǽ��������ֽ�������ת�ַ���
//�����ַ�������
static u16 DecConver2StrAddF(char* str,u8 length,u32 val)	// ���ε�length�Ǳ�ʾ�������ֵĳ���(������������λ��������λ��)
{
	u8 i;
	u8 valLength = getNumOfVal(val);	// �����м�λ��
	char *pStart = str;

	//���㲹0
	if(valLength < length){
		for(i=0;i<length - valLength;i++){
			*str = ' ';
			str++;
		}
	}

	
	for(i=0;i<valLength;i++){
		u32 tempVal = pow(10,valLength-1-i);	// �õ�10��valLength-1-i�η�����һ��ֵ
		*str = val/tempVal + '0';
		val %= tempVal;
		str++;
	}

	*str = '\0';

	return str - pStart;
	
}


//����ת�ַ���(���Ȳ�����油0)
//�����ַ�������
static u16 DecConver2StrAddB(char* str,u8 length,u32 val)	// ���ε�length�Ǳ�ʾС�����ֵĳ���(������������λ��)
{
	u8 i;
	u8 valLength = getNumOfVal(val);
	char *pStart = str;					// �Ѵ�������str�ַ������׵�ַ��ֵ��pStart��Ȼ��str�ĵ�ַ�����ӣ�ֱ����������'\0'��ֹͣ
	
	for(i=0;i<valLength;i++){
		u32 tempVal = pow(10,valLength-1-i);	// �õ�10��valLength-1-i�η�����һ��ֵ
		*str = val/tempVal + '0';
		val %= tempVal;
		str++;
	}

	//������油0
	if(valLength < length){
		for(i=0;i<length - valLength;i++){
			*str = '0';
			str++;
		}
	}

	*str = '\0';

	return str - pStart;		// �����ַ����ĳ��ȣ�pStart��str��ֵ���Ĵ�������ָ����ַ������׵�ַ��str�Ǵ��������ַ�����ͨ��str�����ַ���ƶ���ֱ������'\0'��˵�����μ����ַ������Ƚ�����ͨ��strĿǰ�ĵ�ַ��ȥ����ֵ��pStartʱ�ĵ�ַ������������������ַ����ĳ���
	
}

//��Char����ת��Ϊ�ַ���(ʮ������)
//�����ַ�������
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


//���޷�������ת��Ϊ�ַ���
//�����ַ�������
u16 MySprintf_ud(char* str,u32 val,u8 length)
{		
	return DecConver2StrAddF(str,length,(u32)val);
}

//������ת��Ϊ�ַ���
//�����ַ�������
u16 MySprintf_d(char* str,s32 val,u8 length)
{	
	//��������
	if(val < 0)	{val = -val;	*str = '-';		str++;}
	else		{*str = ' ';		str++;}
	
	return DecConver2StrAddF(str,length,(u32)val) + 1;
}

//��������ת��Ϊ�ַ���
//�����ַ�������
u16 MySprintf_f(char* str,float val,u8 lenInt,u8 lenDec)
{	
	u8 StrIntLength,StrDecLength;

	u32 IntVal,decVal;

	//��������
	if(val < 0)	{val = -val;	*str = '-';		str++;}
	else		{*str = ' ';		str++;}

	IntVal = (u32)val;	// �Դ�������ֵ���������ֽ���ȡ��
	decVal = (u32)((val - IntVal)*pow(10,lenDec));			// �Դ�������ֵ��С����������ȡ��
	// ����lenInt��ʾ�������ֵĳ���
	StrIntLength =  DecConver2StrAddF(str,lenInt,IntVal);	// ����С����ǰ�����������

	//���С����
	str += StrIntLength;	// ��˼��: str��ָ��ǰ��ַ��StrIntLength����ռ�õĵ�ַ���ȣ�str += StrIntLength;����StrIntLength֮��str��ָ��ĵ�ַλ��
	*str = '.';				// ��str��ǰ��ָ��ĵ�ַ�и�ֵ�ַ�.
	str++;					// strָ��ĵ�ַ�����һ��
	// ����lenDec��ʾС�����ֵĳ���
	StrDecLength =  DecConver2StrAddB(str,lenDec,decVal);	// ����С��������С������

	return StrIntLength + StrDecLength + 2;
}




