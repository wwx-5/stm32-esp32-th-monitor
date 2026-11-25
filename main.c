#include "stm32f10x.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "dht11.h"
#include "oled.h"
#include "usart.h"

/*****************乘风单片机工作室******************
  项目		:	STM32-DHT11传感器
  版本		: 	V0.0
  MCU		:	STM32F103C8T6
  哔哩哔哩	:	乘风单片机工作室
  CSDN		:	乘风单片机工作室
  单片机定制+V：cfdz788 QQ：3883997176  组团优惠多多
****************************************************/




uint8_t temp,humi;
char str[32];
void Serial_SendString(char *Str)
{
    while (*Str ){
		USART_SendData (USART1 ,*Str++);
	  while (USART_GetFlagStatus (USART1 ,USART_FLAG_TXE )==RESET);
		}
}
int main(void)
{ 
	//配置系统时钟为72M
	SystemInit();	
	delay_init(72);
	LED_Init();
	LED_On();

	USART1_Config();//串口初始化
	OLED_Init();
	DHT11_Init();
	delay_ms(1000);
	
	USART1_Config();
	
	//显示“温度：”
	OLED_ShowChinese(1,1,0);
	OLED_ShowChinese(1,2,1);
	OLED_ShowChar(1,5, ':');
	OLED_ShowChar(1,9, 'C');	
	//显示“湿度：”
	OLED_ShowChinese(2,1, 2);
	OLED_ShowChinese(2,2, 1);
	OLED_ShowChar(2, 5, ':');	
	OLED_ShowChar(2, 9, '%');
	
  while (1)
  {
		DHT11_Read_Data(&temp,&humi);//数据获取
		printf("temp %d ,humi %d\r\n",temp,humi);
		LED_Toggle();
		delay_ms(1000);

		OLED_ShowNum(1,6,temp,2);	//显示温度数据

		OLED_ShowNum(2,6,humi,2);	//显示湿度数据s
		sprintf  (str,"temp %d,humi %d\r\n",temp ,humi);
	 Serial_SendString (str );
		delay_ms (1000); 
  }
}


