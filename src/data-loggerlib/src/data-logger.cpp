#include "data-logger.h"
#include <fstream>
#include <iostream>
#include <limits>
#include "error.h"

DataLogger::DataLogger() : ctx(nullptr),
buf{ 0 },
is_float(true),
real_power("P", "kW"),
apparent_power("S", "kVA"),
reactive_power("Q", "kVAR"),
energy("W", "kWh", "S", "VArh", "Q", "VAh")
{}

DataLogger::~DataLogger()
{
	modbus_close(ctx);
	modbus_free(ctx);
}

auto DataLogger::init() -> void
{
	sprintf(setup.port, "\COM1");
	setup.port_speed = 19200;
	setup.parity = 'N';
	setup.data_bits = 8;
	setup.stop_bits = 1;
	setup.slave_id = 1;
	ctx = modbus_new_rtu(setup.port, setup.port_speed, setup.parity, setup.data_bits, setup.stop_bits);
	if (ctx == NULL) 
	{
		PRINT_ERROR("Failed to create a context Modbus", true)
	}
	#ifdef DEBUG
	std::cout << "Modbus context created!\n";
	#endif // DEBUG

	// Setting the device address 
	if (modbus_set_slave(ctx, setup.slave_id) == -1)
	{
		modbus_free(ctx);
		PRINT_ERROR("Failed to set the slave", true);
	}

#ifdef DEBUG
	std::cout << "Modbus slave set\n";
#endif // DEBUG
 
	if (modbus_connect(ctx) == -1)
	{
		modbus_free(ctx);
		PRINT_ERROR("Failed to connect to the device", true)
	}
	#ifdef DEBUG
	std::cout << "Successful connection!\n";
	#endif // DEBUG

	if (modbus_read_registers(ctx, SCALE_FACTOR_I_INT, 4, buf) == -1)
	{
		PRINT_ERROR("Register read failed", 1, "Register: {}", SCALE_FACTOR_I_INT)
	}

	 setup.scale_i = fast_pow(10, buf[0]);
	 setup.scale_v = fast_pow(10, buf[1]);
	 setup.scale_w = fast_pow(10, buf[2]);
	 setup.scale_e = fast_pow(10, buf[3]);

	regs_init();

	is_float = false;
}

auto DataLogger::read_data_from_device() -> int
{
	int res = 0;
	res = read_power_from_device(real_power);
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}
	
	res = read_power_from_device(apparent_power);
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_power_from_device(reactive_power);
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_power_factor_from_device();
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_current_from_device();
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_voltage_from_device();
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_energy_from_device();
	if (res)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

#ifdef DEBUG
	std::cout << "Successful reading!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::write_data_to_file() -> void const
{

#ifdef DEBUG
	std::cout << "\nPOWER\n";
#endif // DEBUG

	write_power_to_file(real_power);
	write_power_to_file(apparent_power);
	write_power_to_file(reactive_power);
	write_power_factor_to_file();

#ifdef DEBUG
	std::cout << "\nDEMAND POWER\n";
#endif // DEBUG

	write_demand_power_to_file(real_power);
	write_demand_power_to_file(apparent_power);
	write_demand_power_to_file(reactive_power);

#ifdef DEBUG
	std::cout << "\nCURRENT\n";
#endif // DEBUG

	write_current_to_file();

#ifdef DEBUG
	std::cout << "\nVOLTAGE\n";
#endif // DEBUG

	write_voltage_to_file();

#ifdef DEBUG
	std::cout << "\nENERGY\n";
#endif // DEBUG

	write_energy_to_file();
}

