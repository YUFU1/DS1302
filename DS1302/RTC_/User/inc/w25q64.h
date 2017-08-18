#ifndef W25Q64_H
#define W25Q64_H
#include "includes.h"

              // cs 拉低 PB14
#define  CS_L   GPIOB->ODR &=~(0x1<<14);
	           // cs 拉高
#define  CS_H    GPIOB->ODR |=(0x1<<14);


//页编程
void W25Q64_Page_Program(u32 addr,u8 date);
u8 W25Q64_Read_Date(u32 addr);

void W25Q64_Read_DateFromZK(u32 addr,u16 size,u8 *date);
//写使能
void W25Q64_WriteEnable(void);
void W25Q64_WriteDisable(void);

//擦除
void W25Q64_Sector_Erase(u32 addr);
void W25Q64_Block_Erase(u32 addr);
void W25Q64_Chip_Erase(void);
//读状态
void W25Q64_ReadStatus(void);



#endif

