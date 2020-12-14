#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define adc_vbg 0xE
void ADC_Init(void);
void ADC_enable(void);
void ADC_disable(void);
uint16_t ADC_Read( uint8_t);
uint16_t ADC_Read_Avg( uint8_t, uint8_t);

#endif
