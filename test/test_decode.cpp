/*
 * Copyright (C) Roland Jax 2014 <roland.jax@liwest.at>
 *
 * This file is part of libebus.
 *
 * libebus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libebus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libebus. If not, see http://www.gnu.org/licenses/.
 */

#include "libebus.h"
#include <iostream>
#include <iomanip>


using namespace libebus;


int main()
{
	// Test decode
	Decode* help_dec = NULL;

	// Test encode
	Encode* help_enc = NULL;

	// BCD
	const char* bcd[] = {"00", "01", "02", "12", "99", "100", "-1"};
	for (size_t i = 0; i < sizeof(bcd)/sizeof(bcd[0]); i++) {
		help_enc = new EncodeBCD(bcd[i], "1.0");
		std::cout << "BCD: " << std::setw(14) << bcd[i] << " = " << help_enc->encode() << std::endl;

		delete help_enc;
	}

	std::cout << std::endl;

	// D1B
	const char* d1b[] = {"00", "01", "127", "-127", "-128"};
	for (size_t i = 0; i < sizeof(d1b)/sizeof(d1b[0]); i++) {
		help_enc = new EncodeD1B(d1b[i], "1.0");
		std::cout << "D1B: " << std::setw(14) << d1b[i] << " = " << help_enc->encode() << std::endl;

		delete help_enc;
	}

	std::cout << std::endl;

	// D1C
	const char* d1c[] = {"00", "50", "100", "127", "-127"};
	for (size_t i = 0; i < sizeof(d1c)/sizeof(d1c[0]); i++) {
		help_enc = new EncodeD1C(d1c[i], "1.0");
		std::cout << "D1C: " << std::setw(14) << d1c[i] << " = " << help_enc->encode() << std::endl;

		delete help_enc;
	}

	std::cout << std::endl;

	// D2B
	const char* d2b[] = {"0", "0.00390625", "-0.00390625", "-1.0", "-128.0", "-127.99609375", "127.99609375"};
	for (size_t i = 0; i < sizeof(d2b)/sizeof(d2b[0]); i++) {
		help_enc = new EncodeD2B(d2b[i], "1.0");
		std::cout << "D2B: " << std::setw(14) << d2b[i] << " = " << help_enc->encode() << std::endl;

		delete help_enc;
	}

	std::cout << std::endl;

	// D2C
	const char* d2c[] = {"0", "0.0625", "-0.0625", "-1.0", "-2048.0", "-2047.9375", "2047.9375"};
	for (size_t i = 0; i < sizeof(d2c)/sizeof(d2c[0]); i++) {
		help_enc = new EncodeD2C(d2c[i], "1.0");
		std::cout << "D2C: " << std::setw(14) << d2c[i] << " = " << help_enc->encode() << std::endl;

		delete help_enc;
	}

	return 0;
}


