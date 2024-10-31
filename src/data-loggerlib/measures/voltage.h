#pragma once
#include <modbus.h>

struct Max
{
	float AB;
	float BC;
	float CA;
	float AN;
	float BN;
	float CN;
};

struct Min
{
	float AB;
	float BC;
	float CA;
	float AN;
	float BN;
	float CN;
};

class Voltage
{
public:
	Voltage() : LL_3P_average(0.0),
		LN_3P_average(0.0),
		AB(0.0),
		BC(0.0),
		CA(0.0),
		AN(0.0),
		BN(0.0),
		CN(0.0),
		max{ 0 },
		min{ 0 }
	{}

	float LL_3P_average;
	float LN_3P_average;

	// line-line
	float AB;
	float BC;
	float CA;

	// line-neut
	float AN;
	float BN;
	float CN;

	// max 
	Max max;

	// min
	Min min;
};