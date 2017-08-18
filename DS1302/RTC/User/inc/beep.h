#ifndef BEEP_H
#define BEEP_H
//PF8
#define  BEEP_ON  GPIOF->ODR |=(0x1<<8)
#define  BEEP_OFF GPIOF->ODR &=~(0x1<<8)
//  Ä£Äâ¼üÅÌÊäÈëPE4
#define  BEEP_KEYON   (GPIOF->IDR & (0x01<<4))


void BEEP_Init(void);

void BEEP_Water(void);

void BEEP_KeyInit(void);

#endif

