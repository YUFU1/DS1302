#ifndef KEY_H
#define KEY_H

#define KEY1_ON   ((GPIOA->IDR & (0x1<<0)))
#define KEY1_OFF  ((GPIOA->IDR & (0x1<<0)))

#define KEY2_ON   ((GPIOE->IDR & (0x1<<2)))
#define KEY2_OFF  ((GPIOE->IDR & (0x1<<2)))

#define KEY3_ON   ((GPIOE->IDR & (0x1<<3)))
#define KEY3_OFF  ((GPIOE->IDR & (0x1<<3)))

#define KEY4_ON   ((GPIOE->IDR & (0x1<<4)))
#define KEY4_OFF  ((GPIOE->IDR & (0x1<<4)))

void KEY_Init(void);

int Get_Value(void);

void Key_EXTI_Init(void);

void WJ_Exit_Init(void);

#endif

