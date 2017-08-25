	#include "includes.h"
	/******************************************************
	* Name： LED_init
	* Function：  实现开发板对应的LED灯的输出功能配置
	* Description： PF6 PF9 PF10 PC0 配置成推挽输出
	* Author：  sun
	******************************************************/
	void LED_Init()
	{
	 //打开PF对应的外设时钟
		
		RCC->AHB1ENR |=(0x1<<5);
	 
		/*****配置PF6管脚的输出功能**************************************/
	 
		//输出模式寄存器
		
		GPIOF->MODER &=~(0x3<<12); //清零
		GPIOF->MODER |=(0x1<<12); //通用输出模式
		
		//输出类型寄存器
		GPIOF->OTYPER &=~(0x1<<6); //设置为推挽输出
		
		//输出速率寄存器
		
		GPIOF->OSPEEDR &=~(0x3<<12);  //清零
		GPIOF->OSPEEDR |=(0x1<<12);   //配置成输出速率50M
		
		/*****配置PF9管脚的输出功能************************************/
	 
		//输出模式寄存器
		
		GPIOF->MODER &=~(0x3<<18); //清零
		GPIOF->MODER |=(0x1<<18); //通用输出模式
		
		//输出类型寄存器
		GPIOF->OTYPER &=~(0x1<<9); //设置为推挽输出
		
		//输出速率寄存器
		
		GPIOF->OSPEEDR &=~(0x3<<18);  //清零
		GPIOF->OSPEEDR |=(0x2<<18);   //配置成输出速率50M
		
			/*****配置PF10管脚的输出功能********************************/
	 
		//输出模式寄存器
		
		GPIOF->MODER &=~(0x3<<20); //清零
		GPIOF->MODER |=(0x1<<20); //通用输出模式
		
		//输出类型寄存器
		GPIOF->OTYPER &=~(0x1<<10); //设置为推挽输出
		
		//输出速率寄存器
		
		GPIOF->OSPEEDR &=~(0x3<<20);  //清零
		GPIOF->OSPEEDR |=(0x2<<20);   //配置成输出速率50M
		
		//打开PC对应的外设时钟
		
		RCC->AHB1ENR |=(0x1<<2);
	 
		/*****配置PC0管脚的输出功能***********************************/
	 
		//输出模式寄存器
		
		GPIOC->MODER &=~(0x3<<0); //清零
		GPIOC->MODER |=(0x1<<0); //通用输出模式
		
		//输出类型寄存器
		GPIOC->OTYPER &=~(0x1<<0); //设置为推挽输出
		
		//输出速率寄存器
		
		GPIOC->OSPEEDR &=~(0x3<<0);  //清零
		GPIOC->OSPEEDR |=(0x2<<0);   //配置成输出速率50M
		
	}

	/******************************************************
	* Name： LED_ALLON
	* Function：  实现开发板对应的LED灯全亮
	* Description： 输出低电平
	* Author：  sun
	******************************************************/

	void LED_ALLON()
	{

		LED1_ON;
		LED2_ON;
		LED3_ON;
		LED4_ON;
		

		
	}
	/******************************************************
	* Name： LED_ALLON
	* Function：  实现开发板对应的LED灯全灭
	* Description： 输出低电平
	* Author：  sun
	******************************************************/

	void LED_ALLOFF()
	{

		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;

		
	}
	/******************************************************
	* Name： LED_Water
	* Function：  实现开发板对应的LED灯挨个亮
	* Description： 输出低电平
	* Author：  sun
	******************************************************/

	void LED_Water()
	{
	 while(1)
	 {
		LED1_ON;
				delay(5000000);
		LED1_OFF;
				delay(5000000);
		LED2_ON;
				delay(5000000);
		LED2_OFF;
				delay(5000000);
		LED3_ON;
				delay(5000000);
		LED3_OFF;
				delay(5000000);
		LED4_ON;
				delay(5000000);
		LED4_OFF;
				delay(5000000);
//			if(KEY1_ON ==1 ||KEY2_ON==0||KEY3_ON==0||KEY4_ON==0)
//		 {
//			 break;
//		 }
	 }
		
	}

	/******************************************************
	* Name： LED_Twinkle
	* Function：  实现开发板对应的LED灯闪烁
	* Description： 输出低电平
	* Author：  sun
	******************************************************/

	void LED_Twinkle()
	{
			while(1)
			{
			LED_ALLON();
			delay(5000000);
			LED_ALLOFF();
			delay(5000000);
//				 if(KEY1_ON ==1 ||KEY2_ON==0||KEY3_ON==0||KEY4_ON==0)
//		 {
//			 break;
//		 }
			}
	}
	/******************************************************
	* Name： LED_HX
	* Function：  实现开发板对应的LED呼吸灯
	* Description： 实现原理：开始由亮灭亮灭组合 逐渐加强延时亮 达到最暗到最亮的过程
	*	达到最亮以后，有最亮逐渐变暗，其原理是亮的延时逐渐减小，灭的延时加强。
	* Author：  sun
	******************************************************/
	void LED_HX()
	{
		int i,j;
		for(i=0;i<1000;i++)
		{
			// 最暗到最亮
			for(j=0;j<8000;j++)
			{
					LED_ALLON(); // 逐渐亮
				delay(j);
				
					LED_ALLOFF(); //暗的延时减少
				delay(8000-j);
			}
		
					LED_ALLON(); //达到最亮
				delay(8000);
			// 最亮到最暗
			for(j=0;j<8000;j++)
			{
					LED_ALLON(); // 亮延时减小
				delay(8000-j);
				
				LED_ALLOFF();  //逐渐暗
				delay(j);
			}
				LED_ALLOFF(); //达到最暗
				delay(8000);
		 }
	}
