#pragma once
#include <modbus.h>

#include "real_power.h"
#include "apparent_power.h"
#include "reactive_power.h"
#include "demand_power.h"

class Power
{
public:
	Real_power real;
	Apparent_power apparent;
	Reactive_power reactive;
};