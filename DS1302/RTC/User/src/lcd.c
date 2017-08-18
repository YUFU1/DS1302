#include "includes.h"


/******************** LCD引脚初始化函数 *******************
LCD显示屏硬件管脚对应关系：
数据线：
DB0---PD14	DB1---PD15	DB2---PD0	DB3---PD1
DB4---PE7	DB5---PE8	DB6---PE9	DB7---PE10
DB8---PE11	DB9---PE12	DB10---PE13	DB11---PE14
DB12---PE15	DB13---PD8	DB14---PD9	DB15---PD10
控制线：
WR---PD5	RD---PD4	CS---PG12	RS---PF12	BL--PB15
**********************************************************/
void LCD_Port_Init(void)
{
	 /* 使能LCD硬件管脚时钟 */
	RCC->AHB1ENR |= (1<<3);//开PD
	RCC->AHB1ENR |= (1<<4);//开PE
	RCC->AHB1ENR |= (1<<6);//开PG
	RCC->AHB1ENR |= (1<<1);//开PB
	RCC->AHB1ENR |= (1<<5);//开PF
	
	
	/* 设置LCD硬件管脚模式 */
	/* DB0~DB15数据线：推挽输出  时钟50M */
	GPIOD->MODER |=(0X1<<(2*14))|(0X1<<(2*15));   //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*14))|(0X2ul<<(2*15)); //配置成输出速率50M 		
	
	GPIOD->MODER |=(0X1<<(2*0))|(0X1<<(2*1));     //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*0))|(0X2<<(2*1));   //配置成输出速率50M
	
	GPIOE->MODER |=0x55554000;                    //配置成通用输出  
	GPIOE->OTYPER |= 0;                           //配置成输出推挽
	GPIOE->OSPEEDR |=0x55554000;                  //配置成输出速率50M

	GPIOD->MODER |=(0X1<<(2*8))|(0X1<<(2*9))|(0X1<<(2*10));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*1))|(0X2<<(2*1))|(0X2<<(2*1));   //配置成输出速率50M
	
	/* 控制线：推挽输出  时钟50M 空闲为高电平 */
	GPIOD->MODER |=(0X1<<(2*4))|(0X1<<(2*5));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*4))|(0X2<<(2*5));
	GPIOD->ODR |= (1<<4)|(1<<5);
	
	GPIOG->MODER |=(0X1<<(2*12));              //配置成通用输出  
	GPIOG->OSPEEDR |=(0X2<<(2*12));
	GPIOG->ODR |= (1<<12);

	GPIOF->MODER |=(0X1<<(2*12));              //配置成通用输出  
	GPIOF->OSPEEDR |=(0X2<<(2*12));
	GPIOF->ODR |= (1<<12);
	
	GPIOB->MODER |=(0X1<<(2*15));               //配置成通用输出  
	GPIOB->OSPEEDR |=(0X2ul<<(2*15));
	GPIOB->ODR &= ~(1<<15);                     //关闭背光，在LCD初始化完后再打开                
}

/******************* LCD写命令函数 *********************************
参数：需要输入的命令
********************************************************************/
void LCD_ILI9486_CMD(u16 common)
{	
	GPIOF->ODR &= ~(1 << 12);    //D/C拉低
	GPIOG->ODR &= ~(1 << 12);    //CS拉低
	GPIOD->ODR &= ~(1 << 5);     //WR拉低

	/* 把命令发送到数据总线 */
	if(common&(1<<0))   GPIOD->BSRRL = (1<<14); else GPIOD->BSRRH = (1<<14);
	if(common&(1<<1))   GPIOD->BSRRL = (1<<15); else GPIOD->BSRRH = (1<<15);
	if(common&(1<<2))   GPIOD->BSRRL = (1<<0);  else GPIOD->BSRRH = (1<<0);
	if(common&0x08)   GPIOD->BSRRL = (1<<1);  else GPIOD->BSRRH = (1<<1);
	if(common&0x10)   GPIOE->BSRRL = (1<<7);  else GPIOE->BSRRH = (1<<7);
	if(common&0x20)   GPIOE->BSRRL = (1<<8);  else GPIOE->BSRRH = (1<<8);
	if(common&0x40)   GPIOE->BSRRL = (1<<9);  else GPIOE->BSRRH = (1<<9);
	if(common&0x80)   GPIOE->BSRRL = (1<<10); else GPIOE->BSRRH = (1<<10);
	if(common&0x0100) GPIOE->BSRRL = (1<<11); else GPIOE->BSRRH = (1<<11);
	if(common&0x0200) GPIOE->BSRRL = (1<<12); else GPIOE->BSRRH = (1<<12);
	if(common&0x0400) GPIOE->BSRRL = (1<<13); else GPIOE->BSRRH = (1<<13);
	if(common&0x0800) GPIOE->BSRRL = (1<<14); else GPIOE->BSRRH = (1<<14);
	if(common&0x1000) GPIOE->BSRRL = (1<<15); else GPIOE->BSRRH = (1<<15);
	if(common&0x2000) GPIOD->BSRRL = (1<<8);  else GPIOD->BSRRH = (1<<8);
	if(common&0x4000) GPIOD->BSRRL = (1<<9);  else GPIOD->BSRRH = (1<<9);
	if(common&(1<<15)) GPIOD->BSRRL = (1<<10); else GPIOD->BSRRH = (1<<10);
	GPIOD->ODR |= 1 << 5;        //WR拉高
	GPIOG->ODR |= 1 << 12;       //拉高CS
}

