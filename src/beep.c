#include "beep.h"
#include "delay.h"
#include "my_delay.h"
#include "led_lib.h"

void beep_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;

	GPIO_Init(GPIOF, &GPIO_InitStruct);

}

void beep_ctrl(int status)
{
	status ? beep_on: beep_off;
}

void beep_police(void)
{
	u8 mode = 1;
	u32 new_compare = 300;
	while(1)
	{
		my_delay_ms(2);
		if(mode == 1)		
		{
			new_compare += 5;
			if(new_compare == 700)
				mode = 0;
		}
		else if(mode == 0)
		{
			new_compare -= 5;
			if(new_compare == 300)
				mode = 1;
		}
		TIM_SetCompare1(TIM13, new_compare);
	}

}
