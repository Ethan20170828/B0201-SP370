#include <stdio.h>
#include <stdarg.h>


int my_snprintf(char *s, int size, const char *fmt, ...) //该自定义函数，与系统提供的snprintf()函数相同。
{
    va_list ap;
    int n=0;
    va_start(ap, fmt); //获得可变参数列表
    n=vsnprintf (s, size, fmt, ap); //写入字符串s
    va_end(ap); //释放资源
    return n; //返回写入的字符个数
}

int main(void) 
{
    char str[1024];
    my_snprintf(str, sizeof(str), "%d,%d,%d,%d", 5, 6, 7, 8);
    printf("%s\n",str);
    return 0;
}