#pragma once
#include <modbus.h>
#include "demand_power.h"

class Real_power
{
public:

	static const char* name;

	uint16_t phase_3[2];

	uint16_t A[2]{ 0 };
	uint16_t B[2]{ 0 };
	uint16_t C[2]{ 0 };

	uint16_t total[2]{ 0 };

	uint16_t max[2]{ 0 };
	uint16_t min[2]{ 0 };

	Demand_power demand;
};

