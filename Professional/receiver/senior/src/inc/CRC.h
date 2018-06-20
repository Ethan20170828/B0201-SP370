#ifndef _CRC_H
#define _CRC_H

#define	TURE	1
#define	FLASE	0
unsigned char CRC8_Calc(unsigned char *BlockStart,unsigned char BlockLength);
unsigned char CRC_Calc(unsigned char *BlockStart,unsigned char BlockLength);
unsigned char Calculate_CRC(unsigned char *BlockStart,unsigned char BlockLength);

#endif

