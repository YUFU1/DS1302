#ifndef   LCD_H
#define   LCD_H

#include "stm32f4xx.h" 


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//构造图片信息结构体
struct PIC
{
	u16 x;//起始横坐标
	u16 y;//起始纵坐标
	u16 width ; //图片实际宽度
	u16 high ; //图片实际高度
	const u8 *buf ;//图片颜色数组名
};

extern u8 hz_index[];
extern u8 hz_table[];
extern u8 ch_index[];
extern u8 ch_table[];

extern const unsigned char gImage_pic1[307200];
extern const unsigned char gImage_pic2[307208];
//画点函数
void LCD_Draw_Point(u16 x ,u16 y ,u16 color);


void LCD_Init(void);
void LCD_Clear(u16 SX, u16 EX, u16 SY, u16 EY, u16 Color);
void LCD_ILI9486_Parameter(u16 data);
void LCD_ILI9486_CMD(u16 common);
void LCD_Port_Init(void);

void LCD_Draw(u16 x_start ,u16 y_start,u16 x_end ,u16 y_end ,u16 color);


//汉字显示相关函数
void LCD_Dis16hz(u16 x,u16 y,u16 color,u16 backcolor,u8 *buf);

void LCD_Disanyhz(u16 x,u16 y,u16 width,u16 high,u16 color,u16 backcolor,u8 *buf);
void LCD_Dis32hz(u16 x,u16 y,u16 color, const u8 *pic,u8 *buf);

//字符显示相关函数
void LCD_Dis32ch(u16 x,u16 y,u16 color,const u8 *pic,u8 *buf);
void LCD_Diszf(u16 x,u16 y,u16 color,	u16 backcolor,u8 *buf);
void LCD_Dishz(u16 x,u16 y,u16 color,	u16 backcolor,u8 *buf);

//字符串显示相关函数
void LCD_DisString(u16 x,u16 y ,u16 color,const u8 *pic, u8 *buf);

void LCD_ShowString(u16 x,u16 y ,u16 color, u16 back, u8 *buf);

//图片显示相关函数
void LCD_Dispic(u16 x,u16 y,u16 width,u16 high ,const u8 *buf);
void LCD_Dispic1(struct PIC pic);
void LCD_Dispic2(u16 x,u16 y,const u8 *buf);
#endif
