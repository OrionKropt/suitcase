#include "data_logger.h"

Data_logger::Data_logger() : ctx(nullptr), real_power("P"),
apparent_power("S"),
reactive_power("Q")
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

	regs_init();

	is_float = false;
}

auto Data_logger::read_data() -> void
{
	if (-1 == read_power(real_power))
	{
		// error log
		return;
	}
}

auto Data_logger::write_data() -> void const 
{
	
	write_power(real_power);

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

	// float
	real_power.float_regs.min = REAL_POWER_TOTAL_MIN_FLOAT;

	// ----------apparent_power----------
	// int
	apparent_power.int_regs.A = APPARENT_POWER_A_INT;
}


auto Data_logger::read_power(Power& power) -> int
{
	
	if (!is_float)
	{
		// ---------INT----------

		// phase reading

		if (-1 != modbus_read_registers(ctx, power.int_regs.A, 3, buf))
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
		// ---------INT----------
		return 0;
	}
	else
	{
		// min max
		if (-1 == modbus_read_registers(ctx, power.float_regs.total, 2, buf))
		{
			register_read_error(power.float_regs.total);
			return -1;
		}
		memcpy_s(power.data[power.float_regs.total], sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		return 0;
	}
}

auto Data_logger::register_read_error(uint16_t reg) -> void
{
#ifdef DEBUG
	std::cerr << "Register read error\n";
	std::cerr << "Data_logger::register_read_error()\n";
	std::cerr << "Register " << reg << '\n';
#endif // DEBUG

}

auto Data_logger::write_power(Power& power) -> void const
{
#ifdef DEBUG
	std::cout << "\nPower\n";
	std::cout << power.name << ", kW" << '\n';
	std::cout << "A\tB\tC\n";
	if (!is_float)
	{
		std::cout << real_power.data[power.int_regs.A][0] << "\t"
			<< real_power.data[power.int_regs.B][0] << "\t"
			<< real_power.data[power.int_regs.C][0] << '\n';
		std::cout << "Всего " << power.data[power.int_regs.total][0] << " kW\n";
		std::cout << "Min = " << power.data[power.int_regs.min][0] << " kW\n";
		std::cout << "Max = " << power.data[power.int_regs.max][0] << " kW\n";
	}
	else
	{
		float temp = modbus_get_float( (uint16_t*) power.data[power.int_regs.min]);
		std::cout << "Mim = " << temp << " kW\n";
	}
#endif // DEBUG
}




