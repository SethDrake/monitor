
#pragma once
#ifndef __CO2_SENSOR_H_
#define __CO2_SENSOR_H_

#include "stm32f10x.h"
#include <src/radiation.h>

class CO2Sensor {
public:
	CO2Sensor();
	~CO2Sensor();
	void setupHw(USART_TypeDef* usart);
	void setTime(time_s* time);
	uint16_t GetCurrentCO2Level(void);
	uint16_t GetErrorsCount(void);
	void ResetErrorsCount(void);
	uint16_t* GetPerSecondCo2();
	uint16_t* GetPerMinuteCo2();
	uint16_t* GetPerHourCo2();
	void TickOneSecond();
protected:
	void USART_Send(uint8_t data);
	void USART_SendBlock(uint8_t* data, uint8_t size);
	uint8_t USART_Read(bool* isError);
	bool USART_ReadBlock(uint8_t* data, uint8_t size);
	uint8_t calculateChecksum(uint8_t* command);
	void measureCO2Level(void);
private:
	USART_TypeDef* usart;
	time_s* time;
	uint16_t errorsCount;
	uint16_t counts[14];
	uint16_t co2PerMinutes[62];
	uint16_t co2PerHours[26];
	uint16_t maxLevel;
	uint16_t maxLevelToday;
	uint32_t sumForInterval;
};

#endif
