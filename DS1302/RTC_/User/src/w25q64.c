#include "includes.h"

/***************************************

函数名：W25Q64_Page_Program
函数参数：
函数返回值：无
函数功能：实现W25Q64页编程
函数描述：存储数据
****************************************/
void W25Q64_Page_Program(u32 addr,u8 date)
{
	//写使能
  W25Q64_WriteEnable();
	CS_L;
	//mcu发送芯片也编程指令
	SPI_SendDate(0x02);
	//发送24位地址
	SPI_SendDate(addr>>16);
	SPI_SendDate(addr>>8);
	SPI_SendDate(addr);
	SPI_SendDate(date);
	CS_H;
	W25Q64_ReadStatus();
}
/***************************************

函数名：W25Q64_Read_Date
函数参数：
函数返回值：无
函数功能：实现W25Q64页编程
函数描述：读数据
****************************************/
u8 W25Q64_Read_Date(u32 addr)
{
	u8 date;
	CS_L;
	//发送读指令
	SPI_SendDate(0x03);
	SPI_SendDate(addr>>16);
	SPI_SendDate(addr>>8);
	SPI_SendDate(addr);
	date=SPI_ReceiveDate();
	CS_H;
	return date;
}
/***************************************

函数名：W25Q64_Read_DateFromZK
函数参数：
函数返回值：无
函数功能：实现W25Q64页编程
函数描述：读数据
****************************************/
void W25Q64_Read_DateFromZK(u32 addr,u16 size,u8 *date)
{
	u16 i;
	CS_L;
	//发送读指令
	SPI_SendDate(0x03);
	SPI_SendDate(addr>>16);
	SPI_SendDate(addr>>8);
	SPI_SendDate(addr);
	for(i=0;i<size;i++)
	{
	*date=SPI_ReceiveDate();
	date++;
	}
	CS_H;

}



/***************************************

函数名：W25Q64_WriteEnable()
函数参数：
函数返回值：
函数功能：W25Q64写使能
函数描述：
 
****************************************/
void W25Q64_WriteEnable()
{

	CS_L;
	//发送写使能指令
	SPI_SendDate(0x06);
	CS_H;

}

/***************************************

函数名：W25Q64_WriteDisable()
函数参数：
函数返回值：
函数功能：W25Q64写数据禁能
函数描述：
 
****************************************/
void W25Q64_WriteDisable()
{
	CS_L;
	SPI_SendDate(0x04);
	CS_H;
}

/***************************************

函数名：W25Q64_Sector_Erase()
函数参数：
u32 addr
函数返回值：
函数功能：W25Q64读使能
函数描述：
 
****************************************/
void W25Q64_Sector_Erase(u32 addr)
{
	W25Q64_WriteEnable();
  CS_L;
	//向MCU发送扇区擦除指令
	SPI_SendDate(0x20);
	
	SPI_SendDate(addr>>16);
	SPI_SendDate(addr>>8);
	SPI_SendDate(addr);
	CS_H;
	//等待通过读状态寄存器，等待指令执行完成
	W25Q64_ReadStatus();
	
}

/***************************************

函数名：W25Q64_Block_Erase()
函数参数：
函数返回值：
函数功能：W25Q64读使能
函数描述：
 
****************************************/
void W25Q64_Block_Erase(u32 addr)
{
	W25Q64_WriteEnable();
  CS_L;
	//向MCU发送扇区擦除指令
	SPI_SendDate(0xD8);
	SPI_SendDate(addr>>16);
	SPI_SendDate(addr>>8);
	SPI_SendDate(addr);
	CS_H;
	//等待通过读状态寄存器，等待指令执行完成
	W25Q64_ReadStatus();
	
}
/***************************************

函数名：W25Q64_Chip_Erase()
函数参数：
函数返回值：
函数功能：W25Q64读使能
函数描述：
 
****************************************/
void W25Q64_Chip_Erase()
{
	W25Q64_WriteEnable();
  CS_L;
	//向MCU发送扇区擦除指令
	SPI_SendDate(0xC7);
	CS_H;
	//等待通过读状态寄存器，等待指令执行完成
	W25Q64_ReadStatus();
	
}

/***************************************

函数名：W25Q64_ReadStatus()
函数参数：
函数返回值：
函数功能：W25Q64读状态
函数描述：
busy =0 执行完毕
busy =1 正在执行
 
****************************************/
void W25Q64_ReadStatus()
{
	u8 status;
	CS_L;
	//发送一个读状态指令
	SPI_SendDate(0x05);
	do{
	  status=SPI_ReceiveDate();
	}while((status & (1<<0))==1);
	
   CS_H;
}

