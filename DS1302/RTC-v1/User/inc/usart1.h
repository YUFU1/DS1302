#ifndef USART1_H
#define USART1_H
#include "includes.h"
 //如果采用16倍的过采样，则定义该宏
 //如果采用8倍的过采样，则不定义该宏
#define Usart1_OVER8

void Usart1_Init(int boand);
void Usart1_Sendbyte(u8 date);
void Usart1_SendString(char date[]);
u8 Usart1_Receivebyte(void);
void Usart1_Receivestring(u8 *buf);
void Usart1_Exit_Init(void);

#endif
