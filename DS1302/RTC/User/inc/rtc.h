#ifndef RTC_H
#define RTC_H
#include "stm32f4xx.h"

#define RSTL GPIOG->ODR&=~(1<<15)
#define RSTH GPIOG->ODR|=(1<<15)
#define SCLKL GPIOD->ODR&=~(1<<6)
#define SCLKH GPIOD->ODR|=(1<<6)
#define SDAL  GPIOD->ODR&=~(1<<7)
#define SDAH  GPIOD->ODR|=(1<<7)


#define DS1302_SECOND_WRITE  0x80          
#define	DS1302_MINUTE_WRITE  0x82
#define	DS1302_HOUR_WRITE    0x84
#define	DS1302_DAY_WRITE     0x86
#define	DS1302_MONTH_WRITE   0x88
#define	DS1302_YEAR_WRITE    0x8C
#define	DS1302_WEEK_WRITE    0X8A

#define DS1302_SECOND_READ  0x81          
#define	DS1302_MINUTE_READ   0x83
#define	DS1302_HOUR_READ     0x85
#define	DS1302_DAY_READ     0x87
#define	DS1302_MONTH_READ    0x89
#define	DS1302_YEAR_READ    0x8D
#define	DS1302_WEEK_READ     0X8B

void RST_out(void);
void CLK_out(void);
void SDA_out(void);
void SDA_int(void);
u8 DS1302_Read(u8 cmd);
void DS1302_Write(u8 cmd,u8 dat);
void DS1302_WriteByte(u8 dat);
u8 DS1302_ReadByte(void);
void DS1302_init(void);
void DisplayTime(void);

#endif

