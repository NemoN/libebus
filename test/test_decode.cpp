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
	Decode* help_dec = NULL;
	Encode* help_enc = NULL;

	std::cout << std::endl;
/*
	// HEX
	{
		const char* hex[] = {"53706569636865722020"};
		for (size_t i = 0; i < sizeof(hex)/sizeof(hex[0]); i++) {
			help_dec = new DecodeHEX(hex[i]);
			std::cout << "DecodeHEX: " << std::setw(20) << hex[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* hex[] = {"53 70 65 69 63 68 65 72 20 20"};
		for (size_t i = 0; i < sizeof(hex)/sizeof(hex[0]); i++) {
			help_enc = new EncodeHEX(hex[i]);
			std::cout << "EncodeHEX: " << std::setw(20) << hex[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// UCH
	{
		const char* uch[] = {"00", "01", "32", "ce", "aa", "ff"};
		for (size_t i = 0; i < sizeof(uch)/sizeof(uch[0]); i++) {
			help_dec = new DecodeUCH(uch[i], "1.0");
			std::cout << "DecodeUCH: " << std::setw(20) << uch[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* uch[] = {"0", "1", "50", "206", "170", "255"};
		for (size_t i = 0; i < sizeof(uch)/sizeof(uch[0]); i++) {
			help_enc = new EncodeUCH(uch[i], "1.0");
			std::cout << "EncodeUCH: " << std::setw(20) << uch[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}
*/
	// SCH
	{
		const char* sch[] = {"00", "01", "32", "80", "aa", "ff"};
		for (size_t i = 0; i < sizeof(sch)/sizeof(sch[0]); i++) {
			help_dec = new DecodeSCH(sch[i], "1.0");
			std::cout << "DecodeSCH: " << std::setw(20) << sch[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* sch[] = {"0", "1", "50", "-128", "-86", "-1"};
		for (size_t i = 0; i < sizeof(sch)/sizeof(sch[0]); i++) {
			help_enc = new EncodeSCH(sch[i], "1.0");
			std::cout << "EncodeSCH: " << std::setw(20) << sch[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}
/*
	// STR
	{
		const char* str[] = {"53706569636865722020"};
		for (size_t i = 0; i < sizeof(str)/sizeof(str[0]); i++) {
			help_dec = new DecodeSTR(str[i]);
			std::cout << "DecodeSTR: " << std::setw(20) << str[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* str[] = {"Speicher  "};
		for (size_t i = 0; i < sizeof(str)/sizeof(str[0]); i++) {
			help_enc = new EncodeSTR(str[i]);
			std::cout << "EncodeSTR: " << std::setw(20) << str[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// BCD
	{
		const char* bcd[] = {"00", "01", "02", "03", "12", "99"};
		for (size_t i = 0; i < sizeof(bcd)/sizeof(bcd[0]); i++) {
			help_dec = new DecodeBCD(bcd[i], "1.0");
			std::cout << "DecodeBCD: " << std::setw(20) << bcd[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* bcd[] = {"0", "1", "2", "3", "12", "99"};
		for (size_t i = 0; i < sizeof(bcd)/sizeof(bcd[0]); i++) {
			help_enc = new EncodeBCD(bcd[i], "1.0");
			std::cout << "EncodeBCD: " << std::setw(20) << bcd[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// D1B
	{
		const char* d1b[] = {"00", "01", "7f", "81", "80"};
		for (size_t i = 0; i < sizeof(d1b)/sizeof(d1b[0]); i++) {
			help_dec = new DecodeD1B(d1b[i], "1.0");
			std::cout << "DecodeD1B: " << std::setw(20) << d1b[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* d1b[] = {"00", "01", "127", "-127", "-128"};
		for (size_t i = 0; i < sizeof(d1b)/sizeof(d1b[0]); i++) {
			help_enc = new EncodeD1B(d1b[i], "1.0");
			std::cout << "EncodeD1B: " << std::setw(20) << d1b[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// D1C
	{
		const char* d1c[] = {"00", "64", "c8"};
		for (size_t i = 0; i < sizeof(d1c)/sizeof(d1c[0]); i++) {
			help_dec = new DecodeD1C(d1c[i], "1.0");
			std::cout << "DecodeD1C: " << std::setw(20) << d1c[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* d1c[] = {"0", "50", "100"};
		for (size_t i = 0; i < sizeof(d1c)/sizeof(d1c[0]); i++) {
			help_enc = new EncodeD1C(d1c[i], "1.0");
			std::cout << "EncodeD1C: " << std::setw(20) << d1c[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// D2B
	{
		const char* d2b[] = {"0000", "0100", "ffff", "00ff", "0080", "0180", "ff7f"};
		for (size_t i = 0; i < sizeof(d2b)/sizeof(d2b[0]); i++) {
			help_dec = new DecodeD2B(d2b[i], "1.0");
			std::cout << "DecodeD2B: " << std::setw(20) << d2b[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* d2b[] = {"0", "0.00390625", "-0.00390625", "-1", "-128", "-127.99609375", "127.99609375"};
		for (size_t i = 0; i < sizeof(d2b)/sizeof(d2b[0]); i++) {
			help_enc = new EncodeD2B(d2b[i], "1.0");
			std::cout << "EncodeD2B: " << std::setw(20) << d2b[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}

	// D2C
	{
		const char* d2c[] = {"0000", "0100", "ffff", "f0ff", "0080", "0180", "ff7f"};
		for (size_t i = 0; i < sizeof(d2c)/sizeof(d2c[0]); i++) {
			help_dec = new DecodeD2C(d2c[i], "1.0");
			std::cout << "DecodeD2C: " << std::setw(20) << d2c[i] << " = " << help_dec->decode() << std::endl;

			delete help_dec;
		}

		std::cout << std::endl;
	}
	{
		const char* d2c[] = {"0", "0.0625", "-0.0625", "-1", "-2048", "-2047.9375", "2047.9375"};
		for (size_t i = 0; i < sizeof(d2c)/sizeof(d2c[0]); i++) {
			help_enc = new EncodeD2C(d2c[i], "1.0");
			std::cout << "EncodeD2C: " << std::setw(20) << d2c[i] << " = " << help_enc->encode() << std::endl;

			delete help_enc;
		}

		std::cout << std::endl;
	}
*/
	return 0;
}


