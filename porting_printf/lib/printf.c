#include "vsprintf.h"
#include "string.h"
#include "printf.h"

extern void putc(unsigned char c);
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024


// �Լ�������2��ȫ�ֱ������飬�ֱ���Ϊ����/���ջ�����
// ��������ʱ�Ƚ�Ҫ���͵���Ϣ��ʽ�����뷢�ͻ�������Ȼ��putc����ֱ�Ӵӷ��ͻ�����ȡ���ݷ��ͳ�ȥ��
static char g_pcOutBuf[OUTBUFSIZE];
static char g_pcInBuf[INBUFSIZE];

// printf��������ؼ��ľ���vsprintf����(��������Ǳ���д�õģ����ǲ���ȥ��)
// ��putc����(��Ҫ�����Լ�ȥд)��
// printf("a = %d.\n", a);	"a = %d.\n"��printf�ĵ�һ��������Ҳ��printf�����ʽ������
// %��ͷ�Ķ����Ժ󽫻ᱻ�����a��printf�ĵڶ���������printf�е�ÿ�������ǿ����ŷָ��ģ�
int printf(const char *fmt, ...)	// ...��Linux��֧�ֵĿɱ����
{
	int i;
	int len;
	va_list args;		// �����б�
	// va_start��va_end��Linux�еĿɱ����
	va_start(args, fmt);					// va_start����printf�ı��
	len = vsprintf(g_pcOutBuf, fmt, args);	// vsprintf��ʽ����ӡ��Ϣ�����յõ����ַ�����ʽ�Ĵ�ӡ��Ϣ
	va_end(args);
	for (i = 0; i < strlen(g_pcOutBuf); i++)
	{	// putc������������������豸�󶨣����������Ҫ�����Լ�ȥʵ�֣��������ֲ�Ĺؼ���
		putc(g_pcOutBuf[i]);				// ����putc���������ַ������ͳ�ȥ
	}
	return len;
}


int scanf(const char * fmt, ...)
{
	int i = 0;
	unsigned char c;
	va_list args;
	
	while(1)
	{
		c = getc();
		putc(c);
		if((c == 0x0d) || (c == 0x0a))
		{
			g_pcInBuf[i] = '\0';
			break;
		}
		else
		{
			g_pcInBuf[i++] = c;
		}
	}
	
	va_start(args,fmt);
	i = vsscanf(g_pcInBuf,fmt,args);
	va_end(args);

	return i;
}

