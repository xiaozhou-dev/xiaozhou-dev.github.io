#include "DHT11.h"
#include "delay.h"
#include <stdio.h>

void DHT11_init_GetData(typedefDHT11_struct *DHT11_struct)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE);

	//1. 复位
	DHT11_in();			//DHT11输入模式	GPIOG 9口输出
	GPIO_ResetBits( GPIOG, GPIO_Pin_9);
	delay_ms(20);		//输出20ms低电平
	GPIO_SetBits( GPIOG, GPIO_Pin_9);
	delay_us(13);		//输出30us高电平

	//2. 等待DHT11回应
	DHT11_out();		//DHT11输出模式 GPIOG 9口输入
	if(DHT11_respond())
	{
		printf("DHT11 missing or error\n");
	}

	//3. 读取数据
	uint8_t i,data[5] = {0},sum = 0;
	for(i = 0;i < 5;i++)
	{
		data[i] = DHT11_read_byte();
		sum += data[i];
	}
	
	if(sum - data[4] == data[4])
	{
		DHT11_struct->pos_neg = 1;
		DHT11_struct->humi = data[0];
		DHT11_struct->temp_integer = data[2];
		DHT11_struct->temp_dec = data[3];
//		if((data[3] & 0x01) == 1)
//		{
//			DHT11_struct->pos_neg = -1;
//			DHT11_struct->temp_integer *= -1;
//		}
	}
}


void DHT11_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOG, &GPIO_InitStruct);
}

void DHT11_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOG, &GPIO_InitStruct);
}

//返回值1(从机无回应),0是有回应
uint8_t DHT11_respond(void)
{
	uint8_t count= 0;
	//检测从机有无拉低
	while(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_9) && count < 100)
	{
		count++;
		delay_us(1);
	}
	if(count >= 100)
		return 1;
	else 
		count = 0;
	//检测从机有无升高
	while(!GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_9) && count < 100)
	{
		count++;
		delay_us(1);
	}
	if(count >= 100)
		return 1;
	else 
		return 0;	
}

uint8_t DHT11_read_bit(void)
{
	uint8_t count = 0;
	while(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_9) && count < 100)
	{
		count++;
		delay_us(1);
	}
	if(count >= 100)
		printf("count overtime\n");
	
	count = 0;
	while(!GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_9) && count < 100)
	{
		count++;
		delay_us(1);
	}

//	delay_us(54);//等待低电平过去
	delay_us(50);
	return GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_9);
}

//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
uint8_t DHT11_read_byte(void)
{
	uint8_t i,data = 0x00;
	for(i = 0;i < 8;i++)
	{
		data <<= 1;
		data |= DHT11_read_bit();
	}
	return data;
}
