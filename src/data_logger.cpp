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
}

auto Data_logger::read_data() -> void
{

}

auto Data_logger::read_power() -> int
{
	
	return 0;
}
