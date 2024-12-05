#pragma once

#include <modbus.h>
#include <chrono>
#include "registers.h"
#include "power.h"
#include "power-factor.h"
#include "current.h"
#include "voltage.h"
#include "energy.h"
#include "setup.h"

#define DEBUG

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class DataLogger
{
public:

	DataLogger();
	~DataLogger();
	auto init() noexcept -> void;

	/*After each change of modbus connection settings you must reconnect to the device*/
	auto modbus_connect_to_device() noexcept -> void;
	auto modbus_disconnect_form_device() noexcept -> void;
	auto read_data_from_device() noexcept -> int;
	auto write_data_to_file() noexcept -> void;
	auto print_setup() const noexcept -> void;
	auto
	inline get_long(uint16_t* src) const noexcept -> long;
	auto get_com_port_name(char *port, const int len) const noexcept -> void;
	auto get_com_port_speed_bods() const noexcept -> uint16_t;
	auto get_parity() const noexcept -> char;
	auto get_data_bits() const noexcept -> uint16_t;
	auto get_stop_bits() const noexcept -> uint16_t;
	auto get_slave_id() const noexcept -> uint16_t;
	auto get_real_power() const noexcept -> Power;
	auto get_apparent_power() const noexcept -> Power;
	auto get_reactive_power() const noexcept -> Power;
	auto get_power_factor() const noexcept -> PowerFactor;
	auto get_current() const noexcept -> Current;
	auto get_voltage() const noexcept -> Voltage;
	auto get_energy() const noexcept -> Energy;
	auto get_current_time() const noexcept -> TimePoint;
	auto set_com_port_name(char* port) noexcept -> void;                  // default "/COM1"
	auto set_com_port_speed_bods(uint16_t bods) noexcept -> void;         // default 19200
	auto set_parity(char parity) noexcept -> void;                        // default 'N'
	auto set_data_bits(uint16_t data_bits) noexcept -> void;              // default 8
	auto set_stop_bits(uint16_t stop_bits) noexcept -> void;              // default 1
	auto set_slave_id(uint16_t id) noexcept -> void;                      // default 1
private:
	auto regs_init() noexcept -> void;
	auto read_power_from_device(Power& power) noexcept -> int;
	auto read_current_from_device() noexcept -> int;
	auto read_voltage_from_device() noexcept -> int;
	auto read_energy_from_device() noexcept -> int;
	auto read_power_factor_from_device() noexcept -> int;
	auto write_power_to_file(Power& power) noexcept -> void;
	auto write_demand_power_to_file(Power& power) noexcept -> void;
	auto write_power_factor_to_file() noexcept -> void;
	auto write_current_to_file() noexcept -> void;
	auto write_voltage_to_file() noexcept -> void;
	auto write_energy_to_file() noexcept -> void;

	auto fast_pow(float n, int16_t deg) noexcept -> float;
	Setup setup;
	Power real_power;
	Power apparent_power;
	Power reactive_power;
	PowerFactor power_factor;
	Current current;
	Voltage voltage;
	Energy energy;
	TimePoint time;
	modbus_t* ctx;

	uint16_t buf[16];               // buffer for reading registers

	bool is_float;
};