#include "includes.h"


/*****************************
函数名：Delay_ms
函数参数：
u16 xms: 表示要延时的毫秒数

函数返回值：无
函数功能：实现毫秒级别的延时
函数描述：
一次最长定时 x  ms 
     21000 * x <=2^24
	         x <= 798
*****************************/
void  Delay_ms(u16 xms)
{
	 // 1、选择外部时钟作为滴答定时器的时钟源
	 SysTick->CTRL &=~(1<<2);
   // 2、把计数初始值写入重装载寄存器
   SysTick->LOAD = 21000 *xms;
   // 3、对当前值寄存器清零，让重装载值加载到当前值寄存器
	 SysTick->VAL =0;
   //4、打开定时器时钟，开始计数
   SysTick->CTRL |=(1<<0);	
   //5、等待当前值寄存器递减到0(表示定时时间到了)
   while((SysTick->CTRL &(1<<16))==0);
	 //6、关闭定时器时钟，停止计数 
	 SysTick->CTRL &=~(1<<0);
}



/*****************************
函数名：Delay_us
函数参数：
u32 xus: 表示要延时的微秒数

函数返回值：无
函数功能：实现微秒级别的延时
函数描述：

*****************************/
void  Delay_us(u32 xus)
{
	 // 1、选择外部时钟作为滴答定时器的时钟源
	 SysTick->CTRL &=~(1<<2);
   // 2、把计数初始值写入重装载寄存器
   SysTick->LOAD = 21 *xus;
   // 3、对当前值寄存器清零，让重装载值加载到当前值寄存器
	 SysTick->VAL =0;
   //4、打开定时器时钟，开始计数
   SysTick->CTRL |=(1<<0);	
   //5、等待当前值寄存器递减到0(表示定时时间到了)
   while((SysTick->CTRL &(1<<16))==0);
	 //6、关闭定时器时钟，停止计数 
	 SysTick->CTRL &=~(1<<0);
}


