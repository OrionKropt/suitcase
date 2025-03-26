#pragma once
#include <modbus.h>

class Energy
{
public:
	Energy(const char* _name_real, const char* _metric_real,
		const char* _name_reactive, const char* _metric_reactive,
		const char* _name_apparent, const char* _metric_apparent) : real(0.0),
		apparent(0.0),
		reactive(0.0),
		name_real(_name_real),
		metric_real(_metric_real),
		name_apparent(_name_apparent),
		metric_apparent(_metric_apparent),
		name_reactive(_name_reactive),
		metric_reactive(_metric_reactive)
	{}
	
	const char* metric_real;
	const char* metric_apparent;
	const char* metric_reactive;
	const char* name_real;
	const char* name_apparent;
	const char* name_reactive;
		
	float real;
	float reactive;
	float apparent;
};