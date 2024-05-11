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

	auto write_data() -> void;

private:
	auto read_power() -> int;
	auto read_real_power(bool is_float) -> int;

	auto register_read_error(uint16_t reg) -> void;

	Setup setup;
	Power power;
	Current current;
	Voltage voltage;

	modbus_t* ctx;

	uint16_t buf[16]{ 0 };               // buffer for reading

	bool is_float;
};