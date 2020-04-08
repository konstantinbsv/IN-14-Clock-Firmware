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

HAL_StatusTypeDef outputToDisplay(uint8_t number, bool top_separator_on, bool bottom_separator_on) {
	assert_param(number >=0 || number <= 999999); // from 00 00 00 to 99 99 99
	// assert_param(second_pair >=0 || second_pair <= 10);
	// assert_param(third_pair >=0 || third_pair <= 10);

	uint64_t output_uint = 0b0;
	for (uint8_t i = 0; i < DIGIT_OUTPUTS; i++) {
		uint8_t digit = number % 10; 				// extract least significant digit from input number
		output_uint |= 1 << DIGIT_INDEX[i][digit];	// OR with 64bit output uint
		number /= 10;								// remove least significant digit from input number
	}

	uint8_t output_array[LATCH_BYTES];
	for (uint8_t i = 0; i < LATCH_BYTES; i++) {
			output_array[i] = output_uint | 0xFF;	// reverses and separates uint into an array for SPI transmission
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



void loadLatches() {
	HAL_GPIO_WritePin(LATCH_ENABLE_GPIO_Port, LATCH_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LATCH_ENABLE_GPIO_Port, LATCH_ENABLE_Pin, GPIO_PIN_RESET);
}