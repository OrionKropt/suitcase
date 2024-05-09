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
	void init();

	void read_data();

private:
	int read_power();

	Setup setup;
	Power power;
	Current current;
	Voltage voltage;

	modbus_t* ctx;
};