/*
 * Copyright (C) Roland Jax 2014 <roland.jax@liwest.at>
 * crc calculations from http://www.mikrocontroller.net/topic/75698
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

#include "decode.h"
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace libebus
{


std::string DecodeHEX::decode()
{
	std::ostringstream result;
	
	for (size_t i = 0; i < m_data.length()/2; i++)
		result << m_data.substr(i*2, 2) << " ";
		
	return result.str().substr(0, result.str().length()-1);
}

std::string DecodeUCH::decode()
{
	std::ostringstream result;
	result << strtoul(m_data.c_str(), NULL, 16);

	return result.str();
}

std::string DecodeSCH::decode()
{
	std::ostringstream result;
	short number = strtol(m_data.c_str(), NULL, 16);
	if (number > 0x7F)
		result << (number - 0x100);
	else
		result << number;

	return result.str();
}

std::string DecodeUIN::decode()
{
	std::ostringstream result;
	result << static_cast<unsigned short>(strtoul(m_data.c_str(), NULL, 16));

	return result.str();
}

std::string DecodeSIN::decode()
{
	std::ostringstream result;
	result << static_cast<short>(strtol(m_data.c_str(), NULL, 16));

	return result.str();
}

std::string DecodeULG::decode()
{
	std::ostringstream result;
	result << static_cast<unsigned int>(strtoul(m_data.c_str(), NULL, 16));

	return result.str();
}

std::string DecodeSLG::decode()
{
	std::ostringstream result;
	result << static_cast<int>(strtol(m_data.c_str(), NULL, 16));

	return result.str();
}

std::string DecodeFLT::decode()
{
	std::ostringstream result;
	result << static_cast<float>(strtod(m_data.c_str(), NULL));

	return result.str();
}


std::string esc(const std::string& data)
{
	std::string escaped;

	for (size_t i = 0; i < data.size(); i = i + 2) {
		long test = strtol(data.substr(i, 2).c_str(), NULL, 16);
		
		if (test == 0xAA)
			escaped += "A901";
		else if (test == 0xA9)
			escaped += "A900";
		else
			escaped += data.substr(i, 2).c_str();
	}

	return escaped;
}

std::string unesc(const std::string& data)
{
	std::string unescaped;
	bool found = false;

	for (size_t i = 0; i < data.size(); i = i + 2) {
		long test = strtol(data.substr(i,2).c_str(), NULL, 16);
		
		if (test == 0xA9) {
			found = true;

		} else if (found == true) {
			if (test == 0x01)
				unescaped += "AA";
			else
				unescaped += "A9";

			found = false;
		} else {
			unescaped += data.substr(i,2).c_str();
		}
	}

	return unescaped;
}


unsigned char calc_crc_byte(unsigned char byte, const unsigned char init_crc)
{
	unsigned char crc, polynom;

	crc = init_crc;

	for (int i = 0; i < 8; i++) {

		if (crc & 0x80)
			polynom = (unsigned char) 0x9B;
		else
			polynom = (unsigned char) 0;

		crc = (unsigned char) ((crc & ~0x80) << 1);

		if (byte & 0x80)
			crc = (unsigned char) (crc | 1);

		crc = (unsigned char) (crc ^ polynom);
		byte = (unsigned char) (byte << 1);
	}
	
	return crc;
}

std::string calc_crc(const std::string& data)
{
	unsigned char crc = 0;

	for (size_t i = 0 ; i < data.size() ; i = i + 2)
		crc = calc_crc_byte(strtol(data.substr(i, 2).c_str(), NULL, 16), crc);

	std::stringstream sstr;
	sstr << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned>(crc);
	
	return sstr.str();
}


} //namespace