auto DataLogger::regs_init() -> void
{
	// ----------real_power----------
	// int
	real_power.int_regs.A = REAL_POWER_A_INT;
	real_power.int_regs.B = REAL_POWER_B_INT;
	real_power.int_regs.C = REAL_POWER_C_INT;
	real_power.int_regs.total = REAL_POWER_TOTAL_INT;
	real_power.int_regs.min = REAL_POWER_TOTAL_MIN_INT;
	real_power.int_regs.max = REAL_POWER_TOTAL_MAX_INT;
	real_power.demand.int_regs.total = REAL_POWER_TOTAL_DEMAND_PRESENT_INT;
	real_power.demand.int_regs.peak = REAL_POWER_TOTAL_DEMAND_PEAK_INT;

	// float
	real_power.float_regs.A = REAL_POWER_A_FLOAT;
	real_power.float_regs.B = REAL_POWER_B_FLOAT;
	real_power.float_regs.C = REAL_POWER_C_FLOAT;
	real_power.float_regs.total = REAL_POWER_TOTAL_FLOAT;
	real_power.float_regs.min = REAL_POWER_TOTAL_MIN_FLOAT;
	real_power.float_regs.max = REAL_POWER_TOTAL_MAX_FLOAT;

	// ----------apparent_power----------
	// int
	apparent_power.int_regs.A = APPARENT_POWER_A_INT;
	apparent_power.int_regs.B = APPARENT_POWER_B_INT;
	apparent_power.int_regs.C = APPARENT_POWER_C_INT;
	apparent_power.int_regs.total = APPARENT_POWER_TOTAL_INT;
	apparent_power.int_regs.min = APPARENT_POWER_TOTAL_MIN_INT;
	apparent_power.int_regs.max = APPARENT_POWER_TOTAL_MAX_INT;
	apparent_power.demand.int_regs.total = APPARENT_POWER_TOTAL_DEMAND_PRESENT_INT;
	apparent_power.demand.int_regs.peak = APPARENT_POWER_TOTAL_DEMAND_PEAK_INT;
	
	// float
	apparent_power.float_regs.A = APPARENT_POWER_A_FLOAT;
	apparent_power.float_regs.B = APPARENT_POWER_B_FLOAT;
	apparent_power.float_regs.C = APPARENT_POWER_C_FLOAT;
	apparent_power.float_regs.total = APPARENT_POWER_TOTAL_FLOAT;
	apparent_power.float_regs.min = APPARENT_POWER_TOTAL_MIN_FLOAT;
	apparent_power.float_regs.max = APPARENT_POWER_TOTAL_MAX_FLOAT;
	
	// ----------reactive_power----------
	// int
	reactive_power.int_regs.A = REACTIVE_POWER_A_INT;
	reactive_power.int_regs.B = REACTIVE_POWER_B_INT;
	reactive_power.int_regs.C = REACTIVE_POWER_C_INT;
	reactive_power.int_regs.total = REACTIVE_POWER_TOTAL_INT;
	reactive_power.int_regs.min = REACTIVE_POWER_TOTAL_MIN_INT;
	reactive_power.int_regs.max = REACTIVE_POWER_TOTAL_MAX_INT;
	reactive_power.demand.int_regs.total = REACTIVE_POWER_TOTAL_DEMAND_PRESENT_INT;
	reactive_power.demand.int_regs.peak = REACTIVE_POWER_TOTAL_DEMAND_PEAK_INT;

	// float
	reactive_power.float_regs.A = REACTIVE_POWER_A_FLOAT;
	reactive_power.float_regs.B = REACTIVE_POWER_B_FLOAT;
	reactive_power.float_regs.C = REACTIVE_POWER_C_FLOAT;
	reactive_power.float_regs.total = REACTIVE_POWER_TOTAL_FLOAT;
	reactive_power.float_regs.min = REACTIVE_POWER_TOTAL_MIN_FLOAT;
	reactive_power.float_regs.max = REACTIVE_POWER_TOTAL_MAX_FLOAT;

	// ----------demand_power----------
	// real
	real_power.demand.int_regs.total = REAL_POWER_TOTAL_DEMAND_PRESENT_INT;
	real_power.demand.int_regs.peak = REAL_POWER_TOTAL_DEMAND_PEAK_INT;
	real_power.demand.float_regs.total = REAL_POWER_TOTAL_DEMAND_PRESENT_FLOAT;
	real_power.demand.float_regs.peak = REAL_POWER_TOTAL_DEMAND_PEAK_FLOAT;

	// apparent
	apparent_power.demand.int_regs.total = APPARENT_POWER_TOTAL_DEMAND_PRESENT_INT;
	apparent_power.demand.int_regs.peak = APPARENT_POWER_TOTAL_DEMAND_PEAK_INT;
	apparent_power.demand.float_regs.total = APPARENT_POWER_TOTAL_DEMAND_PRESENT_FLOAT;
	apparent_power.demand.float_regs.peak = APPARENT_POWER_TOTAL_DEMAND_PEAK_FLOAT;

	// reactive
	reactive_power.demand.int_regs.total = REACTIVE_POWER_TOTAL_DEMAND_PRESENT_INT;
	reactive_power.demand.int_regs.peak = REACTIVE_POWER_TOTAL_DEMAND_PEAK_INT;
	reactive_power.demand.float_regs.total = REACTIVE_POWER_TOTAL_DEMAND_PRESENT_FLOAT;
	reactive_power.demand.float_regs.peak = REACTIVE_POWER_TOTAL_DEMAND_PEAK_FLOAT;
}


