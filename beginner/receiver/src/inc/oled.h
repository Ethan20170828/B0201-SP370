#ifndef _OLED_H
#define _OLED_H

#define SLAVEADDR	0x78

#define OLED_MAX_X	(128 - 1)
//#define OLED_MAX_Y	(8-1)

void OLED_Init(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_Clear(void);

void OLED_ShowChr(u8 x,u8 y,char chr);
void OLED_ShowStr(u8 x,u8 y,char *str);
//void OLED_ShowChinese(u8 x,u8 page,u8 num);
void OLED_ShowBMP(u8 x,u8 page,u8 num);
void OLED_CleanBMP(u8 x,u8 page);


#endif

