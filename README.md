# DS1302 
 
####DS1302 慢速充电时钟线芯片包括实时时钟、日历和31字节的静态RAM。与时钟RAM通信只需要三根线，RST、IO、SCLK(串行时钟)。数据可以以每次一个字节或者多达31个字节的多字节形式传送至时钟RAM或者从其中送出来。
   
#### 注意：注意管脚配置冲突。

###实时时钟，可对秒、分、时、日、周、月以及自带的润年补偿的年进行计数。

### DS1302工作原理
	
	组成：移位寄存器、控制逻辑、振荡器、实时时钟和RAM。	
   
   为了初始化任何的数据的传输，把RST置为高电平且把提供地址和命令信息的8位装入移位寄存器，数据在SCLK的上升沿串行输入，无论是读周期还是写周期发生，也无论传送方式是单字节传送还是多字节传送，开始8位指定40个字节中的那个将被访问。

 

##### 写数据

			
![image](https://github.com/210843013/DS1302/blob/master/write.png)
		
		
	    
		void DS1302_Write(u8 cmd,u8 dat)
		{
			RST_out();
			CLK_out();
			RSTL;//初始CE线置为0
			SCLKL;//初始时钟线置为0
			RSTH;//初始CE置为1，传输开始
			Delay_us(4);
			DS1302_WriteByte(cmd);//传输命令字，要写入的时间/日历地址
			DS1302_WriteByte(dat);//写入要修改的时间/日期
			SCLKH;//时钟线拉高
			RSTL;//读取结束， CE置为0，结束数据的传输
		}
		
		
![image](https://github.com/210843013/DS1302/blob/master/writebyte.png)		


		void DS1302_WriteByte(u8 dat)
		{
			u8 i;
			SDA_out();
			CLK_out();
			SCLKL;
			Delay_us(1);
			for(i=0;i<8;i++)
			{	
			  if((dat&(0x01<<i))) //注意DS1302的数据和地址都是从最低位开始传输的
				{
				SDAH;
				}
				else
				{
				SDAL;
				}
				Delay_us(1);
				SCLKH;
				Delay_us(1);
				SCLKL;
			} 
		}
##### 读数据
  
![image](https://github.com/210843013/DS1302/blob/master/reade.png)

		u8 DS1302_Read(u8 cmd)
		{
			u8 dat;
			RST_out();
			CLK_out();
			RSTL;//初始CE线置为0
			SCLKL;//初始时钟线置为0
			RSTH;//初始CE置为1，传输开始
			DS1302_WriteByte(cmd);//传输命令字，要读取的时间/日历地址
			dat=DS1302_ReadByte();//读取要得到的时间/日期
			SCLKH;//时钟线拉高
			RSTL;//读取结束， CE置为0，结束数据的传输
			return dat;//返回得到的时间/日期
		}

![image](https://github.com/210843013/DS1302/blob/master/readebyte.png)

		u8 DS1302_ReadByte(void)
		{
			u8 i,dat=0;
			SDA_int();
			CLK_out();
			Delay_us(1);
			for(i=0;i<8;i++)
			{ 
				
				if((GPIOF->IDR&(1<<7)))
				{
				dat|=0x01<<i;
			    }
				else
			   {
				dat&=~(0x01<<i);
			   }
				SCLKH;
				Delay_us(1);
				SCLKL;
				Delay_us(1);
				}
			return dat;
		}
		
		
		
##### 时钟初始化

			#define DS1302_SECOND_WRITE  0x80          
			#define	DS1302_MINUTE_WRITE  0x82
			#define	DS1302_HOUR_WRITE    0x84
			#define	DS1302_DAY_WRITE     0x86
			#define	DS1302_MONTH_WRITE   0x88
			#define	DS1302_YEAR_WRITE    0x8C
			#define	DS1302_WEEK_WRITE    0X8A

			#define DS1302_SECOND_READ  0x81          
			#define	DS1302_MINUTE_READ   0x83
			#define	DS1302_HOUR_READ     0x85
			#define	DS1302_DAY_READ     0x87
			#define	DS1302_MONTH_READ    0x89
			#define	DS1302_YEAR_READ    0x8D
			#define	DS1302_WEEK_READ     0X8B
			
			
			
			void DS1302_init(void)
			{
				DS1302_Write(0x8e,0x00);//写保护关
				DS1302_Write(DS1302_SECOND_WRITE,0x00); 
				DS1302_Write(DS1302_MINUTE_WRITE,0x29);
				DS1302_Write(DS1302_HOUR_WRITE,0x11);
				DS1302_Write(DS1302_WEEK_WRITE,0x05);
				DS1302_Write(DS1302_DAY_WRITE,0x18); 
				DS1302_Write(DS1302_MONTH_WRITE,0x08);
				DS1302_Write(DS1302_YEAR_WRITE,0x17);
				DS1302_Write(0x90,0x01); //充电
				DS1302_Write(0xc0,0xf0); //初始化一次标示
				DS1302_Write(0x8e,0x80);
			}