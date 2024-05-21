#pragma once
#include <modbus.h>

struct Max
{
	int16_t AB[2];
	int16_t BC[2];
	int16_t CA[2];
	int16_t AN[2];
	int16_t BN[2];
	int16_t CN[2];
};

struct Min
{
	int16_t AB[2];
	int16_t BC[2];
	int16_t CA[2];
	int16_t AN[2];
	int16_t BN[2];
	int16_t CN[2];
};

class Voltage
{
public:
	int16_t LL_3P_average[2];
	int16_t LN_3P_average[2];

	// line-line
	int16_t AB[2];
	int16_t BC[2];
	int16_t CA[2];

	// line-neut
	int16_t AN[2];
	int16_t BN[2];
	int16_t CN[2];

	// max 
	Max max;

	// min
	Min min;

};