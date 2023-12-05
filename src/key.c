#include "key.h"
#include "my_delay.h"
#include "led_lib.h"

void key_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,  ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitTypeDef GPIO_InitStruct2;

	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct1.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct2.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct1);
	GPIO_Init(GPIOE, &GPIO_InitStruct2);
	
}

int is_key_num(void)
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_0) == 0)
		{
			delay_xiaodou();
			if(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_0) == 0)
			{
				while(1)
				{
					if(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_0) == 1)
					{
						delay_xiaodou();
						if(GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_0) == 1)
							return 0;
					}
				}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_2) == 0)
		{
			delay_xiaodou();
			if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_2) == 0)
			{
				while(1)
				{
					if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_2) == 1)
					{
						delay_xiaodou();
						if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_2) == 1)
							return 1;
					}
				}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_3) == 0)
		{
			delay_xiaodou();
			if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_3) == 0)
			{
				while(1)
				{
					if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_3) == 1)
					{
						delay_xiaodou();
						if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_3) == 1)
							return 2;
					}
				}
			}
		}
		else if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_4) == 0)
		{
			delay_xiaodou();
			if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_4) == 0)
			{
				while(1)
				{
					if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_4) == 1)
					{
						delay_xiaodou();
						if(GPIO_ReadInputDataBit(GPIOE ,GPIO_Pin_4) == 1)
							return 3;
					}
				}
			}
		}

	}
}

void delay_xiaodou(void)
{
	int ms = 3; 
	while(ms--)
	{
		for(int i = 0;i < 6000;i++);
	}

}
