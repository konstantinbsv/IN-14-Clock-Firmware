/*
 * rtc.h
 *
 *  Created on: Apr 8, 2020
 *      Author: Konstantin Borissov
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"


/* Function Prototypes */
HAL_StatusTypeDef InitializeRTC (RTC_HandleTypeDef *_rtc_handle);
HAL_StatusTypeDef SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
HAL_StatusTypeDef SetDate(	uint8_t week_day, uint8_t month, uint8_t date, uint8_t year);

#endif /* INC_RTC_H_ */
