#ifndef __DHT11_H__
#define __DHT11_H__
#include "stm32f4xx.h"

typedef struct
{
	int16_t temp_integer;
	int16_t temp_dec;
	int16_t humi;
	int8_t pos_neg;
}typedefDHT11_struct;

void DHT11_init_GetData(typedefDHT11_struct *DHT11_struct);
void DHT11_out(void);
void DHT11_in(void);
uint8_t DHT11_respond(void);
uint8_t DHT11_read_bit(void);
uint8_t DHT11_read_byte(void);





#endif






