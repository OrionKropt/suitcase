#pragma once

#include <modbus.h>
#include <unordered_map>

struct Power_int_registers
{
	uint16_t phase_3;
	uint16_t A;
	uint16_t B;
	uint16_t C;
	uint16_t N;
	uint16_t total;
	uint16_t max;
	uint16_t min;
	uint16_t last;
	uint16_t peak;
};


struct Power_float_registers
{
	uint16_t phase_3;
	uint16_t A;
	uint16_t B;
	uint16_t C;
	uint16_t N;
	uint16_t total;
	uint16_t max;
	uint16_t min;
	uint16_t last;
	uint16_t peak;
};

class Demand_power
{
public:
	std::unordered_map<uint16_t, int16_t[2]> data;

	Power_float_registers int_regs;
	Power_int_registers float_regs;
};