#include "vsprintf.h"
#include "string.h"
#include "printf.h"

extern void putc(unsigned char c);
extern unsigned char getc(void);

#define	OUTBUFSIZE	1024
#define	INBUFSIZE	1024


// 自己定义了2个全局变量数组，分别作为发送/接收缓冲区
// 将来发送时先将要发送的信息格式化送入发送缓冲区，然后putc函数直接从发送缓冲区取数据发送出去。
static char g_pcOutBuf[OUTBUFSIZE];
static char g_pcInBuf[INBUFSIZE];

// printf函数中最关键的就是vsprintf函数(这个函数是别人写好的，我们不用去管)
// 和putc函数(需要我们自己去写)；
// printf("a = %d.\n", a);	"a = %d.\n"是printf的第一个参数，也是printf输出格式样本，
// %开头的东西以后将会被替代，a是printf的第二个参数；printf中的每个参数是靠逗号分隔的；
int printf(const char *fmt, ...)	// ...是Linux中支持的可变参数
{
	int i;
	int len;
	va_list args;		// 参数列表
	// va_start和va_end是Linux中的可变参数
	va_start(args, fmt);					// va_start负责printf的变参
	len = vsprintf(g_pcOutBuf, fmt, args);	// vsprintf格式化打印信息，最终得到纯字符串格式的打印信息
	va_end(args);
	for (i = 0; i < strlen(g_pcOutBuf); i++)
	{	// putc函数才是真正和输出设备绑定，这个函数需要我们自己去实现，这就是移植的关键。
		putc(g_pcOutBuf[i]);				// 调用putc函数，把字符串发送出去
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

