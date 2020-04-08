/*
 * shift_register.h
 *
 *  Created on: Apr. 4, 2020
 *      Author: Konstantin Borissov
 */

#ifndef INC_SHIFT_REGISTER_H_
#define INC_SHIFT_REGISTER_H_

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_gpio.h"
//#include "stm32f1xx_hal_spi.h"
#include "main.h"
#include "stdbool.h"

/* Definitions */
#define SPI_TIMEOUT 	1000				// milliseconds before SPI transmit times out
#define LATCH_OUTPUTS 	64 					// total number of latch outputs
#define LATCH_BYTES 	(LATCH_OUTPUTS / 8)	// number of bytes to completely define all latch output
#define DIGIT_OUTPUTS 	6  					// six IN-14 tubes each displaying one digit at a time
#define SEPR_OUTPUTS	4					// number of separator outputs

							   	   	   	   	  // 0    1   2   3   4   5   6   7   8   9
const static uint8_t DIGIT_INDEX[6][10] = { 	{1,  10,  9,  8,  7,  6,  5,  4,  3,  2}, 	// ones
												{11, 20, 19, 18, 17, 16, 15, 14, 13, 12}, 	// tens
												{23, 32, 31, 30, 29, 28, 27, 26, 25, 24},	// hundreds
												{33, 42, 41, 40, 39, 38, 37, 36, 35, 34},	// one_thousands
												{43, 51, 51, 50, 49, 48, 47, 46, 45, 44},	// hundred_thousands
												{53, 62, 61, 60, 59, 58, 57, 56, 55, 54}	// one_millions
											};

const static uint8_t SEPARATOR_INDEX[4] = {63, 0, 21, 22}; // left decimal point, right decimal point, top separator, bottom separator

enum {
	LEFT_DP 	= 0,
	RIGHT_DP 	= 1,
	TOP_SEPR 	= 2,
	BTM_SEPR 	= 3
};

/* Function Prototypes */
HAL_StatusTypeDef InitializeSPI (SPI_HandleTypeDef *_spi_handle);
HAL_StatusTypeDef outputToDisplay(	uint32_t number, bool left_dp_on, bool right_dp_on, bool top_sepr_on, bool bottom_sepr_on);
HAL_StatusTypeDef writeToLatch(uint8_t data[]);
HAL_StatusTypeDef clearShiftRegisters();
void loadLatches();

#endif /* INC_SHIFT_REGISTER_H_ */
