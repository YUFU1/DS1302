#include "includes.h"

/******************************************************
* Name： Usart1_Init() 
* Parameter：波特率
* Function：  通信模块初始化
* Description： PA10 PA9
* Author：  sun
******************************************************/
void Usart1_Init(int boand)
{
	 
	 float USARTDIV;  	//放入波特率寄存器中的数值
	 int OVER8=0;
   int DIV_Fraction; //表示USARTDIV的小数部分
	 int DIV_Mantissa; //表示USARTDIV的整数部分
	/*****************************************************************************/
	
	/**对USART1的TX RX 管脚的复用配置**/
	
	 //打开PA的时钟线
	  RCC->AHB1ENR |=(0x1<<0);
	
		/**对USART1的TX PA9 复用功能配置**/
	
	//模式寄存器配置
	 GPIOA->MODER &=~(0x3<<18); //清零
	 GPIOA->MODER |=(0x2<<18);  // 复用功能配置
	
	//具体的复用功能配置
	 GPIOA->AFR[1] &=~(0xf<<4); //清零
	 GPIOA->AFR[1] |= (0x7<<4); //复用成USART_TX
	
		/*****************************************************************************/

		/**对USART1的TX PA10 复用功能配置**/
	
	//模式寄存器配置
	 GPIOA->MODER &=~(0x3<<20); //清零
	 GPIOA->MODER |=(0x2<<20);  // 复用功能配置
	
	//具体的复用功能配置
	 GPIOA->AFR[1] &=~(0xf<<8); //清零
	 GPIOA->AFR[1] |= (0x7<<8); //复用成USART_TX
	 
	 	/*****************************************************************************/
	 
		/**对USART1的发送接收的功能配置**/
		
		//打开USART1的外设时钟
	    RCC->APB2ENR |=(0x1<<4);
		
		 //串口发送使能位开启
		 USART1->CR1 |=(0x1<<13);
		
		 //禁止奇偶校验
		 USART1->CR1 &=~(0x1<<10);
		 
		 //发送器使能
		 USART1->CR1 |=(0x1<<3);
		 //接收器使能
		 USART1->CR1 |=(0x1<<2);
		 
		 //不发送断路字符
		 USART1->CR1 &=~(0x1<<0);
		 
#ifdef Usart1_OVER8  //16倍
		 USART1->CR1 &=~(1<<15);
		 OVER8=0;
#else
     USART1->CR1 |=(0x1<<15);
     OVER8=1;
#endif

		 /*****************************************************************************/
		 
		 /**TX RX	波特率配置**/
		 
		 USARTDIV=(float)84000000/(8*(2-OVER8)*boand);
		 DIV_Mantissa = USARTDIV;
		 DIV_Fraction = (USARTDIV-DIV_Mantissa)*8*(2-OVER8);
		 USART1->BRR = DIV_Mantissa<<4 |DIV_Fraction;
		 
	
}



/******************************************************
* Name： Usart1_Sendbyte() 
* Parameter: 单个字符
* Function：  usart1发送函数  u8 date 
* Description： 
* Author：  sun
******************************************************/
void Usart1_Sendbyte(u8 date)
{
 //等待上次的数据发送完成 // 一般都是状态寄存器
	while((USART1->SR &(0x1<<6))==0);
 //发送该数据
	USART1->DR = date;

}



/******************************************************
* Name： Usart1_SendString() 
* Parameter: 字符串
* Function：  usart1发送函数  u8 date 
* Description： 
* Author：  sun
******************************************************/
void Usart1_SendString(char date[])
{
	int i=0;
 //发送该数据
	while(date[i]!='\0')
	USART1->DR = date[i++];
}


/******************************************************
* Name： Usart1_Sendbyte() 
* Parameter: 单个字符
* Function：  usart1发送函数  u8 date 
* Description： 
* Author：  sun
******************************************************/
u8 Usart1_Receivebyte()
{
	u8 date;
 //等待上次的数据接收完成 
	while((USART1->SR &(0x1<<5))==0)
	;
	
 //接收该数据
    date	= USART1->DR;
	//返回接收到的数据
	return date;
}
/******************************************************
* Name： Usart1_SendString() 
* Parameter: 字符串
* Function：  usart1发送函数  u8 *buf 
* Description： 
* Author：  sun
******************************************************/
void Usart1_Receivestring(u8 *buf)
{
	 u8 ch; 
	 while(1)
	 {
		  ch =Usart1_Receivebyte();
		 //判断接收到的是否是结束标志
		 if(ch ==' ' || ch =='\r' || ch=='\n')
		 {
			 break;
		 }
		 *buf =ch;
		 buf++;
	 }
	 *buf ='\0';//添加一个结束标志 
}

#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	Usart1_Sendbyte(ch);
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch) {
  Usart1_Sendbyte(ch);
}



void Usart1_Exit_Init()
{
	RCC->APB2ENR |=(0x1<<14);
	USART1->CR1 |=(0x1<<5);
	My_NVIC_Init(7-2,2,3,USART1_IRQn);
}

void USART1_IRQHandler(void)
{
//  static char buf[30];
//	static int i=0;
//	USART1->SR &=~(0x1<<5);
//	buf[i]=USART1->DR;
//	if(buf[i]!='\r'||buf[i]!='\n')
//	{
//	  if(strcmp(buf,"abc")==0)
//		{
//		 LED2_ON;
//		}
//		i++;
//	}
	u8 date;
  //static u32 addr=0; // 存储汉字库的开始地址
	static u32 addr=1046790; // 存储ASCLL的开始地址
	date = USART1->DR;
	W25Q64_Page_Program(addr++,date);
}
