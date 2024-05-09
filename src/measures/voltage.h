#pragma once
#include <modbus.h>

class Voltage
{

	uint16_t phase_3[2];

	// line-line
	uint16_t AB[2];
	uint16_t BC[2];
	uint16_t CA[2];

	// line-neut
	uint16_t AN[2];
	uint16_t BN[2];
	uint16_t CN[2];

	// max AB
	uint16_t AB_max[2];

	// min AB
	uint16_t AB_min[2];

	// max AN
	uint16_t AN_max[2];

	// min AN
	uint16_t AN_min[2];
};