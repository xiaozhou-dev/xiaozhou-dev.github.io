因为小车有很多模块嘛,所以我就细说两个模块的使用,虽然我知道没人会看我的博客,不过无所谓.

DHT11温湿度传感器:
  这作为一个很入门的模块,我觉得一开始来说读懂数据手册是很重要的,正因为它很容易,所以就要好好读一下.
  时序图是最重要的:
  ![1701776756500](https://github.com/xiaozhou-dev/xiaozhou_mailuncar.github.io/assets/73087310/a5a96461-01e4-4cd6-8429-f550222beb56)
  这段时序图画出来了主机该做什么,从机该做什么,但是没有标明时间关系,所以你只知道拉高拉低电平,但是不知道持续多长时间.
  ![image](https://github.com/xiaozhou-dev/xiaozhou_mailuncar.github.io/assets/73087310/03ee907e-3481-458d-899d-a4a444421676)
  这里就写出来了主机至少拉低18us,然后拉高20us-40us表示主机给dht11发送了复位信号,然后呢,你并不知道从机(dht11)给我发送的是什么信号,是0还是1
  ![image](https://github.com/xiaozhou-dev/xiaozhou_mailuncar.github.io/assets/73087310/24cec0a9-da21-4cc4-9311-45ea3c4680ce)
  从这张图可以看出来,从机都先是50us,然后是高电平70us表示逻辑1,26-28us表示逻辑0,这样我们就知道了从机给我们发的是0还是1.

代码如下 : 
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
	for(i = 0;i < 5;i++){
		data[i] = DHT11_read_byte();
		sum += data[i];
	}
	if(sum - data[4] == data[4]){
		DHT11_struct->pos_neg = 1;
		DHT11_struct->humi = data[0];
		DHT11_struct->temp_integer = data[2];
		DHT11_struct->temp_dec = data[3];
	}
}
void DHT11_out(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOG, &GPIO_InitStruct);
}
void DHT11_in(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOG, &GPIO_InitStruct);    
}
//返回值1(从机无回应),0是有回应
uint8_t DHT11_respond(void){
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
uint8_t DHT11_read_bit(void){
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
uint8_t DHT11_read_byte(void){
	uint8_t i,data = 0x00;
	for(i = 0;i < 8;i++)
	{
		data <<= 1;
		data |= DHT11_read_bit();
	}
	return data;
}
