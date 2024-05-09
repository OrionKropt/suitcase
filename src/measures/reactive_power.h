#pragma once
#include <modbus.h>
#include "demand_power.h"

class Reactive_power
{
public:
	uint16_t phase_3[2];

	uint16_t A[2];
	uint16_t B[2];
	uint16_t C[2];

	uint16_t total[2];

	uint16_t max[2];
	uint16_t min[2];

	Demand_power demand;
};