/******************* LCD写数据函数 *********************************
参数：需要输入的命令
********************************************************************/
void LCD_ILI9486_Parameter(u16 data)
{
	GPIOF->ODR |= 1 << 12;       //D/C拉高
	GPIOG->ODR &= ~(1 << 12);    //CS拉低	
	GPIOD->ODR &= ~(1 << 5);     //WR拉低
	/* 把数据发送到数据总线 */
	if(data&0x01)   GPIOD->BSRRL = (1<<14); else GPIOD->BSRRH = (1<<14);
	if(data&0x02)   GPIOD->BSRRL = (1<<15); else GPIOD->BSRRH = (1<<15);
	if(data&0x04)   GPIOD->BSRRL = (1<<0);  else GPIOD->BSRRH = (1<<0);
	if(data&0x08)   GPIOD->BSRRL = (1<<1);  else GPIOD->BSRRH = (1<<1);
	if(data&0x10)   GPIOE->BSRRL = (1<<7);  else GPIOE->BSRRH = (1<<7);
	if(data&0x20)   GPIOE->BSRRL = (1<<8);  else GPIOE->BSRRH = (1<<8);
	if(data&0x40)   GPIOE->BSRRL = (1<<9);  else GPIOE->BSRRH = (1<<9);
	if(data&0x80)   GPIOE->BSRRL = (1<<10); else GPIOE->BSRRH = (1<<10);
	if(data&0x0100) GPIOE->BSRRL = (1<<11); else GPIOE->BSRRH = (1<<11);
	if(data&0x0200) GPIOE->BSRRL = (1<<12); else GPIOE->BSRRH = (1<<12);
	if(data&0x0400) GPIOE->BSRRL = (1<<13); else GPIOE->BSRRH = (1<<13);
	if(data&0x0800) GPIOE->BSRRL = (1<<14); else GPIOE->BSRRH = (1<<14);
	if(data&0x1000) GPIOE->BSRRL = (1<<15); else GPIOE->BSRRH = (1<<15);
	if(data&0x2000) GPIOD->BSRRL = (1<<8);  else GPIOD->BSRRH = (1<<8);
	if(data&0x4000) GPIOD->BSRRL = (1<<9);  else GPIOD->BSRRH = (1<<9);
	if(data&0x8000) GPIOD->BSRRL = (1<<10); else GPIOD->BSRRH = (1<<10);
	GPIOD->ODR |= 1 << 5;        //WR拉高
	GPIOG->ODR |= 1 << 12;       //拉高CS
}

/********************** LCD清屏函数 ******************************************
参数：
SX、EX：X轴的起始坐标和结束坐标
SY、EY：Y轴的起始坐标和结束坐标
Color： 屏幕颜色
******************************************************************************/
void LCD_Clear(u16 SX, u16 EX, u16 SY, u16 EY, u16 Color)
{
	u32 i;
	
	LCD_ILI9486_CMD(0x2A);                      //发送0x2A命令，设置X轴坐标
	LCD_ILI9486_Parameter((SX & 0xff00) >> 8);  //发送X轴起始坐标的高8位
	LCD_ILI9486_Parameter((SX & 0x00ff));       //发送X轴起始坐标的低8位
	LCD_ILI9486_Parameter((EX & 0xff00) >> 8);  //发送X轴结束坐标的高8位
	LCD_ILI9486_Parameter((EX & 0x00ff));       //发送X轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2B);                      //发送0x2B命令，设置Y轴坐标
	LCD_ILI9486_Parameter((SY & 0xff00) >> 8);  //发送Y轴起始坐标的高8位
	LCD_ILI9486_Parameter((SY & 0x00ff));       //发送Y轴起始坐标的低8位
	LCD_ILI9486_Parameter((EY & 0xff00) >> 8);  //发送Y轴结束坐标的高8位
	LCD_ILI9486_Parameter((EY & 0x00ff));       //发送Y轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2C);                      //发送0x2A命令，设置LCD显示颜色
	for(i = 0; i < (EX - SX + 1) * (EY - SY + 1); i++)  //循环清屏区域的每一个像素点
	{
		LCD_ILI9486_Parameter(Color);
	}
}