auto DataLogger::read_power_from_device(Power& power) -> int
{
	if (!is_float)
	{
		// ---------INT----------

		// phase reading
		if (modbus_read_registers(ctx, power.int_regs.A, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", true, "Register: {}", power.int_regs.A)
			return -1;
		}
		power.data[power.int_regs.A] = buf[0] * setup.scale_w;
		power.data[power.int_regs.B] = buf[1] * setup.scale_w;
		power.data[power.int_regs.C] = buf[2] * setup.scale_w;
		
		if (modbus_read_registers(ctx, power.int_regs.total, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.total)
			return -1;
		}
		power.data[power.int_regs.total] = buf[0] * setup.scale_w;

		// min max
		if (modbus_read_registers(ctx, power.int_regs.min, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.min)
			return -1;
		}
		power.data[power.int_regs.min] = buf[0] * setup.scale_w;
		
		if (modbus_read_registers(ctx, power.int_regs.max, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.max)
			return -1;
		}
		power.data[power.int_regs.max] = buf[0] * setup.scale_w;

		// demand
		if (modbus_read_registers(ctx, power.demand.int_regs.total, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.int_regs.total)
			return -1;
		}
		power.demand.data[power.demand.int_regs.total] = buf[0] * setup.scale_w;

		if (modbus_read_registers(ctx, power.demand.int_regs.peak, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.int_regs.peak)
			return -1;
		}
		power.demand.data[power.demand.int_regs.peak] = buf[0] * setup.scale_w;
		// ---------INT----------
	}
	else
	{
		// -------FLOAT----------
		// phase reading

		if (modbus_read_registers(ctx, power.float_regs.A, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.A)
			return -1;
		}
		power.data[power.float_regs.A] = modbus_get_float_abcd(buf);
		power.data[power.float_regs.B] = modbus_get_float_abcd(buf + 2);
		power.data[power.float_regs.C] = modbus_get_float_abcd(buf + 4);

		// min max
		if (modbus_read_registers(ctx, power.float_regs.min, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.min)
			return -1;
		}
		power.data[power.float_regs.min] = modbus_get_float_abcd(buf);

		if (modbus_read_registers(ctx, power.float_regs.max, 2, buf) == -1)
{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.max)
			return -1;
		}
		power.data[power.float_regs.max] = modbus_get_float_abcd(buf);

		// demand
		if (modbus_read_registers(ctx, power.demand.float_regs.total, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.float_regs.total)
			return -1;
		}
		power.demand.data[power.demand.float_regs.total] = modbus_get_float_abcd(buf);

		if (modbus_read_registers(ctx, power.demand.float_regs.peak, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.float_regs.peak)
			return -1;
		}
		power.demand.data[power.demand.float_regs.peak] = modbus_get_float_abcd(buf);
		// --------FLOAT---------
	}
	
#ifdef DEBUG
	std::cout << "Power reading completed!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::read_current_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------;

		// phase reading
		if (modbus_read_registers(ctx, CURRENT_A_INT, 4, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_INT)
			return -1;
		}
		current.A = buf[0] * setup.scale_i;
		current.B = buf[1] * setup.scale_i;
		current.C = buf[2] * setup.scale_i;
		current.N = buf[3] * setup.scale_i;

		if (modbus_read_registers(ctx, CURRENT_3P_AVERAGE_INT, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_3P_AVERAGE_INT)
			return -1;
		}
		current.phase_3 = buf[0] * setup.scale_i;

		// demand per phase
		if (modbus_read_registers(ctx, CURRENT_A_DEMAND_PRESENT_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PRESENT_INT)
			return -1;
		}
		current.A_demand = buf[0] * setup.scale_i;
		current.B_demand = buf[1] * setup.scale_i;
		current.C_demand = buf[2] * setup.scale_i;

		// demand peak per phase
		if (modbus_read_registers(ctx, CURRENT_A_DEMAND_PEAK_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PEAK_INT)
			return -1;
		}
		current.A_demand_peak = buf[0] * setup.scale_i;
		current.B_demand_peak = buf[1] * setup.scale_i;
		current.C_demand_peak = buf[2] * setup.scale_i;

		// max min
		if (modbus_read_registers(ctx, CURRENT_A_MIN_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MIN_INT)
			return -1;
		}
		current.A_min = buf[0] * setup.scale_i;
		current.B_min = buf[1] * setup.scale_i;
		current.C_min = buf[2] * setup.scale_i;

		if (modbus_read_registers(ctx, CURRENT_A_MAX_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MAX_INT)
			return -1;
		}
		current.A_max = buf[0] * setup.scale_i;
		current.B_max = buf[1] * setup.scale_i;
		current.C_max = buf[2] * setup.scale_i;

		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------
		// phase reading
		if (modbus_read_registers(ctx, CURRENT_A_FLOAT, 8, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_FLOAT)
			return -1;
		}
		current.A = modbus_get_float_abcd(buf);
		current.B = modbus_get_float_abcd(buf + 2);
		current.C = modbus_get_float_abcd(buf + 4);
		current.N = modbus_get_float_abcd(buf + 6);

		if (modbus_read_registers(ctx, CURRENT_3P_AVERAGE_FLOAT, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_3P_AVERAGE_FLOAT)
			return -1;
		}
		current.phase_3 = modbus_get_float_abcd(buf);

		// demand per phase
		if (modbus_read_registers(ctx, CURRENT_A_DEMAND_PRESENT_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PRESENT_FLOAT)
			return -1;
		}
		current.A_demand = modbus_get_float_abcd(buf);
		current.B_demand = modbus_get_float_abcd(buf + 2);
		current.C_demand = modbus_get_float_abcd(buf + 4);
	
		// demand peak per phase
		if (modbus_read_registers(ctx, CURRENT_A_DEMAND_PEAK_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PEAK_FLOAT)
			return -1;
		}
		current.A_demand_peak = modbus_get_float_abcd(buf);
		current.B_demand_peak = modbus_get_float_abcd(buf + 2);
		current.C_demand_peak = modbus_get_float_abcd(buf + 4);

		// max min
		if (modbus_read_registers(ctx, CURRENT_A_MIN_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MIN_FLOAT)
			return -1;
		}
		current.A_min = modbus_get_float_abcd(buf);
		current.B_min = modbus_get_float_abcd(buf + 2);
		current.C_min = modbus_get_float_abcd(buf + 4);

		if (modbus_read_registers(ctx, CURRENT_A_MAX_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MAX_FLOAT)
			return -1;
		}
		current.A_max = modbus_get_float_abcd(buf);
		current.B_max = modbus_get_float_abcd(buf + 2);
		current.C_max = modbus_get_float_abcd(buf + 4);
		
		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Current reading completed!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::read_voltage_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------

		// phase reading AB BC CA
		if (modbus_read_registers(ctx, VOLTAGE_AB_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_INT)
			return -1;
		}
		voltage.AB = buf[0] * setup.scale_v;
		voltage.BC = buf[1] * setup.scale_v;
		voltage.CA = buf[2] * setup.scale_v;

		// average
		if (modbus_read_registers(ctx, VOLTAGE_LL_3P_AVERAGE_INT, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_LL_3P_AVERAGE_INT)
			return -1;
		}
		voltage.LL_3P_average = buf[0] * setup.scale_v;
		voltage.LN_3P_average = buf[1] * setup.scale_v;

		// phase reading AN BN CN
		if (modbus_read_registers(ctx, VOLTAGE_AN_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_INT)
			return -1;
		}
		voltage.AN = buf[0] * setup.scale_v;
		voltage.BN = buf[1] * setup.scale_v;
		voltage.CN = buf[2] * setup.scale_v;

		// min max
		if (modbus_read_registers(ctx, VOLTAGE_AB_MIN_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MIN_INT)
			return -1;
		}
		voltage.min.AB = buf[0] * setup.scale_v;
		voltage.min.BC = buf[1] * setup.scale_v;
		voltage.min.CA = buf[2] * setup.scale_v;

		if (modbus_read_registers(ctx, VOLTAGE_AN_MIN_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MIN_INT)
			return -1;
		}
		voltage.min.AN = buf[0] * setup.scale_v;
		voltage.min.BN = buf[1] * setup.scale_v;
		voltage.min.CN = buf[2] * setup.scale_v;

		if (modbus_read_registers(ctx, VOLTAGE_AB_MAX_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MAX_INT)
			return -1;
		}
		voltage.max.CN = buf[0] * setup.scale_v;
		voltage.max.BC = buf[1] * setup.scale_v;
		voltage.max.CA = buf[2] * setup.scale_v;

		if (modbus_read_registers(ctx, VOLTAGE_AN_MAX_INT, 3, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MAX_INT)
			return -1;
		}
		voltage.max.AN = buf[0] * setup.scale_v;
		voltage.max.BN = buf[1] * setup.scale_v;
		voltage.max.CN = buf[2] * setup.scale_v;

		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------

		// phase reading AB BC CA
		if (modbus_read_registers(ctx, VOLTAGE_AB_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_FLOAT)
			return -1;
		}
		voltage.AB = modbus_get_float_abcd(buf);
		voltage.BC = modbus_get_float_abcd(buf + 2);
		voltage.CA = modbus_get_float_abcd(buf + 4);

		// average
		if (modbus_read_registers(ctx, VOLTAGE_LL_3P_AVERAGE_FLOAT, 4, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_LL_3P_AVERAGE_FLOAT)
			return -1;
		}
		voltage.LL_3P_average = modbus_get_float_abcd(buf);
		voltage.LN_3P_average = modbus_get_float_abcd(buf + 2);

		// phase reading AN BN CN
		if (modbus_read_registers(ctx, VOLTAGE_AN_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_FLOAT)
			return -1;
		}
		voltage.AN = modbus_get_float_abcd(buf);
		voltage.BN = modbus_get_float_abcd(buf + 2);
		voltage.CN = modbus_get_float_abcd(buf + 4);

		// min max
		if (modbus_read_registers(ctx, VOLTAGE_AB_MIN_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MIN_FLOAT)
			return -1;
		}
		voltage.min.AB = modbus_get_float_abcd(buf);
		voltage.min.BC = modbus_get_float_abcd(buf + 2);
		voltage.min.CA = modbus_get_float_abcd(buf + 4);

		if (modbus_read_registers(ctx, VOLTAGE_AN_MIN_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MIN_FLOAT)
			return -1;
		}
		voltage.min.AN = modbus_get_float_abcd(buf);
		voltage.min.BN = modbus_get_float_abcd(buf + 2);
		voltage.min.CN = modbus_get_float_abcd(buf + 4);

		if (modbus_read_registers(ctx, VOLTAGE_AB_MAX_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MAX_FLOAT)
			return -1;
		}
		voltage.max.AB = modbus_get_float_abcd(buf);
		voltage.max.BC = modbus_get_float_abcd(buf + 2);
		voltage.max.CA = modbus_get_float_abcd(buf + 4);

		if (modbus_read_registers(ctx, VOLTAGE_AN_MAX_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MAX_FLOAT)
			return -1;
		}
		voltage.max.AN = modbus_get_float_abcd(buf);
		voltage.max.BN = modbus_get_float_abcd(buf + 2);
		voltage.max.CN = modbus_get_float_abcd(buf + 4);

		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Voltage reading completed!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::read_energy_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------

		if (modbus_read_registers(ctx, REAL_ENERGY_TOTAL_LONG, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", REAL_ENERGY_TOTAL_LONG)
			return -1;
		}

		energy.real = get_long(buf);
		energy.apparent = get_long(buf + 2);
		energy.reactive = get_long(buf + 4);
		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------

		if (modbus_read_registers(ctx, REAL_ENERGY_TOTAL_FLOAT, 6, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", REAL_ENERGY_TOTAL_FLOAT)
			return -1;
		}

		energy.real = modbus_get_float_abcd((uint16_t*) buf);
		energy.apparent = modbus_get_float_abcd((uint16_t*) (buf + 2));
		energy.reactive = modbus_get_float_abcd((uint16_t*) (buf + 4));

		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Energy reading completed!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::read_power_factor_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------

		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL_INT, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL_INT)
			return -1;
		}
		power_factor.total = buf[0] * setup.scale_pf;

		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL_MIN_INT, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL_MIN_INT)
			return -1;
		}
		power_factor.min = buf[0] * setup.scale_pf;

		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL_MAX_INT, 1, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL_MAX_INT)
			return -1;
		}
		power_factor.max = buf[0] * setup.scale_pf;

		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------
		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL)
			return -1;
		}
		power_factor.total = modbus_get_float_abcd(buf);

		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL_MIN_FLOAT, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL_MIN_FLOAT)
			return -1;
		}
		power_factor.min = modbus_get_float_abcd(buf);

		if (modbus_read_registers(ctx, POWER_FACTOR_TOTAL_MAX_FLOAT, 2, buf) == -1)
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", POWER_FACTOR_TOTAL_MAX_FLOAT)
			return -1;
		}
		power_factor.max = modbus_get_float_abcd(buf);

		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Power factor reading completed!\n";
