#pragma once
#include <modbus.h>

#include "demand_power.h"




class Power
{
public:
	Power(const char* _name, const char* _metric) : name(_name), metric(_metric) {}
	const char* name;
	const char* metric;
	std::unordered_map<uint16_t, int16_t[2]> data;


	Demand_power demand;

	Power_float_registers int_regs;
	Power_int_registers float_regs;
};