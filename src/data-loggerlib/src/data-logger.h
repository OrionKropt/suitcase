#pragma once

#include <modbus.h>
#include "registers.h"
#include "power.h"
#include "current.h"
#include "voltage.h"
#include "energy.h"
#include "setup.h"

#define DEBUG

class DataLogger
{
public:
	DataLogger();
	~DataLogger();
	auto init() -> void;
	auto read_data_from_device() -> int;
	auto write_data_to_file() -> void const;
	auto print_setup() -> void const;

private:

	auto regs_init() -> void;

	auto read_power_from_device(Power& power) -> int;
	auto read_current_from_device() -> int;
	auto read_voltage_from_device() -> int;
	auto read_energy_from_device() -> int;
	auto write_power_to_file(Power& power) -> void const;
	auto write_demand_power_to_file(Power& power) -> void const;
	auto write_current_to_file() -> void const;
	auto write_voltage_to_file() -> void const;
	auto write_energy_to_file() -> void const;

	auto fast_pow(float n, int16_t deg) -> float;
	inline auto get_long(uint16_t* src) -> long;
	Setup setup;
	Power real_power;
	Power apparent_power;
	Power reactive_power;
	Current current;
	Voltage voltage;
	Energy energy;
	modbus_t* ctx;

	uint16_t buf[16];               // buffer for reading

	bool is_float;
};