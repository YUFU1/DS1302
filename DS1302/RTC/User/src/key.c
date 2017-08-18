#include "includes.h"
	/******************************************************
	* Name： KEY_Init
	* Function：  实现开发板对应的KEY的输入功能配置
	* Description： PA0 PE2 PE3 PE4
	* Author：  sun
	******************************************************/
	void KEY_Init()
	{

		/**打开GPIOA管脚的时钟**/
		 RCC->AHB1ENR |=(0x1<<0);
		
		/**打开GPIOE管脚的时钟**/
		 RCC->AHB1ENR |=(0x1<<4);
		
		/**设置PA0端口的输入模式**/

		GPIOA->MODER &=~(0x3<<0); // 清零
	// 	GPIOA->MODER |= (0x0<<0); //配置输入模式
		
		/**设置PA0端口的无上下拉模式**/
		
		GPIOA->PUPDR &=~(0x3<<0); // 清零 
	//	GPIOA->PUPDR |=(0x0<<0); // 配置无上下拉 
		
		
		
		/**设置PE2端口的输入模式**/
		GPIOE->MODER &=~(0x3<<4); // 清零
	//	GPIOE->MODER |= (0x0<<4);  //配置输入模式
		
		/**设置PF2端口的无上下拉模式**/
		
		GPIOE->PUPDR &=~(0x3<<4);// 清零
	//	GPIOE->PUPDR |=(0x0<<4); // 配置无上下拉 



			/**设置PE3端口的输入模式**/
		GPIOE->MODER &=~(0x3<<6);// 清零
  //GPIOE->MODER |= (0x0<<6);  //配置输入模式 
		
		/**设置PE3端口的无上下拉模式**/
		
		GPIOE->PUPDR &=~(0x3<<6); // 清零
	//GPIOE->PUPDR |=(0x0<<6); // 配置无上下拉  



			/**设置P4端口的输入模式**/
		GPIOE->MODER &=~(0x3<<8);// 清零
		//GPIOE->MODER |= (0x0<<8);  //配置输入模式 
		
		/**设置PE4端口的无上下拉模式**/
		
		GPIOE->PUPDR &=~(0x3<<8); // 清零
		//GPIOE->PUPDR |=(0x0<<8); //配置无上下拉   
		
	}
	
		/******************************************************
	* Name： Get_Value
	* Function：  得到对应的按键值
	* Description： KEY1 KEY2 KEY3 KEY4
	* Author：  sun
	******************************************************/
	int Get_Value()
	{
		if(KEY1_ON ==1)
			return 1;
		else if(KEY2_ON == 0)
			return 2;
		else if(KEY3_ON == 0)
			return 3;
		else if(KEY4_ON == 0)
			return 4;
		return 0;

	
	}
		
	
	/******************************************************
	* Name： 
	* Function：  按键EXTI的外部中断触发软件
	* Description：  PA0 PE2 PE3 PE4
	* Author：  sun
	******************************************************/
	
	
	/**
	 配置步骤：
	1、把按键对应的IO口配置成输入模式
	打开SYSCFG的外设时钟
	***********************************
	---- PA0 
	
		1、选择PA0作为EXTIO的触发管脚 -- 上升沿触发
		2、允许EXTI0上触发中断
		3、EXTI0的NVIC配置
	 	
	******************************** **/
	void Key_EXTI_Init()
	{
		// 按键初始化
	  KEY_Init();
		
		// 打开SYSCFG的外设时钟
	  RCC->APB2ENR |=(0x1<<14);
		
		
		/************配置PA0**************/
	   
		//选择PA0作为EXTI0的触发管脚
		SYSCFG->EXTICR[0] &=~(0xf<<0); 
		
		// 将EXTI0设置为上升沿触发
		EXTI->RTSR |=(0x1<<0);
		
		//允许EXTI0的中断触发
		EXTI->IMR |=(0x1<<0);
		
		//EXTI0的NVIC配置
	  My_NVIC_Init(7-2,3,1,EXTI0_IRQn);
		
		
			/************配置PE2**************/
	   
		//选择PE2作为EXTI2的触发管脚
		SYSCFG->EXTICR[0] &=~(0xf<<8);
	  SYSCFG->EXTICR[0] |=(0x4<<8);
		
		// 将EXTI2设置为下降沿触发
		EXTI->FTSR |=(0x1<<2);
		
		//允许EXTI2的中断触发
		EXTI->IMR |=(0x1<<2);
		
		//EXTI0的NVIC配置
	  My_NVIC_Init(7-2,2,1,EXTI2_IRQn);
		
		
		
			/************配置PE3**************/
	   
		//选择PE3作为EXTI2的触发管
		SYSCFG->EXTICR[0] &=~(0xf<<12);
	  SYSCFG->EXTICR[0] |=(0x4<<12);
		
		// 将EXTI3设置为下降沿触发
		EXTI->FTSR |=(0x1<<3);
		
		//允许EXTI3的中断触发
		EXTI->IMR |=(0x1<<3);
		
		//EXTI3的NVIC配置
	  My_NVIC_Init(7-2,1,1,EXTI3_IRQn);
		
		
		
			/************配置PE4**************/
	   
		//选择PE4作为EXTI0的触发管脚
		SYSCFG->EXTICR[1] &=~(0xf<<0);
	  SYSCFG->EXTICR[1] |=(0x4<<0);
		
		// 将EXTI4设置为下降沿触发
		EXTI->FTSR |=(0x1<<4);
		
		//允许EXTI4的中断触发
		EXTI->IMR |=(0x1<<4);
		
		//EXTI4的NVIC配置
	  My_NVIC_Init(7-2,0,1,EXTI4_IRQn);
		
	}
	void  EXTI0_IRQHandler(void)
	{
		//清除中断标志
		EXTI->PR|=(0x1<<0);
    LED1_ON;
		
 	}
	void  EXTI2_IRQHandler(void)
	{
		EXTI->PR|=(0x1<<2);
		LED1_OFF;
	
 	}
		void  EXTI3_IRQHandler(void)
	{
		 EXTI->PR|=(0x1<<3);
		 LED_ALLON();
	
 	}

		void  EXTI4_IRQHandler(void)
	{
		EXTI->PR|=(0x1<<4);
		LED_ALLOFF();

 	}
	
	void WJ_Exit_Init()
	{
	 /**
		 打开PF的时钟线
		**/
		RCC->AHB1ENR |=(0x1<<5);
		
		
		
		//打开SYSCFG的时钟线
				
		RCC->APB2ENR |=(0x1<<14);
		
		 
		/**PF14 PF15 PF7**/
		
		
		
			/************配置PF7**************/
		
		//设置PF7的输入模式
		GPIOF->MODER &=~(0x3<<14);
		GPIOF->MODER |=(0x0<<14);
		
		//设置PF7的上下拉模式
		
		GPIOF->PUPDR &=~(0x3<<14);
		GPIOF->PUPDR |=(0x2<<14); // 设置为下拉模式
	   
		//选择PF7作为EXTI7的触发管脚
		SYSCFG->EXTICR[1] &=~(0xf<<12);
    SYSCFG->EXTICR[1] |=(0x5<<12);		
		
		// 将EXTI7设置为上升沿触发
		EXTI->RTSR |=(0x1<<7);
		
		//允许EXTI7的中断触发
		EXTI->IMR |=(0x1<<7);
		
		//EXTI7的NVIC配置
	  My_NVIC_Init(7-2,3,1,EXTI9_5_IRQn);
		
	
		
		/************配置PF14**************/
		
			
		//设置PF14的输入模式
		GPIOF->MODER &=~(0x3<<28);
		GPIOF->MODER |=(0x0<<28);
		
		//设置PF14的上下拉模式
		
		GPIOF->PUPDR &=~(0x3<<28);
		GPIOF->PUPDR |=(0x2<<28); // 设置为下拉模式
	   
		//选择PF14作为EXTI14的触发管脚
		SYSCFG->EXTICR[3] &=~(0xf<<8);
    SYSCFG->EXTICR[3] |=(0x5<<8);		
		
		// 将EXTI14设置为下降沿触发
		EXTI->RTSR |=(0x1<<14);
		
		//允许EXTI14的中断触发
		EXTI->IMR |=(0x1<<14);
		
		//EXTI14的NVIC配置
	  My_NVIC_Init(7-2,2,1,EXTI15_10_IRQn);
		
		
		/************配置PF15**************/
		
		
		
		//设置PF15的输入模式
		
		GPIOF->MODER &=~((unsigned int)0x3<<30);
		GPIOF->MODER |=((unsigned int)0x0<<30);
		
		//设置PF15的上下拉模式
		
		GPIOF->PUPDR &=~((unsigned int)0x3<<2*15);
		GPIOF->PUPDR |=((unsigned int)0x2<<2*15); 
		
		// 设置为下拉模式
		
		
	   
		//选择PF15作为EXTI15的触发管脚
		SYSCFG->EXTICR[3] &=~(0xf<<12);
    SYSCFG->EXTICR[3] |=(0x5<<12);		
		
		// 将EXTI15设置为上沿触发
		EXTI->RTSR |=(0x1<<15);
		
		//允许EXTI15的中断触发
		EXTI->IMR |=(0x1<<15);
		
		//EXTI15的NVIC配置
	  My_NVIC_Init(7-2,1,1,EXTI15_10_IRQn);
		
	}
void EXTI9_5_IRQHandler(void)
{
 
	//执行PE7的中断清除标志
	if(EXTI->PR & (1<<7))
	{
	EXTI->PR |=(0x1<<7);
	LED1_OFF;
	}

}

void EXTI15_10_IRQHandler(void)
{
 
	//执行PE14的中断清除标志
	if(EXTI->PR & (1<<14))
	{
	EXTI->PR |=(0x1<<14);
	LED2_OFF;
	}
	
	else if(EXTI->PR & (1<<15))
	{
	EXTI->PR |=(0x1<<15);
	LED3_OFF;
	LED4_OFF;
	}

}

