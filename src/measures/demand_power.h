#pragma once

#include <modbus.h>

class Demand_power
{
public:
	uint16_t phase_3[2];

	uint16_t last[2];
	uint16_t peak[2];
	

	uint16_t total[2];
};