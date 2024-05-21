#include "data_logger.h"
#include <fstream>
#include <format>
#include <iostream>

#include "error.h"


Data_logger::Data_logger() : ctx(nullptr), real_power("P", "kW"),
apparent_power("S", "kVA"),
reactive_power("Q", "kVAR")
{}


Data_logger::~Data_logger()
{
	modbus_close(ctx);
	modbus_free(ctx);
}


auto Data_logger::init() -> void
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
	else
	{
		#ifdef DEBUG
		std::cout << "Modbus context created!\n";
		#endif // DEBUG
	}

	// Setting the device address 
	modbus_set_slave(ctx, setup.slave_id);

	// Setting to RS-485
	modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);

	// Connecting to the device 
	if (modbus_connect(ctx) == -1)
	{
		modbus_free(ctx);
		PRINT_ERROR("Failed to connect to the device", true)
	}
	else
	{
		#ifdef DEBUG
		std::cout << "Successful connection!\n";
		#endif // DEBUG
	}

	if (-1 == modbus_read_registers(ctx, SCALE_FACTOR_I_INT, 4, buf))
	{
		PRINT_ERROR("Register read failed", 1, "Register: {}", SCALE_FACTOR_I_INT)
	}

	setup.scale_i[0] = buf[0];
	setup.scale_v[0] = buf[1];
	setup.scale_w[0] = buf[2];
	setup.scale_e[0] = buf[3];

	regs_init();

	is_float = true;
}

auto Data_logger::read_data_from_device() -> int
{
	int res = 0;
	res = read_power_from_device(real_power);
	if (res == -1)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}
	

	res = read_power_from_device(apparent_power);
	if (res == -1)
	{
#ifdef DEBUG
		PRINT_ERROR("Error reading data", false)
#endif // DEBUG
		return -1;
	}

	res = read_power_from_device(reactive_power);
	if (res == -1)
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

#ifdef DEBUG
	std::cout << "Successful reading!\n";
#endif // DEBUG

	return 0;
}

auto Data_logger::write_data_to_file() -> void const
{

#ifdef DEBUG
	std::cout << "\nPOWER\n";
#endif // DEBUG

	write_power_to_file(real_power);
	write_power_to_file(apparent_power);
	write_power_to_file(reactive_power);

	
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

	
}

auto Data_logger::regs_init() -> void
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


