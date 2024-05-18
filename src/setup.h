#pragma once

#include <modbus.h>




class Setup
{
public:
	uint16_t mode[2];                        // 0 = IEC, 1 - IEEE
	
	int16_t scale_i[2]{ 0 };                     // current, power of 10
	int16_t scale_v[2]{ 0 };                     // voltage, power of 10
	int16_t scale_w[2]{ 0 };                     // power, power of 10
	int16_t scale_e[2]{ 0 };                     // energy, power of 10

	
	// This combination timer counts the total time for which the absolute current on at least one phase is > 0.1Amp.
	
	uint16_t usage_hours[2]{ 0 };                 // <= 0
	uint16_t usage_minutes[2]{ 0 };               // 0.0 - 59.0   

	char port[10];                                // \COMN, N - COM number
	uint16_t port_speed;                          // bods, 19200 by default 
	char parity;                                  // N - none, E - even, O - odd 
	uint16_t data_bits;                           // number of bits of data, the allowed values are 5, 6, 7 and 8
	uint16_t stop_bits;                           // bits of stop, the allowed values are 1 and 2
	uint16_t slave_id;                            // slave ID in RTU
};