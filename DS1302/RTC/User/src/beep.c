#include "includes.h"
/******************************************************
* Name： BEEP_Init
* Function：  实现开发板蜂鸣器
* Description： 输出高电平
* Author：  sun
******************************************************/
void BEEP_Init()
{    	
  /* GPIO时钟配置 */	
	RCC->AHB1ENR|=1<<5;  //打开管脚时钟 PF
	
	/* PF8管脚配置 */
	/* 模式配置 */
	GPIOF->MODER &=~(0x3<<16);  //清零
	GPIOF->MODER |=(0x1<<16);  //配置成通用输出
	
	/* 输出类型配置 */ 
	GPIOF->OTYPER &=~(1<<8);  //配置成通用推挽
	
	/* 输出速率配置 */
	GPIOF->OSPEEDR &=~(0x3<<16);  //清零
	GPIOF->OSPEEDR |=(0x2<<16);   //配置成输出速率50M
	
	/* 上下拉模式配置 */
	GPIOF->PUPDR &=~(0x3<<16);  //清零
	GPIOF->PUPDR |=(0x1<<16);   //管脚初始化时输出高电平
	
}
/******************************************************
* Name： BEEP_KeyInit
* Function：  实现开发板蜂鸣器
* Description： 模拟键盘输出高电平 PF4
* Author：  sun
******************************************************/
void BEEP_KeyInit()
{
  /* GPIO时钟配置 */	
	RCC->AHB1ENR|=1<<5;  //打开管脚时钟 PF

	GPIOF->MODER &=~(0x3<<8); //配置为浮空输入模式
	//GPIOF->MODER |=(0x0<<8);
	
	GPIOF->PUPDR &=~(0x3<<8); // 清零
	GPIOF->PUPDR |= (0x1<<8); //配置成上拉模式  01 上拉GND  10下拉VCC

}
/******************************************************
* Name： BEEP_Water滴答
* Description： 输出高电平
* Author：  sun
******************************************************/
void BEEP_Water()
{
	BEEP_ON;
	delay(5000000);
	BEEP_OFF;
	delay(5000000);
	
}
