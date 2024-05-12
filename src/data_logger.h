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

	auto read_data() -> int;

	auto write_data() -> void const;

	auto print_setup() -> void const;

private:

	auto regs_init() -> void;

	auto read_power(Power& power) -> int;
	auto write_power(Power& power) -> void const;
	auto write_demand_power(Power& power) -> void const;
	auto register_read_error(uint16_t reg) -> void;

	Setup setup;
	Power real_power;
	Power apparent_power;
	Power reactive_power;
	Current current;
	Voltage voltage;

	modbus_t* ctx;

	uint16_t buf[16]{ 0 };               // buffer for reading

	bool is_float;
};