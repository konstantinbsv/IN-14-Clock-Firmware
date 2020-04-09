/*
 * shift_register.c
 *
 *  Created on: Apr. 4, 2020
 *      Author: Konstantin Borissov
 */

#include "shift_register.h"

static SPI_HandleTypeDef *spi_handle = NULL;

/**
 * @brief Initialize SPI handle
 *
 * @return HAL_OK if handle not null
 */
HAL_StatusTypeDef InitializeSPI (SPI_HandleTypeDef *_spi_handle) {
	spi_handle = _spi_handle;

	return spi_handle != NULL ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef outputToDisplay (	uint32_t number,
									bool left_dp_on,
									bool right_dp_on,
									bool top_sepr_on,
									bool btm_sepr_on)
{
	assert_param(number >=0 && number <= 999999); // from 00 00 00 to 99 99 99

	uint64_t output_uint = 0b0;
	for (uint8_t i = 0; i < DIGIT_OUTPUTS; i++) {
		uint8_t digit = number % 10; 							// extract least significant digit from input number
		output_uint |= (uint64_t) 1 << DIGIT_INDEX[i][digit];	// OR with 64bit output uint
		number /= 10;											// remove least significant digit from input number
	}

	output_uint |= (uint64_t)left_dp_on  	<< SEPARATOR_INDEX[LEFT_DP];
	output_uint |= right_dp_on 	<< SEPARATOR_INDEX[RIGHT_DP];
	output_uint |= top_sepr_on  << SEPARATOR_INDEX[TOP_SEPR];
	output_uint |= btm_sepr_on  << SEPARATOR_INDEX[BTM_SEPR];

	uint8_t output_array[LATCH_BYTES] = {0};
	for (uint8_t i = 0; i < LATCH_BYTES; i++) {
			output_array[LATCH_BYTES - i - 1] = output_uint & 0xFF;	// reverses and separates uint into an array for SPI transmission
			output_uint /= (0xFF + 1);				// remove last 8 bits from output number
	}

	return writeToLatch(output_array);
}

HAL_StatusTypeDef writeToLatch(uint8_t data[]) {
	// all outputs must be written to at once
	assert_param(sizeof(data)/sizeof(data[0]) == LATCH_BYTES);

	return HAL_SPI_Transmit(spi_handle, data, LATCH_BYTES, SPI_TIMEOUT);
}

HAL_StatusTypeDef clearShiftRegisters() {
	uint8_t zero_bits[8] = {0x00};

	return writeToLatch(zero_bits);
}

void cathodePoisoningPrevention(uint8_t cycles) {
	bool dp_on = false;
	bool sepr_on = false;
	for (uint32_t i = 0; i < cycles; i++) {
		if (i % 5 == 0) dp_on = !dp_on;
		if (i % 5 == 0) sepr_on = !sepr_on;

		outputToDisplay(111111 * (i % 10), dp_on, dp_on, sepr_on, !sepr_on);
		HAL_Delay(100);
	}
}

void loadLatches() {
	HAL_GPIO_WritePin(LATCH_ENABLE_GPIO_Port, LATCH_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LATCH_ENABLE_GPIO_Port, LATCH_ENABLE_Pin, GPIO_PIN_RESET);
}
