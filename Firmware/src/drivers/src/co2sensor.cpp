
#include <algorithm>
#include <stm32f10x_rcc.h>
#include "co2sensor.h"

/* MH-Z19 CO2 SENSOR */

CO2Sensor::CO2Sensor() {
	maxLevel = 0;
	maxLevelToday = 0;
	sumForInterval = 0;
	errorsCount = 0;
	std::fill_n(counts, 240, 0);
	std::fill_n(co2PerMinutes, 62, 0);
	std::fill_n(co2PerHours, 26, 0);
}

CO2Sensor::~CO2Sensor() {
}

void CO2Sensor::setupHw(USART_TypeDef* usart)
{
	this->usart = usart;
}

void CO2Sensor::USART_Send(uint8_t data) 
{
	while (!(USART1->SR & USART_SR_TC));
	USART1->DR = data; 
}

void CO2Sensor::USART_SendBlock(uint8_t* data, uint8_t size) 
{
	for (int i = 0; i < size; i++)
	{
		USART_Send(data[i]);
	} 
}

uint8_t CO2Sensor::USART_Read(bool* isOk) 
{
	volatile uint32_t nCount;
	RCC_ClocksTypeDef RCC_Clocks;
	
	*isOk = true;
	RCC_GetClocksFreq(&RCC_Clocks);
	nCount = (RCC_Clocks.HCLK_Frequency / 10000) * 100; //100 ms

	while (!(USART1->SR & USART_SR_RXNE))
	{
		nCount--;
		if (nCount == 0) //break if await longer 0.1 s
		{
			*isOk = false;
			break;
		}
	}
	return USART1->DR;
}

bool CO2Sensor::USART_ReadBlock(uint8_t* data, uint8_t size) 
{
	bool isOk = true;
	for (int i = 0; i < size; i++)
	{
		data[i] = USART_Read(&isOk);
		if (!isOk)
		{
			return false;
		}
	}
	return true;
}

uint8_t CO2Sensor::calculateChecksum(uint8_t* command)
{
	uint8_t checksum = 0;
	for (int i = 1; i < 8; i++)
	{
		checksum += command[i];
	}
	checksum = checksum ^ 0xFF;
	checksum += 1;
	return checksum;
}


void CO2Sensor::measureCO2Level()
{
	uint8_t cmd[9] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
	
	USART_SendBlock(cmd, 9);
	if (!USART_ReadBlock(cmd, 9))
	{
		counts[0] = 0;
		errorsCount++;
		return;
	}
	
	uint8_t checksum = calculateChecksum(cmd);
	uint8_t checksumFromSensor = cmd[8];
	if (checksum != checksumFromSensor) { //checksum incorrect
		counts[0] = 0;
		errorsCount++;
		return;
	}
	counts[0] = (cmd[2] << 8) + cmd[3];
	if (counts[0] > 34000) // invalid value
	{
		counts[0] = 0;
	}
}

uint16_t CO2Sensor::GetCurrentCO2Level(void) 
{
	return counts[1];
}

void CO2Sensor::setTime(time_s* time)
{
	this->time = time;
}

void CO2Sensor::TickOneSecond()
{
	if (time->second % 10 == 0)
	{
		this->measureCO2Level();
		for (uint8_t i = 5; i > 0; i--)
		{
			sumForInterval += counts[i];
		}
		for (uint8_t i = 13; i > 0; i--)
		{
			counts[i] = counts[i - 1];
		}
		counts[0] = 0;
	}
	
	if (time->second == 60) // one minute elapsed
	{
		co2PerMinutes[time->minute] = sumForInterval / 6;
		sumForInterval = 0;
		if (maxLevelToday < co2PerMinutes[time->minute]) {
			maxLevelToday = co2PerMinutes[time->minute];
		}
		if (maxLevel < maxLevelToday)
		{
			maxLevel = maxLevelToday;
		}
	}
	
	if (time->minute == 60) // one hour elapsed
	{
		for (uint8_t i = 0; i < 60; i++) {
			co2PerHours[time->hour] += co2PerMinutes[i];
		}
		co2PerHours[time->hour] = co2PerHours[time->hour] / 60; // 60 поминутных выборок усредняем - средний уровнь за час
	}
}

uint16_t* CO2Sensor::GetPerSecondCo2()
{
	return this->counts;
}

uint16_t* CO2Sensor::GetPerMinuteCo2()
{
	return this->co2PerMinutes;
}

uint16_t* CO2Sensor::GetPerHourCo2()
{
	return this->co2PerHours;
}

uint16_t CO2Sensor::GetErrorsCount()
{
	return errorsCount;
}

void CO2Sensor::ResetErrorsCount()
{
	errorsCount = 0;
}