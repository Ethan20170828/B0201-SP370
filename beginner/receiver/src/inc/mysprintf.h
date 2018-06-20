#ifndef _MYSPRINTF_H
#define _MYSPRINTF_H


u16 MySprintf_x(char* str,u8 data);
u16 MySprintf_ud(char* str,u32 val,u8 length);
u16 MySprintf_d(char* str,s32 val,u8 length);
u16 MySprintf_f(char* str,float val,u8 lenInt,u8 lenDec);


#endif


