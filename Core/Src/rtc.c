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

/**
 * Set the time
 */
HAL_StatusTypeDef SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
	assert_param(hours <= 23U);
	assert_param(hours <= 59U);
	assert_param(seconds <= 59U);

	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;

	return HAL_RTC_SetTime(rtc_handle, &sTime, RTC_FORMAT_BCD);
}

/**
 * Set the date
 */
HAL_StatusTypeDef SetDate(	uint8_t week_day,
							uint8_t month,
							uint8_t date,
							uint8_t year)
{
	assert_param(week_day >= RTC_WEEKDAY_SUNDAY && week_day <= RTC_WEEKDAY_SATURDAY);
	assert_param(month >= RTC_MONTH_JANUARY && month <= RTC_MONTH_DECEMBER);
	assert_param(date >= 1 && date <= 31);
	assert_param(year <= 99);

	RTC_DateTypeDef DateToUpdate = {0};

	DateToUpdate.WeekDay = week_day;
	DateToUpdate.Month = month;
	DateToUpdate.Date = date;
	DateToUpdate.Year = year;

	return HAL_RTC_SetDate(rtc_handle, &DateToUpdate, RTC_FORMAT_BCD);

}
