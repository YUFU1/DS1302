#include "includes.h"
/*************************************
函数名：DS1302_IO_init()
函数参数：无
函数返回值：无
函数功能：
函数描述:
CLK   PD6
DAT   PD7
RST   PG15
**************************************/
void RST_out(void)
{
		//打开外设时钟
	RCC->AHB1ENR |=(1<<6);
		/**PG15配置为推挽模式**/
	//端口模式寄存器
	GPIOG->MODER &=~(0x3ul<<30);  //清零
	GPIOG->MODER |=(1ul<<30);     //配置为通用输出模式
	//输出类型
	GPIOG->OTYPER &=~(1<<15);   //推挽输出
	//输出速率
	GPIOG->OSPEEDR &=~(0x3ul<<30); //清零
	GPIOG->OSPEEDR |=(0x2ul<<30); //输出速率50M
	//上下拉
	GPIOG->PUPDR &=~(0x3ul<<30);  //清零
	GPIOG->PUPDR |=1ul<<30;
}
void CLK_out(void)
{
	//打开外设时钟
	RCC->AHB1ENR |=(1<<3);
	/**PD6配置为推挽模式**/
	//端口模式寄存器
	GPIOD->MODER &=~(0x3<<12);  //清零
	GPIOD->MODER |=(1<<12);     //配置为通用输出模式
	//输出类型
	GPIOD->OTYPER &=~(1<<6);   //推挽输出
	//输出速率
	GPIOD->OSPEEDR &=~(0x3<<12); //清零
	GPIOD->OSPEEDR |=(0x2<<12); //输出速率50M
	//上下拉
	GPIOD->PUPDR &=~(0x3<<12);  //清零
	GPIOD->PUPDR |=1ul<<12;
	
}

void SDA_out(void)
{
	//打开外设时钟
	RCC->AHB1ENR |=(1<<3);
	/**PD7配置为推挽模式**/
	//端口模式寄存器
	GPIOD->MODER &=~(0x3<<14);  //清零
	GPIOD->MODER |=(1<<14);     //配置为通用输出模式
	//输出类型
	GPIOD->OTYPER &=~(1<<7);   //推挽输出
	//输出速率
	GPIOD->OSPEEDR &=~(0x3<<14); //清零
	GPIOD->OSPEEDR |=(0x2<<14); //输出速率50M
	//上下拉
	GPIOD->PUPDR &=~(0x3<<14);  //清零	
	GPIOD->PUPDR |=1ul<<14;
}
void SDA_int(void)
{
	//打开外设时钟
	RCC->AHB1ENR |=(1<<3);
	//端口模式寄存器
	GPIOD->MODER &=~(0x3<<14);  //清零
	//上下拉
	GPIOD->PUPDR &=~(0x3<<14);  //清零	
}

/************************************************
*名称:DS1302Read()
*说明: 先写地址，后读数据
*功能: 从cmd相应地址中读取一个字节的数据
*调用:DS1302WriteByte(),DS1302ReadByte()
*输入:cmd:要写入的控制字节
*输出:dat:读取的数据
*************************************************/
u8 DS1302_Read(u8 cmd)
{
	u8 dat;
	RST_out();
	CLK_out();
	RSTL;//初始CE线置为0
	SCLKL;//初始时钟线置为0
	RSTH;//初始CE置为1，传输开始
	DS1302_WriteByte(cmd);//传输命令字，要读取的时间/日历地址
	dat=DS1302_ReadByte();//读取要得到的时间/日期
	SCLKH;//时钟线拉高
	RSTL;//读取结束， CE置为0，结束数据的传输
	return dat;//返回得到的时间/日期
}
/************************************************
*名称:DS1302Write
*说明: 先写地址，后写数据
*功能: 向cmd相应地址中写一个字节的数据
*调用:DS1302WriteByte()
*输入:cmd:要写入的控制字,dat:要写入的数据
*输出: 无
*************************************************/
void DS1302_Write(u8 cmd,u8 dat)
{
	RST_out();
	CLK_out();
	RSTL;//初始CE线置为0
	SCLKL;//初始时钟线置为0
	RSTH;//初始CE置为1，传输开始
	Delay_us(2);
	DS1302_WriteByte(cmd);//传输命令字，要写入的时间/日历地址
	DS1302_WriteByte(dat);//写入要修改的时间/日期
	SCLKH;//时钟线拉高
	RSTL;//读取结束， CE置为0，结束数据的传输
}
/************************************************
*名称:DS1302WriteByte
*说明: 无
*功能: 写入8bit数据
*调用:delayus()
*输入:dat:要写入的数据
*输出: 无
*************************************************/
void DS1302_WriteByte(u8 dat)
{
	u8 i;
	SDA_out();
	CLK_out();
	SCLKL;
	Delay_us(2);
	for(i=0;i<8;i++)
	{
		
	  if((dat&(0x01<<i))) //注意DS1302的数据和地址都是从最低位开始传输的
		{
	    SDAH;
		}
		else
		{
			SDAL;
		}
		Delay_us(2);
		SCLKH;
		Delay_us(2);
		SCLKL;
	  
	}
	 
}
/************************************************
*名称:DS1302ReadByte()
*说明: 无
*功能: 读取8bit的数据
*调用:delayus()
*输入: 无
*输出:dat:读取的数据
*************************************************/
u8 DS1302_ReadByte(void)
{
	u8 i,dat=0;
	SDA_int();
	CLK_out();
	Delay_us(2);
	for(i=0;i<8;i++)
	{ 
		
		if((GPIOD->IDR&(1<<7)))
		{
			dat|=0x01<<i;
	  }
		else
	  {
		   dat&=~(0x01<<i);
	  }
		SCLKH;
		Delay_us(2);
		SCLKL;
		Delay_us(2);
		}
	return dat;
}



