#pragma once
#include <modbus.h>

class Current
{
public:
	Current() : phase_3(0.0),
		A(0.0),
		B(0.0),
		C(0.0),
		N(0.0),
		A_demand(0.0),
		B_demand(0.0),
		C_demand(0.0),
		A_demand_peak(0.0),
		B_demand_peak(0.0),
		C_demand_peak(0.0),
		A_max(0.0),
		B_max(0.0),
		C_max(0.0),
		A_min(0.0),
		B_min(0.0),
		C_min(0.0)
	{}

	float phase_3;

	// Ampers per phase
	float A;
	float B;
	float C;
	float N;

	// Ampers demand
	float A_demand;
	float B_demand;
	float C_demand;

	float A_demand_peak;
	float B_demand_peak;
	float C_demand_peak;

	float A_max;
	float A_min;

	float B_max;
	float B_min;

	float C_max;
	float C_min;
};