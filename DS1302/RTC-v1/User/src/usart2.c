#include "includes.h"
/******************************************************
* Name： Usart2_Init() 
* Parameter：波特率 int boand
* Function：  通信模块初始化
* Description： PA2 PA3
* Author：  sun
******************************************************/

void Usart2_Init(int boand)
{
	  float USARTDIV;     //放入波特率寄存器中的值
	  int DIV_Fraction; //保留波特率小数部分
    int DIV_Mantissa; //保留波特率整数部分
	  int OVER8=0;
	
	
   /**打开PA的时钟线**/
	 RCC->AHB1ENR |=(0x1<<0);
	 
	/**配置USART2的PA2,PA3 TX RX的复用功能配置**/
	  
	  // XX XX XX XX  GPIOA
	  // 11 11 00 00  0xf<<4
  	// 00 00 11 11 ~(0xf<<4);
	  // 00 00 XX XX  &
    // 10 10 00 00  OxA<<4
	  // 10 10 XX XX
	 
		/**配置PA2，PA3的TX功能**/
	
	   //模式寄存器 1010 // 10 =a
	
//	  GPIOA->MODER &=~(0xf<<4);
//	  GPIOA->MODER |=(0xa<<4); 

		GPIOA->MODER &=~(0x3<<4);
		GPIOA->MODER |=(0x2<<4); 
		

		GPIOA->AFR[0] &=~(0xf<<8);
		GPIOA->AFR[0] |=(0x7<<8); //0111
	
	
		
		GPIOA->MODER &=~(0x3<<6);
		GPIOA->MODER |=(0x2<<6);	//输出类型寄存器
		
	 
	  // 0111 0111  1+2+4
  	
		//复用功能配置
	
//   	GPIOA->AFR[0] &=~(0xff<<8);
//  	GPIOA->AFR[0] |=(0x77<<8);

		

		GPIOA->AFR[0] &=~(0xf<<12);
		GPIOA->AFR[0] |=(0x7<<12);

	
	/**对UASRT2的接收发送功能配置**/
	
	   //打开USART2的外设时钟
		 RCC->APB1ENR |=(0x1<<17);
		 
		 //串口发送使能位开启
		 USART2->CR1 |=(0x1<<13);
		 
		 //禁止奇偶校验
		 USART2->CR1 &=~(0x1<<10);
		 
		 //发送器使能
		 USART2->CR1 |=(0x1<<3);
		 
		 //接收器使能
		 USART2->CR1 |=(0x1<<2);
		 
		 //不发送断路字符 置0
		 
		 USART2->CR1 &=~(0x1<<0);
		 
		 
#ifdef Usart2_OVER8  //16倍
		 USART2->CR1 &=~(0x1<<15);
		 OVER8=0;
#else
     USART2->CR1 |=(0x1<<15);
     OVER8=1;
#endif
		 
		 /**设置波特率**/
		 		 
		 
		 USARTDIV=(float)(42000000/(8*(2-OVER8)*boand));
		 DIV_Mantissa = USARTDIV;
		 DIV_Fraction = (USARTDIV-DIV_Mantissa)*8*(2-OVER8);
     USART2->BRR = DIV_Mantissa<<4 | DIV_Fraction;
}   

/******************************************************
* Name： Usart2_Sendbyte() 
* Parameter: 字符串
* Function：  usart2发送函数  u8 date 
* Description： 
* Author：  sun
******************************************************/
void Usart2_Sendbyte(u8 date)
{
 //等待上次的数据完成发送
	while((USART2->SR & (0x1<<6)) == 0);
	  USART2->DR = date;

}


/*****************************
函数名：USART1_Sendstring
函数参数：
u8  *buf : 字符指针，
     表示要发送字符串的首地址
函数返回值：无
函数功能：实现串口1发送一个字符串
函数描述：
*****************************/
void Usart2_SendString(u8 *buf)
{
	 while(*buf !='\0')
	 {
		  Usart2_Sendbyte(*buf);
		  buf++;
	 }
	 Usart2_Sendbyte('\r');
	 Usart2_Sendbyte('\n');
}


/******************************************************
* Name： Usart2_Sendbyte() 
* Parameter: 单个字符
* Function：  usart2发送函数  u8 date 
* Description： 
* Author：  sun
******************************************************/
u8 Usart2_Receivebyte()
{
	u8 date;
 //等待上次的数据接收完成 
	while((USART2->SR &(0x1<<5))==0)
	;
	
 //接收该数据
    date	= USART2->DR;
	//返回接收到的数据
	return date;
}

void Usart2_Exit_Init()
{
  Usart2_Init(9600);
	RCC->APB2ENR |=(0x1<<14);
	USART2->CR1 |=(0x1<<5);
	My_NVIC_Init(7-2,2,3,USART2_IRQn);
}

void USART2_IRQHandler(void)
{
  static char buf[30];
	static int i=0;
	USART2->SR &=~(0x1<<5);

	buf[i]=USART2->DR;
	if(buf[i]!='\r'||buf[i]!='\n')
	{
	  if(strcmp(buf,"abc")==0)
		{
			LED1_ON;
		 
		}
		if(strcmp(buf,"bcd")==0)
		{
		  LED1_OFF;
		}
		i++;
	}
}