#endif // DEBUG

	return 0;
}

auto DataLogger::print_setup() -> void const
{
#ifdef DEBUG
	std::cout << "\nSETUP\n\n";
	std::cout << "Scale (power of 10):\n";
	std::cout << "Current " << setup.scale_i << '\n';
	std::cout << "Voltage " << setup.scale_v << '\n';
	std::cout << "Power " << setup.scale_w << '\n';
	std::cout << "Energy " << setup.scale_e << "\n\n";
	std::cout << "Total time for which the absolute current on at least one phase is > 0.1Amp:\n";
	std::cout << setup.usage_hours << " hours, " << setup.usage_minutes << " minutes\n";
	std::cout << "\nPORT SETUP\n\n";
	std::cout << "PORT: " << setup.port << '\n';
	std::cout << "PORT SPEED: " << setup.port_speed << " bauds\n";
	std::cout << "DATA BITS: " << setup.data_bits << '\n';
	std::cout << "STOP BITS: " << setup.stop_bits << '\n';
	std::cout << "Device ID: " << setup.slave_id << '\n';
#endif // DEBUG
}

auto DataLogger::write_power_to_file(Power& power) -> void const
{
#ifdef DEBUG
	std::cout << power.name << ", " << power.metric << '\n';
	std::cout << "A\tB\tC\n";
	std::cout << real_power.data[power.int_regs.A] << '\t'
		<< real_power.data[power.int_regs.B] << '\t'
		<< real_power.data[power.int_regs.C] << '\n';
	std::cout << "Total " << power.data[power.int_regs.total] << ' ' << power.metric << '\n';
	std::cout << "Min = " << power.data[power.int_regs.min] << ' ' << power.metric << '\n';
	std::cout << "Max = " << power.data[power.int_regs.max] << ' ' << power.metric << "\n\n";
#endif // DEBUG
}

