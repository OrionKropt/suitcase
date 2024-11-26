#pragma once

#include <modbus.h>

class Setup
{
public:
	Setup() : mode(0),
		scale_i(0),
		scale_v(0),
		scale_w(0),
		scale_e(0),
		usage_hours(0),
		usage_minutes(0),
		port_speed(0),
		parity(0),
		data_bits(0),
		stop_bits(0),
		slave_id(0),
		port{ 0 },
		scale_pf(0.0001f)                         // Constantat from the manual
	{}
	uint16_t mode;                                // 0 = IEC, 1 - IEEE
	
	float scale_i;                                // current
	float scale_v;                                // voltage
	float scale_w;                                // power
	float scale_e;                                // energy
	const float scale_pf;                         // power factor

	
	// This combination timer counts the total time for which the absolute current on at least one phase is > 0.1Amp.
	
	uint16_t usage_hours;                         // <= 0
	uint16_t usage_minutes;                       // 0.0 - 59.0   

	char port[8];                                 // \COMN, N - COM number
	uint16_t port_speed;                          // bods, 19200 by default 
	char parity;                                  // N - none, E - even, O - odd 
	uint16_t data_bits;                           // number of bits of data, the allowed values are 5, 6, 7 and 8
	uint16_t stop_bits;                           // bits of stop, the allowed values are 1 and 2
	uint16_t slave_id;                            // slave ID in RTU
};