#pragma once
#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include "ili9341.h"
#include "settings.h"

#define JD0 2451911 // days 01 Jan 2001

typedef struct
{
	uint16_t year;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
} time_s;

typedef struct
{
	int8_t days;
	int8_t hour;
	int8_t minute;
	int8_t second;
} short_time_s;

template<class ForwardIt>
	ForwardIt max_element(ForwardIt first, ForwardIt last)
	{
		if (first == last) {
			return last;
		}
		ForwardIt largest = first;
		++first;
		for (; first != last; ++first) {
			if (*largest < *first) {
				largest = first;
			}
		}
		return largest;
	}


typedef enum
{ 
	LOADING = 0,
	ACTIVE,
	SLEEP,
	SHUTDOWN
} SYSTEM_MODE;

typedef enum
{ 
	VREFINT_VOLTAGE = 0,
	BATTERY_VOLTAGE = 1,
	TEMPERATURE
} ADC_MODE;

typedef enum
{ 
	HSE = 0,
	HSI
} RCC_MODE;


extern ILI9341 display;
extern SettingsManager settingsManager; 
extern volatile SYSTEM_MODE systemMode;
extern volatile RCC_MODE rccMode;

extern volatile bool isHVPumpActive;

extern volatile float battVoltage;
extern volatile float cpuTemp;

extern void processGeigerImpulse(void);
extern void processHVTestImpulse(void);
extern void processPeriodicTasks(void);
extern void readADCValue(void);
extern time_s timeFromSeconds(uint32_t val);
extern short_time_s shortTimeFromSeconds(uint32_t val);

#endif //__OBJECTS_H_