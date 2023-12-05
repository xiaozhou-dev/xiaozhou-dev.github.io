#ifndef __BEEP_H__
#define __BEEP_H__
#include "stm32f4xx.h"

enum BEEP_STATUS
{
	BEEP_OFF,BEEP_ON
};

/*控制蜂鸣器响*/
#define beep_on GPIO_SetBits(GPIOF, GPIO_Pin_8)
#define beep_off GPIO_ResetBits(GPIOF, GPIO_Pin_8)
#define beep_tog GPIO_ToggleBits(GPIOF, GPIO_Pin_8)

void beep_init(void);
void beep_ctrl(int status);
void beep_police(void);


#endif
