/*
 * rtc.c
 *
 *  Created on: Apr 8, 2020
 *      Author: Konstantin Borissov
 */


#include "rtc.h"

static RTC_HandleTypeDef *rtc_handle = NULL;

HAL_StatusTypeDef InitializeRTC (RTC_HandleTypeDef *_rtc_handle) {
	rtc_handle = _rtc_handle;

	return rtc_handle != NULL ? HAL_OK : HAL_ERROR;
}