/********************** LCD初始化函数 *******************************************/
void LCD_Init(void)
{
	LCD_Port_Init();
	//************* Reset LCD Driver ****************//
	Delay_ms(120); // Delay 120 ms
	//************* Start Initial Sequence **********//
	LCD_ILI9486_CMD(0XF2);
	LCD_ILI9486_Parameter(0x18);
	LCD_ILI9486_Parameter(0xA3);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0XB2);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0xFF);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0XF8);
	LCD_ILI9486_Parameter(0x21);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_CMD(0XF9);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_CMD(0x36);
	LCD_ILI9486_Parameter(0x08);  
	LCD_ILI9486_CMD(0x3A);
	LCD_ILI9486_Parameter(0x55);   //设置16位BPP
	LCD_ILI9486_CMD(0xB4);
	LCD_ILI9486_Parameter(0x01);//0x00
	LCD_ILI9486_CMD(0xB6);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0x22);
	LCD_ILI9486_CMD(0xC1);
	LCD_ILI9486_Parameter(0x41);
	LCD_ILI9486_CMD(0xC5);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x07);//0X18
	LCD_ILI9486_CMD(0xE0);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x1F);
	LCD_ILI9486_Parameter(0x1C);
	LCD_ILI9486_Parameter(0x0C);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_Parameter(0x48);
	LCD_ILI9486_Parameter(0x98);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x0A);
	LCD_ILI9486_Parameter(0x13);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_Parameter(0x11);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0xE1);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x32);
	LCD_ILI9486_Parameter(0x2E);
	LCD_ILI9486_Parameter(0x0B);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x05);
	LCD_ILI9486_Parameter(0x47);
	LCD_ILI9486_Parameter(0x75);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x06);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x03);
	LCD_ILI9486_Parameter(0x24);
	LCD_ILI9486_Parameter(0x20);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0x11);   //退出睡眠
	Delay_ms(120);
	LCD_ILI9486_CMD(0x29);   //开启显示
	
	/* 以下由用户按实际编写 */
	LCD_Clear(0,319,0,479,WHITE);  //把屏幕底色位绿色
	GPIOB->ODR |= 1 << 15;          //开LCD背光灯
}


/*****************************
函数名：LCD_Draw_Point
函数参数：
u16 x   u16 y   u16 color
横坐标  纵坐标   颜色

函数返回值：无
函数功能：在LCD屏显示一个点
函数描述：

*****************************/
void LCD_Draw_Point(u16 x ,u16 y ,u16 color)
{
	LCD_ILI9486_CMD(0x2A);                      //发送0x2A命令，设置X轴坐标
	LCD_ILI9486_Parameter((x & 0xff00) >> 8);  //发送X轴起始坐标的高8位
	LCD_ILI9486_Parameter((x & 0x00ff));       //发送X轴起始坐标的低8位
	LCD_ILI9486_Parameter((x & 0xff00) >> 8);  //发送X轴结束坐标的高8位
	LCD_ILI9486_Parameter((x & 0x00ff));       //发送X轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2B);                      //发送0x2B命令，设置Y轴坐标
	LCD_ILI9486_Parameter((y & 0xff00) >> 8);  //发送Y轴起始坐标的高8位
	LCD_ILI9486_Parameter((y & 0x00ff));       //发送Y轴起始坐标的低8位
	LCD_ILI9486_Parameter((y & 0xff00) >> 8);  //发送Y轴结束坐标的高8位
	LCD_ILI9486_Parameter((y & 0x00ff));       //发送Y轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2C);
	LCD_ILI9486_Parameter(color);
}



