#pragma once
#include <modbus.h>

class Current
{
public:

	uint16_t phase_3[2];

	// Ampers per phase
	uint16_t A[2];
	uint16_t B[2];
	uint16_t C[2];
	uint16_t N[2];

	// Ampers demand
	uint16_t A_demand[2];
	uint16_t B_demand[2];
	uint16_t C_demand[2];

	uint16_t A_demand_peak[2];
	uint16_t B_demand_peak[2];
	uint16_t C_demand_peak[2];

	uint16_t A_max[2];
	uint16_t A_min[2];
	
	uint16_t B_max[2];
	uint16_t B_min[2];

	uint16_t C_max[2];
	uint16_t C_min[2];
	
};