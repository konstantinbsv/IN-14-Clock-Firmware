/*
 * rtc.h
 *
 *  Created on: Apr 8, 2020
 *      Author: Konstantin Borissov
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "main.h"
#include "stm32f1xx_hal_rtc_ex.h"
#include "stdbool.h"

/* Definitions */
#define RTC_BACKUP_REG 	0x00000001U //RTC_BKP_DR1
#define REG_CHECK_VAL 	0x32F2

/* Function Prototypes */
HAL_StatusTypeDef InitializeRTC (RTC_HandleTypeDef *_rtc_handle);
HAL_StatusTypeDef SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
HAL_StatusTypeDef SetDate(	uint8_t week_day, uint8_t month, uint8_t date, uint8_t year);
RTC_TimeTypeDef GetTime();
RTC_DateTypeDef GetDate();
bool IsBackupLost();
void SetAlarm();
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc);


#endif /* INC_RTC_H_ */