/*****************************
函数名：LCD_Draw
函数参数：
u16 x_start   u16 y_start    u16 x_end  u16 y_end  u16 color
起始横坐标  起始纵坐标   颜色  终点横坐标  终点纵坐标  颜色

函数返回值：无
函数功能：把LCD屏上某个区域清成某种颜色
函数描述：
*****************************/
void LCD_Draw(u16 x_start ,u16 y_start,u16 x_end ,u16 y_end ,u16 color)
{
	u16 i;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x_start>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x_start);//开始位置的低8位
	LCD_ILI9486_Parameter(x_end>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x_end);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y_start>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y_start);//开始位置的低8位
	LCD_ILI9486_Parameter(y_end>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y_end);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<(x_end -x_start)*(y_end -y_start);i++)
	{
		LCD_ILI9486_Parameter(color);
	}
}


/***************************************
函数名：LCD_Dis16hz
函数参数：
u16 x  u16 y  u16 color  u16 backcolor  u8 *buf
  起始位置     字体颜色   背景颜色       字摸数组
函数返回值：无
函数功能：在LCD显示16*16 大小的汉字
***************************************/
void LCD_Dis16hz(u16 x,u16 y,u16 color,u16 backcolor,u8 *buf)
{
	u16 i,j;
	u16 temp;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+15)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+15);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+15)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+15);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<16;i++)//行
	{
		//得到每一行的字摸
		temp = buf[2*i]<<8 |buf[2*i+1];
		for(j=0;j<16;j++)//列
		{
			 if(temp &(0x8000>>j))//汉字
			 {
				 LCD_ILI9486_Parameter(color);
			 }
			 else //背景
			 {
				 LCD_ILI9486_Parameter(backcolor);
			 }
		}
	}
}


/***************************************
函数名：LCD_Dis32hz
函数参数：
u16 x  u16 y  u16 color  u16 backcolor  u8 *buf
  起始位置     字体颜色   背景颜色       字摸数组
函数返回值：无
函数功能：在LCD显示32*32 大小的汉字
***************************************/
void LCD_Dis32hz(u16 x,u16 y,u16 color, const u8 *pic,u8 *buf)
{
	u16 i,j;
	u32 temp;
	u16 backcolor;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+31);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+31);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<32;i++)//行
	{
		//得到每一行的字摸
		temp = buf[4*i]<<24 |buf[4*i+1]<<16 |buf[4*i +2]<<8 |buf[4*i+3];
		for(j=0;j<32;j++)//列
		{
			 if(temp &(0x80000000>>j))//汉字
			 {
				 LCD_ILI9486_Parameter(color);
			 }
			 else //背景
			 {
				 backcolor =pic[(i+y)*320*2 +(j+x)*2]<<8 |pic[(i+y)*320*2 +(j+x)*2+1];
				 LCD_ILI9486_Parameter(backcolor);
			 }
		}
	}
}
void LCD_Dishz(u16 x,u16 y,u16 color,	u16 backcolor,u8 *buf)
{
	u16 i,j;
	u32 temp;

	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+31);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+31);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<32;i++)//行
	{
		//得到每一行的字摸
		temp = buf[4*i]<<24 |buf[4*i+1]<<16 |buf[4*i +2]<<8 |buf[4*i+3];
		for(j=0;j<32;j++)//列
		{
			 if(temp &(0x80000000>>j))//汉字
			 {
				 LCD_ILI9486_Parameter(color);
			 }
			 else //背景
			 {
				// backcolor =pic[(i+y)*320*2 +(j+x)*2]<<8 |pic[(i+y)*320*2 +(j+x)*2+1];
				 LCD_ILI9486_Parameter(backcolor);
			 }
		}
	}
}
/***************************************
函数名：LCD_Disanyhz
函数参数：
u16 x  u16 y  u16 width  u16 high u16 color  u16 backcolor  u8 *buf
  起始位置      汉字宽度    汉字高度  字体颜色   背景颜色       字摸数组
函数返回值：无
函数功能：在LCD显示任意大小的汉字
***************************************/
void LCD_Disanyhz(u16 x,u16 y,u16 width,u16 high,u16 color,u16 backcolor,u8 *buf)
{
	u16 i,j,k;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+width-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+width-1);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+high-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+high-1);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<high;i++)//行
	{
		//把一行拆分成n个8位
		for(k=0;k<width/8;k++)
		{
			 //8位处理
			for(j=0;j<8;j++)//列
			{
				 if(*buf &(0x80>>j))//汉字
				 {
					 LCD_ILI9486_Parameter(color);
				 }
				 else //背景
				 {
					 LCD_ILI9486_Parameter(backcolor);
				 }
			}
			buf++;
		}
		
	}
}


