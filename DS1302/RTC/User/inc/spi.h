#ifndef SPI_H
#define SPI_H
#include "includes.h"

void SPI_Init(void);
void SPI_SendDate(u8 date);
u8 SPI_ReceiveDate(void);
#endif


