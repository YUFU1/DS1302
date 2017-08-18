#ifndef USART2_H
#define USART2_H
	#include "includes.h"
	/**
	 如果定义了Usart2_OVER8则说明采用的是16倍的过采样
	否则是8倍的过采样
	**/

#define Usart2_OVER8

void Usart2_Init(int boand);
void Usart2_Sendbyte(u8 date);
void Usart2_SendString(u8 *buf);
u8 Usart2_Receivebyte(void);
void Usart2_Exit_Init(void);




#endif