/***************************************
函数名：LCD_Dispic
函数参数：
u16 x  u16 y  u16 width  u16 high        u8 *buf
  起始位置      图片宽度    图片高度      字摸数组
函数返回值：无
函数功能：在LCD显示任意大小的图片
***************************************/
void LCD_Dispic(u16 x,u16 y,u16 width,u16 high ,const u8 *buf)
{
	u16 i,j;
	u16 color;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+width-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+width-1);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+high-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+high-1);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<high;i++)//行
	{
		for(j=0;j<width;j++)//列
		{
			//得到图片上每一点颜色值
			color = buf[i*width*2 +j*2] <<8 |buf[i*width*2 +j*2+1];
			LCD_ILI9486_Parameter(color);
		}
	}
	
}


/***************************************
函数名：LCD_Dispic1
函数参数：
 struct PIC pic ：图片信息结构体变量
函数返回值：无
函数功能：在LCD显示任意大小的图片
***************************************/
void LCD_Dispic1(struct PIC pic)
{
	u16 i,j;
	u16 color;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(pic.x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(pic.x);//开始位置的低8位
	LCD_ILI9486_Parameter((pic.x+pic.width-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(pic.x+pic.width-1);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(pic.y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(pic.y);//开始位置的低8位
	LCD_ILI9486_Parameter((pic.y+pic.high-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(pic.y+pic.high-1);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<pic.high;i++)//行
	{
		for(j=0;j<pic.width;j++)//列
		{
			//得到图片上每一点颜色值
			color = pic.buf[i*pic.width*2 +j*2] <<8 |pic.buf[i*pic.width*2 +j*2+1];
			LCD_ILI9486_Parameter(color);
		}
	}
	
}

/***************************************
函数名：LCD_Dispic2
函数参数：
u16 x  u16 y         u8 *buf
  起始位置          字摸数组
函数返回值：无
函数功能：在LCD显示任意大小的图片
  勾选 包含图像头信息
***************************************/
void LCD_Dispic2(u16 x,u16 y,const u8 *buf)
{
	u16 i,j;
	u16 color;
	u16 width;
	u16 high ;
	width =buf[2]<<8 |buf[3];
	high =buf[4]<<8 |buf[5];
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+width-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+width-1);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+high-1)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+high-1);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<high;i++)//行
	{
		for(j=0;j<width;j++)//列
		{
			//得到图片上每一点颜色值
			color = buf[i*width*2 +j*2+8] <<8 |buf[i*width*2 +j*2+1+8];
			LCD_ILI9486_Parameter(color);
		}
	}
	
}

/***************************************
函数名：LCD_Dis32ch
函数参数：
u16 x  u16 y  u16 color  u16 backcolor  u8 *buf
  起始位置     字体颜色   背景颜色       字摸数组
函数返回值：无
函数功能：在LCD显示32*32 大小的字符
***************************************/
void LCD_Dis32ch(u16 x,u16 y,u16 color,const u8 *pic,u8 *buf)
{
	u16 i,j;
	u32 temp;
	u16 backcolor;
	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+15)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+15);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+31);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<32;i++)//行
	{
		//得到每一行的字摸
		temp = buf[2*i]<<8 |buf[2*i+1];
		for(j=0;j<16;j++)//列
		{
			 if(temp &(0x8000>>j))//汉字
			 {
				 LCD_ILI9486_Parameter(color);
			 }
			 else //背景
			 {
				  backcolor =pic[(i+y)*320*2 +(j+x)*2]<<8 |pic[(i+y)*320*2 +(j+x)*2+1];
				 LCD_ILI9486_Parameter(backcolor);
			 }
		}
	}
}
void LCD_Diszf(u16 x,u16 y,u16 color,	u16 backcolor,u8 *buf)
{
	u16 i,j;
	u32 temp;

	//确定要显示区域的宽度范围
	LCD_ILI9486_CMD(0x2A); 
	LCD_ILI9486_Parameter(x>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x);//开始位置的低8位
	LCD_ILI9486_Parameter((x+15)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(x+15);//结束位置的低8位
	
	//确定要显示区域的高度范围
	LCD_ILI9486_CMD(0x2B); 
	LCD_ILI9486_Parameter(y>>8);//开始位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y);//开始位置的低8位
	LCD_ILI9486_Parameter((y+31)>>8);//结束位置的高8位，要通过低8位的数据线传输
	LCD_ILI9486_Parameter(y+31);//结束位置的低8位
	
	//在确定的范围内填充颜色
	LCD_ILI9486_CMD(0x2C); 
	for(i=0;i<32;i++)//行
	{
		//得到每一行的字摸
		temp = buf[2*i]<<8 |buf[2*i+1];
		for(j=0;j<16;j++)//列
		{
			 if(temp &(0x8000>>j))//汉字
			 {
				 LCD_ILI9486_Parameter(color);
			 }
			 else //背景
			 {
				 // backcolor =pic[(i+y)*320*2 +(j+x)*2]<<8 |pic[(i+y)*320*2 +(j+x)*2+1];
				 LCD_ILI9486_Parameter(backcolor);
			 }
		}
	}
}

/***************************************
函数名：LCD_DisString
函数参数：
u16 x  u16 y   u16 color  u16 backcolor  u8 *buf
  起始位置      字体颜色   背景颜色       字符串数组
函数返回值：无
函数功能：在LCD显示字符串

u8 *buf="今a天bc是2017你/07/31";
***************************************/
void LCD_DisString(u16 x,u16 y ,u16 color,const u8 *pic, u8 *buf)
{
	u32 AB ; //区码
	u32 CD ; //位码
	u32 addr; 
	u8 str[128];
	u8 ch[64];
	 while(*buf !='\0')//字符串数组中有要显示的内容
	 {
		 if(*buf>0xa0 && *(buf+1)>0xa0)//要显示的是汉字
		 {
			 //在GB2312汉字库中找到要显示的汉字
			 AB = (*buf-0xa1) *94; //汉字所在的区
			 CD = *(buf+1)-0xa1;   //汉字在区中所占的位
			 addr =(AB+CD)*128 ;   //汉字在字库文件中的地址
			 addr +=0;            //字库在存储芯片中的开始位置
				//从存储芯片中读出字摸
       W25Q64_Read_DateFromZK(addr,128,str);
					 //调取汉字显示函数
			 LCD_Dis32hz(x,y,color,pic,str);
			
			  buf +=2;
				  x +=32; //水平显示字符串
			 
		 }
		
		 else if(*buf >=' ' && *buf <='~')//要显示的是字符
		 {
			 //在ASCLL字符库中找到要显示的字符
			 addr = (*buf-32) *64 ;//字符在字库文件中的地址
			 addr += 1046790 ;   //至少汉字库在存储芯片中的偏移
			 //从存储芯片中读出字摸
	       W25Q64_Read_DateFromZK(addr,64,ch);
				 //调取字符显示函数
				 LCD_Dis32ch(x,y,color,pic,ch);
	
			 buf++;
			 x +=16;
			 
		 }
	 }
}



/***************************************
函数名：LCD_DisString
函数参数：
u16 x  u16 y   u16 color  u16 backcolor  u8 *buf
  起始位置      字体颜色   背景颜色       字符串数组
函数返回值：无
函数功能：在LCD显示字符串

u8 *buf="今a天bc是2017你/07/31";
***************************************/
void LCD_ShowString(u16 x,u16 y ,u16 color, u16 back, u8 *buf)
{
	 u8 i;
	u32 AB ; //区码
	u32 CD ; //位码
	u32 addr; 
	u8 str[128];
	u8 ch[64];
	 while(*buf !='\0')//字符串数组中有要显示的内容
	 {
		 if(*buf>0xa0 && *(buf+1)>0xa0)//要显示的是汉字
		 {
			 //在GB2312汉字库中找到要显示的汉字
			 AB = (*buf-0xa1) *94; //汉字所在的区
			 CD = *(buf+1)-0xa1;   //汉字在区中所占的位
			 addr =(AB+CD)*128 ;   //汉字在字库文件中的地址
			 addr +=0;            //字库在存储芯片中的开始位置
				//从存储芯片中读出字摸
       W25Q64_Read_DateFromZK(addr,128,str);
					 //调取汉字显示函数
			 LCD_Dishz(x,y,color,back,str);
			
			  buf +=2;
				  x +=32; //水平显示字符串
			 
		 }
		
		 else if(*buf >=' ' && *buf <='~')//要显示的是字符
		 {
			 //在ASCLL字符库中找到要显示的字符
			 addr = (*buf-32) *64 ;//字符在字库文件中的地址
			 addr += 1046790 ;   //至少汉字库在存储芯片中的偏移
			 //从存储芯片中读出字摸
	       W25Q64_Read_DateFromZK(addr,64,ch);
				 //调取字符显示函数
				 LCD_Diszf(x,y,color,back,ch);
			 buf++;
			 x +=16;
			 
		 }
	 }
}












