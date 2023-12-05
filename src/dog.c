#include "dog.h"

void IWDG_init(void)
{
	//使能IWDG的读写功能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//设置IWDG分频系数
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//设置IWDOG装载值
	IWDG_SetReload(1000);	//1s
	//使能IWDOG
    IWDG_Enable();
}

//重新加载计算值"喂狗"
void iwdog_feed(void)
{
    IWDG_ReloadCounter();
}

