#pragma once
#include <modbus.h>

class Current
{
public:

	int16_t phase_3[2];

	// Ampers per phase
	int16_t A[2];
	int16_t B[2];
	int16_t C[2];
	int16_t N[2];

	// Ampers demand
	int16_t A_demand[2];
	int16_t B_demand[2];
	int16_t C_demand[2];

	int16_t A_demand_peak[2];
	int16_t B_demand_peak[2];
	int16_t C_demand_peak[2];

	int16_t A_max[2];
	int16_t A_min[2];
	
	int16_t B_max[2];
	int16_t B_min[2];

	int16_t C_max[2];
	int16_t C_min[2];
	
};