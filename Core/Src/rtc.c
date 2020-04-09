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

	rtc_handle->Instance = RTC;
	rtc_handle->Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	rtc_handle->Init.OutPut = RTC_OUTPUTSOURCE_ALARM;

	HAL_StatusTypeDef init_status = HAL_RTC_Init(rtc_handle); // initialize RTC, must be done before setting interrupt

	HAL_RTCEx_SetSecond_IT(rtc_handle);	 // set first bit in RTC's CRH register to enable 1Hz RTC interrupts

	return init_status;
}

/**
 * Set the time
 */
HAL_StatusTypeDef SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
	assert_param(hours <= 23U);
	assert_param(minutes <= 59U);
	assert_param(seconds <= 59U);

	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = hours;
	sTime.Minutes = minutes;
	sTime.Seconds = seconds;

	HAL_RTCEx_BKUPWrite(rtc_handle, RTC_BACKUP_REG, REG_CHECK_VAL);

	return HAL_RTC_SetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);
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

	return HAL_RTC_SetDate(rtc_handle, &DateToUpdate, RTC_FORMAT_BIN);

}

RTC_TimeTypeDef GetTime() {
	RTC_TimeTypeDef sTime;
	HAL_RTC_GetTime(rtc_handle, &sTime, RTC_FORMAT_BIN);

	return sTime;
}

RTC_DateTypeDef GetDate() {
	RTC_DateTypeDef sDate;
	HAL_RTC_GetDate(rtc_handle, &sDate, RTC_FORMAT_BIN);

	return sDate;
}

bool IsBackupLost(){
	return (HAL_RTCEx_BKUPRead(rtc_handle, RTC_BACKUP_REG) != REG_CHECK_VAL);
}

void SetAlarm() {
	RTC_AlarmTypeDef sAlarm = {0};

	sAlarm.Alarm = RTC_ALARM_A;
	HAL_RTC_SetAlarm_IT(rtc_handle, &sAlarm, RTC_FORMAT_BCD);
}