auto Data_logger::read_power_from_device(Power& power) -> int
{
	
	if (!is_float)
	{
		// ---------INT----------

		// phase reading

		if (-1 == modbus_read_registers(ctx, power.int_regs.A, 3, buf))
		{
			PRINT_ERROR("Register read failed", true, "Register: {}", power.int_regs.A)
			return -1;
		}
		power.data[power.int_regs.A][0] = buf[0];
		power.data[power.int_regs.B][0] = buf[1];
		power.data[power.int_regs.C][0] = buf[2];
		

		if (-1 == modbus_read_registers(ctx, power.int_regs.total, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.total)
			return -1;
		}
		power.data[power.int_regs.total][0] = buf[0];




		// min max
		if (-1 == modbus_read_registers(ctx, power.int_regs.min, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.min)
			return -1;
		}
		power.data[power.int_regs.min][0] = buf[0];
		if (-1 == modbus_read_registers(ctx, power.int_regs.max, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.int_regs.max)
			return -1;
		}
		power.data[power.int_regs.max][0] = buf[0];

		// demand
		if (-1 == modbus_read_registers(ctx, power.demand.int_regs.total, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.int_regs.total)
			return -1;
		}
		power.demand.data[power.demand.int_regs.total][0] = buf[0];
		if (-1 == modbus_read_registers(ctx, power.demand.int_regs.peak, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.int_regs.peak)
			return -1;
		}
		power.demand.data[power.demand.int_regs.peak][0] = buf[0];
		// ---------INT----------
		
	}
	else
	{
		// -------FLOAT----------
		// phase reading

		if (-1 == modbus_read_registers(ctx, power.float_regs.A, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.A)
			return -1;
		}
		memcpy_s(power.data[power.float_regs.A], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(power.data[power.float_regs.B], sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(power.data[power.float_regs.C], sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		// min max
		if (-1 == modbus_read_registers(ctx, power.float_regs.min, 2, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.min)
			return -1;
}
		memcpy_s(power.data[power.float_regs.min], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, power.float_regs.max, 2, buf))
{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.float_regs.max)
			return -1;
		}
		memcpy_s(power.data[power.float_regs.max], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);

		// demand
		if (-1 == modbus_read_registers(ctx, power.demand.float_regs.total, 2, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.float_regs.total)
			return -1;
		}
		memcpy_s(power.demand.data[power.demand.float_regs.total], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, power.demand.float_regs.peak, 2, buf))
		{

			PRINT_ERROR("Register read failed", false, "Register: {}", power.demand.float_regs.peak)
			return -1;
		}
		memcpy_s(power.demand.data[power.demand.float_regs.peak], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		
		// --------FLOAT---------

	}
	
#ifdef DEBUG
	std::cout << "Power reading completed!\n";
#endif // DEBUG

	return 0;
}

auto Data_logger::read_current_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------
		
		// phase reading
		if (-1 == modbus_read_registers(ctx, CURRENT_A_INT, 4, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_INT)
			return -1;
		}
		current.A[0] = buf[0];
		current.B[0] = buf[1];
		current.C[0] = buf[2];
		current.N[0] = buf[3];

		if (-1 == modbus_read_registers(ctx, CURRENT_3P_AVERAGE_INT, 1, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_3P_AVERAGE_INT)
			return -1;
		}
		current.phase_3[0] = buf[0];

		// demand per phase
		if (-1 == modbus_read_registers(ctx, CURRENT_A_DEMAND_PRESENT_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PRESENT_INT)
			return -1;
		}
		current.A_demand[0] = buf[0];
		current.B_demand[0] = buf[1];
		current.C_demand[0] = buf[2];

		// demand peak per phase
		if (-1 == modbus_read_registers(ctx, CURRENT_A_DEMAND_PEAK_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PEAK_INT)
			return -1;
		}
		current.A_demand_peak[0] = buf[0];
		current.B_demand_peak[0] = buf[1];
		current.C_demand_peak[0] = buf[2];

		// max min
		if (-1 == modbus_read_registers(ctx, CURRENT_A_MIN_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MIN_INT)
			return -1;
		}
		current.A_min[0] = buf[0];
		current.B_min[0] = buf[1];
		current.C_min[0] = buf[2];

		if (-1 == modbus_read_registers(ctx, CURRENT_A_MAX_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MAX_INT)
			return -1;
		}
		current.A_max[0] = buf[0];
		current.B_max[0] = buf[1];
		current.C_max[0] = buf[2];

		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------
		// phase reading
		if (-1 == modbus_read_registers(ctx, CURRENT_A_FLOAT, 8, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_FLOAT)
			return -1;
		}
		memcpy_s(current.A, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(current.B, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(current.C, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);
		memcpy_s(current.N, sizeof(uint16_t) * 2, buf + 6, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, CURRENT_3P_AVERAGE_FLOAT, 2, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_3P_AVERAGE_FLOAT)
			return -1;
		}
		memcpy_s(current.phase_3, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);

		// demand per phase
		if (-1 == modbus_read_registers(ctx, CURRENT_A_DEMAND_PRESENT_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PRESENT_FLOAT)
			return -1;
		}
		memcpy_s(current.A_demand, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(current.B_demand, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(current.C_demand, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);
	
		// demand peak per phase
		if (-1 == modbus_read_registers(ctx, CURRENT_A_DEMAND_PEAK_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_DEMAND_PEAK_FLOAT)
				return -1;
		}
		memcpy_s(current.A_demand_peak, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(current.B_demand_peak, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(current.C_demand_peak, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);


		// max min
		if (-1 == modbus_read_registers(ctx, CURRENT_A_MIN_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MIN_FLOAT)
			return -1;
		}
		memcpy_s(current.A_min, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(current.B_min, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(current.C_min, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, CURRENT_A_MAX_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", CURRENT_A_MAX_FLOAT)
			return -1;
		}
		memcpy_s(current.A_max, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(current.B_max, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(current.C_max, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);
		
		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Current reading completed!\n";
#endif // DEBUG

	return 0;
}

auto Data_logger::read_voltage_from_device() -> int
{
	if (!is_float)
	{
		// ---------INT----------

		// phase reading AB BC CA
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_INT)
			return -1;
		}
		voltage.AB[0] = buf[0];
		voltage.BC[0] = buf[1];
		voltage.CA[0] = buf[2];

		// average
		if (-1 == modbus_read_registers(ctx, VOLTAGE_LL_3P_AVERAGE_INT, 2, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_LL_3P_AVERAGE_INT)
			return -1;
		}
		voltage.LL_3P_average[0] = buf[0];
		voltage.LN_3P_average[0] = buf[1];

		// phase reading AN BN CN
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_INT)
			return -1;
		}
		voltage.AN[0] = buf[0];
		voltage.BN[0] = buf[1];
		voltage.CN[0] = buf[2];

		// min max
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_MIN_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MIN_INT)
			return -1;
		}
		voltage.min.AB[0] = buf[0];
		voltage.min.BC[0] = buf[1];
		voltage.min.CA[0] = buf[2];

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_MIN_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MIN_INT)
			return -1;
		}
		voltage.min.AN[0] = buf[0];
		voltage.min.BN[0] = buf[1];
		voltage.min.CN[0] = buf[2];

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_MAX_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MAX_INT)
			return -1;
		}
		voltage.max.CN[0] = buf[0];
		voltage.max.BC[0] = buf[1];
		voltage.max.CA[0] = buf[2];

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_MAX_INT, 3, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MAX_INT)
			return -1;
		}
		voltage.max.AN[0] = buf[0];
		voltage.max.BN[0] = buf[1];
		voltage.max.CN[0] = buf[2];

		// ---------INT----------
	}
	else
	{
		// --------FLOAT---------

		// phase reading AB BC CA
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_FLOAT)
			return -1;
		}
		memcpy_s(voltage.AB, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.BC, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.CA, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		// average
		if (-1 == modbus_read_registers(ctx, VOLTAGE_LL_3P_AVERAGE_FLOAT, 4, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_LL_3P_AVERAGE_FLOAT)
			return -1;
		}
		memcpy_s(voltage.LL_3P_average, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.LN_3P_average, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);

		// phase reading AN BN CN
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_FLOAT)
			return -1;
		}
		memcpy_s(voltage.AN, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.BN, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.CN, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		// min max
		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_MIN_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MIN_FLOAT)
			return -1;
		}
		memcpy_s(voltage.min.AB, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.min.BC, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.min.CA, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_MIN_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MIN_FLOAT)
			return -1;
		}
		memcpy_s(voltage.min.AN, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.min.BN, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.min.CN, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AB_MAX_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AB_MAX_FLOAT)
			return -1;
		}
		memcpy_s(voltage.max.CN, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.max.BC, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.max.CA, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		if (-1 == modbus_read_registers(ctx, VOLTAGE_AN_MAX_FLOAT, 6, buf))
		{
			PRINT_ERROR("Register read failed", false, "Register: {}", VOLTAGE_AN_MAX_FLOAT)
			return -1;
		}
		memcpy_s(voltage.max.AN, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(voltage.max.BN, sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(voltage.max.CN, sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);

		// --------FLOAT---------
	}

#ifdef DEBUG
	std::cout << "Voltage reading completed!\n";
#endif // DEBUG

	return 0;
}

auto Data_logger::print_setup() -> void const
{
#ifdef DEBUG
	std::cout << "\nSETUP\n\n";
	std::cout << "Scale (power of 10):\n";
	std::cout << "Current " << setup.scale_i[0] << '\n';
	std::cout << "Voltage " << setup.scale_v[0] << '\n';
	std::cout << "Power " << setup.scale_w[0] << '\n';
	std::cout << "Energy " << setup.scale_e[0] << "\n\n";
	std::cout << "Total time for which the absolute current on at least one phase is > 0.1Amp:\n";
	std::cout << setup.usage_hours[0] << " hours, " << setup.usage_minutes[0] << " minutes\n";
	std::cout << "\nPORT SETUP\n\n";
	std::cout << "PORT: " << setup.port << '\n';
	std::cout << "PORT SPEED: " << setup.port_speed << " bauds\n";
	std::cout << "DATA BITS: " << setup.data_bits << '\n';
	std::cout << "STOP BITS: " << setup.stop_bits << '\n';
	std::cout << "Device ID: " << setup.slave_id << '\n';
#endif // DEBUG

}


auto Data_logger::write_power_to_file(Power& power) -> void const
{
	float A = 0, B = 0, C = 0;
	float min = 0, max = 0, total = 0;
	int pow = setup.scale_w[0];
	int scale = fast_pow(10, pow);

	
	if (!is_float)
	{
		A = (float) real_power.data[power.int_regs.A][0] / scale;
		B = (float) real_power.data[power.int_regs.B][0] / scale;
		C = (float) real_power.data[power.int_regs.C][0] / scale;
		total = (float) power.data[power.int_regs.total][0] / scale;
		min = power.data[power.int_regs.min][0];
		max = power.data[power.int_regs.max][0];
	}
	else
	{
		A = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.A]);
		B = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.B]);
		C = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.C]);
		min = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.min]);
		max = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.max]);
		total = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.total]);
	}
