#include "AD9833.h"

uint16_t low_frq_bits = 0;
uint16_t high_frq_bits = 0;
uint32_t curr_phase = 0;

extern SPI_HandleTypeDef hspi1;

void AD9833_WriteRegister(uint16_t word)
{
    HAL_GPIO_WritePin(AD9833_PORT, AD9833_FSYNC, GPIO_PIN_RESET);
	__NOP(); __NOP(); // Slight delay
    HAL_SPI_Transmit(&hspi1, (uint8_t*)&word, 1, HAL_MAX_DELAY);
	__NOP(); __NOP();
    HAL_GPIO_WritePin(AD9833_PORT, AD9833_FSYNC, GPIO_PIN_SET);
}

void AD9833_SetWave(uint16_t wave_type)
{
	switch(wave_type)
	{
	case 0:
		AD9833_WriteRegister(0x2000); // Sine wave
		break;
	case 1:
		AD9833_WriteRegister(0x2002); // Triangle wave
		break;
	case 2:
		AD9833_WriteRegister(0x2028); // Square wave
		break;
	}
}

void AD9833_SetWaveParams(float frequency, float phase)
{
	if (phase < 0.0) phase = 0.0;
	if (phase > 360.0) phase = 360.0;

	curr_phase = (int)(phase * (4096.0f/360.0f));
	curr_phase |= 0xc000;

	uint32_t freq_reg;
	freq_reg = frequency * _2_POW_28 / FMCLK;

	low_frq_bits = freq_reg & 0x3fff;
	high_frq_bits = (freq_reg >> 14) & 0x3fff;

	low_frq_bits |= 0x4000;
	high_frq_bits |= 0x4000;

	AD9833_WriteRegister(0x2100);
	AD9833_WriteRegister(low_frq_bits);
	AD9833_WriteRegister(high_frq_bits);
	AD9833_WriteRegister(curr_phase);
	AD9833_WriteRegister(0x2000);

	AD9833_SetWave(curr_wave);
}

void AD9833_Init(enum wave_type wave, float frequency, float phase)
{
	HAL_GPIO_WritePin(AD9833_PORT, AD9833_FSYNC, GPIO_PIN_SET);
	AD9833_SetWaveParams(frequency, phase);
	AD9833_SetWave(wave);
}
