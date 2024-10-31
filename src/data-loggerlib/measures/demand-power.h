#pragma once

#include <modbus.h>
#include <unordered_map>

struct PowerIntRegisters
{
	PowerIntRegisters() : phase_3(0),
		A(0),
		B(0),
		C(0),
		N(0),
		total(0),
		max(0),
		min(0),
		last(0),
		peak(0)
	{}
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


struct PowerFloatRegisters
{
	PowerFloatRegisters() : phase_3(0),
		A(0),
		B(0),
		C(0),
		N(0),
		total(0),
		max(0),
		min(0),
		last(0),
		peak(0)
	{}
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

class DemandPower
{
public:
	/*Displays the contents of a register by register number*/
	std::unordered_map<uint16_t, float> data;

	PowerFloatRegisters int_regs;
	PowerIntRegisters float_regs;
};