/************************************************
*名称:void DS1302_init(void)
*说明: 给1302写入一个初始的值
*功能: 写入日期，和时钟的值
*调用:DS1302Write（）
*输入: 无
*输出: 无
*************************************************/
void DS1302_init(void)
{
	DS1302_Write(0x8e,0x00);//写保护关
	DS1302_Write(DS1302_SECOND_WRITE,0x00); 
	DS1302_Write(DS1302_MINUTE_WRITE,0x29);
	DS1302_Write(DS1302_HOUR_WRITE,0x11);
	DS1302_Write(DS1302_WEEK_WRITE,0x05);
	DS1302_Write(DS1302_DAY_WRITE,0x18); 
	DS1302_Write(DS1302_MONTH_WRITE,0x08);
	DS1302_Write(DS1302_YEAR_WRITE,0x17);
	DS1302_Write(0x90,0x01); //充电
	DS1302_Write(0xc0,0xf0); //初始化一次标示
	DS1302_Write(0x8e,0x80);
}
/************************************************
函数名：DisplayTime(void)
函数参数：无
函数返回值：无
函数功能：
函数描述: 
*************************************************/
void DisplayTime(void)
{
	u8 str1[20];
	u8 str2[20];
	u8 str3[20];
	u8 year,month,day,hour,minute,second,week;
	u8 temp_year,temp_month,temp_day,temp_hour,temp_minute,temp_second;
	LCD_Init();
	while(1)
	{
	 
  temp_year=DS1302_Read(DS1302_YEAR_READ);
	temp_month=DS1302_Read(DS1302_MONTH_READ);
	temp_day=DS1302_Read(DS1302_DAY_READ);
  temp_hour=DS1302_Read(DS1302_HOUR_READ);
	temp_minute=DS1302_Read(DS1302_MINUTE_READ );
	temp_second=DS1302_Read(DS1302_SECOND_READ);
	week=DS1302_Read(DS1302_WEEK_READ);	
	year=(temp_year/16)*10+temp_year%16;
  month=(temp_month/16)*10+temp_month%16;
  day=(temp_day/16)*10+temp_day%16;
  hour=(temp_hour/16)*10+temp_hour%16;
  minute=(temp_minute/16)*10+temp_minute%16;		
	second=	(temp_second/16)*10+temp_second%16;		
	sprintf((char*)str1,"20%02d年%02d月%02d日",year,month,day);
	sprintf((char*)str2,"%02d:%02d:%02d",hour,(char)minute,second);	
	sprintf((char*)str3,"星期%d",(char)week);
	LCD_ShowString(110,140,RED,WHITE,"实时时钟");
  LCD_ShowString(48,180,RED,WHITE,str1);
	LCD_ShowString(80,212,RED,WHITE,str2);
	LCD_ShowString(104,244,RED,WHITE,str3);
  }
}

