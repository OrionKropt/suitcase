#pragma once

#include <fstream>
#include <modbus.h>
#include "registers.h"
#include "power.h"
#include "current.h"
#include "voltage.h"
#include "setup.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


class Data_logger
{
public:
	Data_logger();
	~Data_logger();
	auto init() -> void;

	auto read_data() -> void;

private:
	auto read_power() -> int;

	Setup setup;
	Power power;
	Current current;
	Voltage voltage;

	modbus_t* ctx;
};