#ifdef DEBUG
	std::cout << power.name << ", " << power.metric << '\n';
	std::cout << "A\tB\tC\n";
	std::cout << A << '\t' << B << '\t' << C << '\n';
	std::cout << "Total " << total << ' ' << power.metric << '\n';
	std::cout << "Min = " << min << ' ' << power.metric << '\n';
	std::cout << "Max = " << max << ' ' << power.metric << "\n\n";
#endif // DEBUG
}

auto Data_logger::write_demand_power_to_file(Power& power) -> void const
{
	float total = 0, peak = 0;
	int pow = setup.scale_w[0];
	int scale = fast_pow(10, pow);
	if (!is_float)
	{
		total = (float) power.demand.data[power.demand.int_regs.total][0] / scale;
		peak = (float) power.demand.data[power.demand.int_regs.peak][0] / scale;
	}
	else
	{
		total = modbus_get_float_abcd( (uint16_t*) power.demand.data[power.demand.float_regs.total]);
		peak = modbus_get_float_abcd( (uint16_t*) power.demand.data[power.demand.float_regs.peak]);
	}
	std::cout << power.name << ", " << power.metric << '\n';
	std::cout << "Total " << total << ' ' << power.metric << '\n';
	std::cout << "Peak " << peak << ' ' << power.metric << "\n\n";
}

