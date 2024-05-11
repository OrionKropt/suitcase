#include "data_logger.h"

Data_logger::Data_logger() : ctx(nullptr)
{
}

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
			std::cout << "Не удалось создать контекст Modbus!\n";
		#endif // DEBUG

		return;
	}
	else
	{
		#ifdef DEBUG
		std::cout << "Контекст Modbus создан!\n";
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
		std::cout << "Не удалось подключиться к устройству!\n";
		#endif // DEBUG

		modbus_free(ctx);
		return;
	}
	else
	{
		#ifdef DEBUG
		std::cout << "Успешное подключение!\n";
		#endif // DEBUG
	}

	is_float = false;
}

auto Data_logger::read_data() -> void
{
	read_power();
}

auto Data_logger::write_data() -> void
{
	
#ifdef DEBUG
	std::cout << "\nPower\n";
	std::cout << power.real.name << ", kW" << '\n';
	std::cout << "A\tB\tC\n";
	if (!is_float)
	{
		std::cout << power.real.B[0] << "\t"
			<< power.real.B[0] << "\t"
			<< power.real.C[0] << '\n';
		std::cout << "Всего " << power.real.total[0] << " kW\n";
		std::cout << "Min = " << power.real.min[0] << " kW\n";
		std::cout << "Max = " << power.real.max[0] << " kW\n";
	}
	else
	{
		float temp = modbus_get_float(power.real.min);
		std::cout << "Mim = " << temp << " kW\n";
	}
#endif // DEBUG

}

auto Data_logger::read_power() -> int
{
	if (-1 == read_real_power(is_float))
	{
		return -1;
	}



}

auto Data_logger::register_read_error(uint16_t reg) -> void
{
#ifdef DEBUG
	std::cout << "Ошибка чтения регистров\n";
	std::cout << "Регистр " << reg << '\n';
#endif // DEBUG

}

auto Data_logger::read_real_power(bool is_float) -> int
{
	if (!is_float)
	{	
		// ---------INT----------

		// phase reading

		if (-1 == modbus_read_registers(ctx, REAL_POWER_A_INT, 3, buf))
		{
			register_read_error(REAL_POWER_A_INT);
			return -1;
		}
		power.real.A[0] = buf[0];
		power.real.B[0] = buf[1];
		power.real.C[0] = buf[2];


		if (-1 == modbus_read_registers(ctx, REAL_POWER_TOTAL_INT, 1, buf))
		{
			register_read_error(REAL_POWER_TOTAL_INT);
			return -1;
		}
		power.real.total[0] = buf[0];

		// min max
		if (-1 == modbus_read_registers(ctx, REAL_POWER_TOTAL_MIN_INT, 1, buf))
		{
			register_read_error(REAL_POWER_TOTAL_INT);
			return -1;
		}
		power.real.min[0] = buf[0];
		if (-1 == modbus_read_registers(ctx, REAL_POWER_TOTAL_MAX_INT, 1, buf))
		{
			register_read_error(REAL_POWER_TOTAL_INT);
			return -1;
		}

		power.real.max[0] = buf[0];
		// ---------INT----------
		return 0;
	}
	else
	{
		// min max
		if (-1 == modbus_read_registers(ctx, REAL_POWER_TOTAL_MIN_FLOAT, 2, buf))
		{
			register_read_error(REAL_POWER_TOTAL_INT);
			return -1;
		}
		memcpy_s(power.real.min, sizeof(uint16_t) * 2, buf, sizeof(uint16_t) * 2);
		return 0;
	}
}


