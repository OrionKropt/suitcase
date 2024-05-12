#include "data_logger.h"

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
	sprintf(setup.port, "\COM2");
	setup.port_speed = 19200;
	setup.parity = 'N';
	setup.data_bits = 8;
	setup.stop_bits = 1;
	setup.slave_id = 1;
	ctx = modbus_new_rtu(setup.port, setup.port_speed, setup.parity, setup.data_bits, setup.stop_bits);
	if (ctx == NULL) 
	{
		#ifdef DEBUG
			std::cerr << "Failed to create a context Modbus!\n";
			std::cerr << "Data_logger::init()\n";
		#endif // DEBUG

		return;
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
		#ifdef DEBUG
		std::cerr << "Failed to connect to the device!\n";
		std::cerr << "Data_logger::init()\n";
		#endif // DEBUG

		modbus_free(ctx);
		return;
	}
	else
	{
		#ifdef DEBUG
		std::cout << "Successful connection!\n";
		#endif // DEBUG
	}

	if (-1 == modbus_read_registers(ctx, SCALE_FACTOR_I_INT, 4, buf))
	{
		#ifdef DEBUG
		std::cerr << "Register read error\n";
		std::cerr << "Data_logger::init()\n";
		std::cerr << "Register " << SCALE_FACTOR_I_INT << '\n';
		#endif // DEBUG
		return;
	}

	setup.scale_i[0] = buf[0];
	setup.scale_v[0] = buf[1];
	setup.scale_w[0] = buf[2];
	setup.scale_e[0] = buf[3];

	regs_init();

	is_float = true;
}

auto Data_logger::read_data() -> int
{
	read_power(real_power);
//	if (
//		-1 == read_power(real_power) ||
//		-1 == read_power(apparent_power) ||
//		-1 == read_power(reactive_power)
//		)
//	{
//#ifdef DEBUG
//		std::cerr << "Error reading data\n";
//		std::cerr << "Data_logger::read_data()\n";
//#endif // DEBUG
//
//		return -1;
//	}
	
#ifdef DEBUG
	std::cout << "Successful reading\n";
#endif // DEBUG
	return 0;
}

auto Data_logger::write_data() -> void const 
{
	//
#ifdef DEBUG
	std::cout << "\nPOWER\n";
#endif // DEBUG

	write_power(real_power);
	write_power(apparent_power);
	write_power(reactive_power);

	//
#ifdef DEBUG
	std::cout << "\nDEMAND POWER\n";
#endif // DEBUG

	write_demand_power(real_power);
	write_demand_power(apparent_power);
	write_demand_power(reactive_power);
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
	apparent_power.float_regs.total = APPARENT_POWER_TOTAL_FLOAT;
	
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
	reactive_power.float_regs.total = REACTIVE_POWER_TOTAL_FLOAT;
}


auto Data_logger::read_power(Power& power) -> int
{
	
	if (!is_float)
	{
		// ---------INT----------

		// phase reading

		if (-1 == modbus_read_registers(ctx, power.int_regs.A, 3, buf))
		{
			register_read_error(power.int_regs.A);
			return -1;
		}
		power.data[power.int_regs.A][0] = buf[0];
		power.data[power.int_regs.B][0] = buf[1];
		power.data[power.int_regs.C][0] = buf[2];
		

		if (-1 == modbus_read_registers(ctx, power.int_regs.total, 1, buf))
		{
			register_read_error(power.int_regs.total);
			return -1;
		}
		power.data[power.int_regs.total][0] = buf[0];


		// min max
		if (-1 == modbus_read_registers(ctx, power.int_regs.min, 1, buf))
		{
			register_read_error(power.int_regs.min);
			return -1;
		}
		power.data[power.int_regs.min][0] = buf[0];
		if (-1 == modbus_read_registers(ctx, power.int_regs.max, 1, buf))
		{
			register_read_error(power.int_regs.max);
			return -1;
		}
		power.data[power.int_regs.max][0] = buf[0];

		// demand
		if (-1 == modbus_read_registers(ctx, power.demand.int_regs.total, 1, buf))
		{
			register_read_error(power.demand.int_regs.total);
			return -1;
		}
		power.demand.data[power.demand.int_regs.total][0] = buf[0];
		if (-1 == modbus_read_registers(ctx, power.demand.int_regs.peak, 1, buf))
		{
			register_read_error(power.demand.int_regs.peak);
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
			register_read_error(power.float_regs.A);
			return -1;
		}
		memcpy_s(power.data[power.float_regs.A], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		memcpy_s(power.data[power.float_regs.B], sizeof(uint16_t) * 2, buf + 2, sizeof(uint16_t) * 2);
		memcpy_s(power.data[power.float_regs.C], sizeof(uint16_t) * 2, buf + 4, sizeof(uint16_t) * 2);
		
		/*if (-1 == modbus_read_registers(ctx, power.float_regs.total, 2, buf))
		{
			register_read_error(power.float_regs.total);
			return -1;
		}
		memcpy_s(power.data[power.float_regs.total], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);*/

		// min max
		if (-1 == modbus_read_registers(ctx, power.float_regs.min, 2, buf))
		{
			register_read_error(power.float_regs.min);
			return -1;
		}
		memcpy_s(power.data[power.float_regs.min], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
	
		if (-1 == modbus_read_registers(ctx, power.float_regs.max, 2, buf))
		{
			register_read_error(power.float_regs.max);
			return -1;
		}
		memcpy_s(power.data[power.float_regs.max], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
	}

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

auto Data_logger::register_read_error(uint16_t reg) -> void
{
#ifdef DEBUG
	std::cerr << "Register read error\n";
	std::cerr << "Data_logger::read_power()\n";
	std::cerr << "Register " << reg << '\n';
#endif // DEBUG

}

auto Data_logger::write_power(Power& power) -> void const
{
	float A = 0, B = 0, C = 0;
	float min = 0, max = 0, total = 0;
	if (!is_float)
	{
		A = real_power.data[power.int_regs.A][0];
		B = real_power.data[power.int_regs.B][0];
		C = real_power.data[power.int_regs.C][0];
		total = power.data[power.int_regs.total][0];
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
	std::cout << A << "\t" << B << "\t" << C << '\n';
	std::cout << "Total " << total << ' ' << power.metric << '\n';
	std::cout << "Min = " << min << ' ' << power.metric << '\n';
	std::cout << "Max = " << max << ' ' << power.metric << "\n\n";
#endif // DEBUG
}

auto Data_logger::write_demand_power(Power& power) -> void const
{
	float total = 0, peak = 0;
	if (!is_float)
	{
		total = power.demand.data[power.demand.int_regs.total][0];
		peak = power.demand.data[power.demand.int_regs.peak][0];
	}
	else
	{
		total = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.total]);
		peak = modbus_get_float_abcd( (uint16_t*) power.data[power.float_regs.peak]);
	}
	std::cout << power.name << ", " << power.metric << '\n';
	std::cout << "Total " << total << ' ' << power.metric << '\n';
	std::cout << "Peak " << power.demand.data[power.demand.int_regs.peak][0] << ' ' << power.metric << "\n\n";
}