auto Data_logger::write_current_to_file() -> void const
{
	float phase_3 = 0;
	float A = 0, B = 0, C = 0, N = 0;
	float A_demand = 0, B_demand = 0, C_demand = 0;
	float A_demand_peak = 0, C_demand_peak = 0, B_demand_peak = 0;
	float A_max = 0, A_min = 0, B_max = 0, B_min = 0, C_max = 0, C_min = 0;

	int pow = setup.scale_i[0];
	int scale = fast_pow(10, pow);
	if (!is_float)
	{
		phase_3 = (float)current.phase_3[0] / scale;

		A = (float) current.A[0] / scale;
		B = (float) current.B[0] / scale;
		C = (float) current.C[0] / scale;
		N = (float) current.N[0] / scale;

		A_demand = (float) current.A_demand[0] / scale;
		B_demand = (float) current.B_demand[0] / scale;
		C_demand = (float) current.C_demand[0] / scale;

		A_demand_peak = (float) current.A_demand_peak[0] / scale;
		B_demand_peak = (float) current.B_demand_peak[0] / scale;
		C_demand_peak = (float) current.C_demand_peak[0] / scale;

		A_max = (float) current.A_max[0] / scale;
		A_min = (float) current.A_min[0] / scale;
		B_max = (float) current.B_max[0] / scale;
		B_min = (float) current.B_min[0] / scale;
		C_max = (float) current.C_max[0] / scale;
		C_min = (float) current.C_min[0] / scale;
	}
	else
	{
		A = modbus_get_float_abcd((uint16_t*) current.A);
		B = modbus_get_float_abcd((uint16_t*) current.B);
		C = modbus_get_float_abcd((uint16_t*) current.C);
		N = modbus_get_float_abcd((uint16_t*) current.N);

		A_demand = modbus_get_float_abcd((uint16_t*) current.A_demand);
		B_demand = modbus_get_float_abcd((uint16_t*) current.B_demand);
		C_demand = modbus_get_float_abcd((uint16_t*) current.C_demand);

		A_demand_peak = modbus_get_float_abcd((uint16_t*) current.A_demand_peak);
		B_demand_peak = modbus_get_float_abcd((uint16_t*) current.B_demand_peak);
		C_demand_peak = modbus_get_float_abcd((uint16_t*) current.C_demand_peak);

		A_max = modbus_get_float_abcd((uint16_t*) current.A_max);
		A_min = modbus_get_float_abcd((uint16_t*) current.A_min);
		B_max = modbus_get_float_abcd((uint16_t*) current.B_max);
		B_min = modbus_get_float_abcd((uint16_t*) current.B_min);
		C_max = modbus_get_float_abcd((uint16_t*) current.C_max);
		C_min = modbus_get_float_abcd((uint16_t*) current.C_min);
	}
#ifdef DEBUG
	std::cout << "I = " << phase_3 << '\n';
	std::cout << "Present\n";
	std::cout << "\tA\tB\tC\tN\n";
	std::cout << '\t' << A << '\t' << B << '\t' << C << '\t' << N << "\n\n";
	std::cout << "MAX\t" << A_max << '\t' << B_max << '\t' << C_max << '\n';
	std::cout << "MIN\t" << A_min << '\t' << B_min << '\t' << C_min << "\n\n";
	std::cout << "Demand\n";
	std::cout << "A\tB\tC\n";
	std::cout << A_demand << '\t' << B_demand << '\t' << C_demand << "\n\n";
	std::cout << "Peak\n";
	std::cout << "A\tB\tC\n";
	std::cout << A_demand_peak << '\t' << B_demand_peak << '\t' << C_demand_peak << "\n\n";
#endif // DEBUG

	

}

auto Data_logger::write_voltage_to_file() -> void const
{
	float LL_3P_average = 0, LN_3P_average = 0;
	float AB = 0, BC = 0, CA = 0;
	float AN = 0, BN = 0, CN = 0;
	float AB_max = 0, BC_max = 0, CA_max = 0;
	float AN_max = 0, BN_max = 0, CN_max = 0;
	float AB_min = 0, BC_min = 0, CA_min = 0;
	float AN_min = 0, BN_min = 0, CN_min = 0;

}



auto Data_logger::fast_pow(const int& n, const int& m) -> int
{
	if (m == 0) return 1;
	
	if (m % 2 == 0)
	{
		int a = fast_pow(n, m / 2);
		return a * a;
	}
	else
	{
		int a = fast_pow(n, (m - 1) / 2);
		return a * a * n;
	}
}



