#pragma once
#include <modbus.h>
#include "demand-power.h"

class Power
{
public:
	Power(const char* _name, const char* _metric) : name(_name),
		metric(_metric)
	{}
	const char* name;
	const char* metric;
	std::unordered_map<uint16_t, float> data;

	DemandPower demand;
	PowerFloatRegisters int_regs;
	PowerIntRegisters float_regs;
};