auto DataLogger::write_demand_power_to_file(Power& power) -> void const
{
#ifdef DEBUG
	std::cout << power.name << ", " << power.metric << '\n';
	std::cout << "Total " << power.demand.data[power.demand.int_regs.total] << ' ' << power.metric << '\n';
	std::cout << "Peak " << power.demand.data[power.demand.int_regs.peak] << ' ' << power.metric << "\n\n";
#endif // DEBUG
}

auto DataLogger::write_power_factor_to_file() -> void const
{
#ifdef DEBUG
	std::cout << "Power factor\n";
	std::cout << "Total = " << power_factor.total << '\n';
	std::cout << "Max = " << power_factor.max << '\n';
	std::cout << "Min = " << power_factor.min << "\n\n";
#endif // DEBUG
}

auto DataLogger::write_current_to_file() -> void const
{
#ifdef DEBUG
	std::cout << "I = " << current.phase_3 << '\n';
	std::cout << "Present\n";
	std::cout << "\tA\tB\tC\tN\n";
	std::cout << '\t' << current.A << '\t' << current.B << '\t' << current.C << '\t' << current.N << "\n\n";
	std::cout << "MAX\t" << current.A_max << '\t' << current.B_max << '\t' << current.C_max << '\n';
	std::cout << "MIN\t" << current.A_min << '\t' << current.B_min << '\t' << current.C_min << "\n\n";
	std::cout << "Demand\n";
	std::cout << "A\tB\tC\n";
	std::cout << current.A_demand << '\t' << current.B_demand << '\t' << current.C_demand << "\n\n";
	std::cout << "Peak\n";
	std::cout << "A\tB\tC\n";
	std::cout << current.A_demand_peak << '\t' << current.B_demand_peak << '\t' << current.C_demand_peak << "\n\n";
#endif // DEBUG

	

}

