#ifndef AD9833_H_
#define AD9833_H_

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define FMCLK 25000000 // Master clock frequency
#define _2_POW_28 268435456
#define AD9833_PORT GPIOA
#define AD9833_FSYNC GPIO_PIN_4
#define AD9833_SCLK GPIO_PIN_5
#define AD9833_SDATA GPIO_PIN_7

enum wave_type
{
    SINUS,
    TRIANGLE,
    SQUARE
};

void write_to_spi(uint16_t word);
void AD9833_SetWave(uint16_t wave_type);
void AD9833_SetWaveParams(float frequency, float phase);
void AD9833_Init(uint16_t wave_type, float frequency, float phase);

#endif 
