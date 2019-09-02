#pragma once
#ifndef __OBJECTS_H_
#define __OBJECTS_H_

#include "ili9341.h"

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
	STANDBY
} SYSTEM_MODE;

typedef enum
{ 
	BATTERY_VOLTAGE = 0,
	TEMPERATURE
} ADC_MODE;

typedef enum
{ 
	HSE = 0,
	HSI
} RCC_MODE;

typedef enum
{ 
	STATE_LOADED = 0,
	STATE_SLEEP,
	STATE_STANDBY,
	STATE_AWAKENED,
	STATE_PULSE_DETECT,
	STATE_FORCE_HV_PUMP,
	STATE_HV_TEST,
	STATE_PERIODIC_TASK
} LAST_STATE;

extern ILI9341 display;
extern volatile SYSTEM_MODE systemMode;
extern volatile RCC_MODE rccMode;

extern volatile bool isHVPumpActive;

extern volatile float battVoltage;
extern volatile float cpuTemp;

extern void processGeigerImpulse(void);
extern void processHVTestImpulse(void);
extern void processPeriodicTasks(void);
extern void readADCValue(void);

#endif //__OBJECTS_H_