auto DataLogger::write_voltage_to_file() -> void const
{
#ifdef DEBUG
	std::cout << "Voltage L-L, 3 phase " << voltage.LL_3P_average << '\n';
	std::cout << "Voltage L-N, 3 phase " << voltage.LN_3P_average << '\n';
	std::cout << "AB\tBC\tCA\n";
	std::cout << voltage.AB << '\t' << voltage.BC << '\t' << voltage.CA << "\n\n";
	std::cout << "AN\tBN\tCN\n";
	std::cout << voltage.AN << '\t' << voltage.BN << '\t' << voltage.CN << "\n\n";
	std::cout << "\tMAX\t\t\t\t\tMIN\n\n";
	std::cout << "AB\tBC\tCA\t\t\tAB\tBC\tCA\n";
	std::cout << voltage.max.AB << '\t' << voltage.max.BC << '\t'
		<< voltage.max.CA << "\t\t\t" << voltage.min.AB << '\t'
		<< voltage.min.BC << '\t' << voltage.min.CA << "\n\n";
	std::cout << "AN\tBN\tCN\t\t\tAN\tBN\tCN\n";
	std::cout << voltage.max.AN << '\t' << voltage.max.BN
		<< '\t' << voltage.max.CN << "\t\t\t" << voltage.min.AN
		<< '\t' << voltage.min.BN << '\t' << voltage.min.CN << '\n';
#endif // DEBUG

}

auto DataLogger::write_energy_to_file() -> void const
{
#ifdef DEBUG
	std::cout << energy.name_real << " = " << energy.real << ' ' << energy.metric_real << ' ' << '\n';
	std::cout << energy.name_reactive << " = " << energy.reactive << ' ' << energy.metric_reactive << '\n';
	std::cout << energy.name_apparent << " = " << energy.apparent << ' ' << energy.metric_apparent << '\n';
#endif // DEBUG
}

auto DataLogger::fast_pow(float n, int16_t deg) -> float
{
	if (deg == 0) return 1;
	
	if (deg < 0)
	{
		n = 1 / n;
		deg = -deg;
	}

	if (deg % 2 == 0)
	{
		float a = fast_pow(n, deg / 2);
		return a * a;
	}
	else
	{
		float a = fast_pow(n, (deg - 1) / 2);
		return a * a * n;
	}
}

inline auto DataLogger::get_long(uint16_t* src) -> long
{
	if (src == nullptr)
	{
		PRINT_ERROR("Pointer not initialized", true);
		return -1;
	}
	
	return (((long)src[0]) << 16) | src[1];
}



