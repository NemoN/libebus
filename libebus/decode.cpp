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

namespace libebus
{


Decode::Decode(const std::string& data, const std::string& factor)
	: m_data(data)
{
	if ((factor.find_first_not_of("0123456789.") == std::string::npos) == true)
		m_factor = static_cast<float>(strtod(factor.c_str(), NULL));
	else
		m_factor = 1.0;
}


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
	unsigned char src = strtol(m_data.c_str(), NULL, 16);

	if (src > 0x7F)
		result << (src - 0x100);
	else
		result << src;

	return result.str();
}

std::string DecodeUIN::decode()
{
	std::ostringstream result;
	result << static_cast<unsigned short>(strtoul(m_data.c_str(), NULL, 16) * m_factor);

	return result.str();
}

std::string DecodeSIN::decode()
{
	std::ostringstream result;
	result << static_cast<short>(strtol(m_data.c_str(), NULL, 16) * m_factor);

	return result.str();
}

std::string DecodeULG::decode()
{
	std::ostringstream result;
	result << static_cast<unsigned int>(strtoul(m_data.c_str(), NULL, 16) * m_factor);

	return result.str();
}

std::string DecodeSLG::decode()
{
	std::ostringstream result;
	result << static_cast<int>(strtol(m_data.c_str(), NULL, 16) * m_factor);

	return result.str();
}

std::string DecodeFLT::decode()
{
	std::ostringstream result;
	result << static_cast<float>(strtol(m_data.c_str(), NULL, 16) * m_factor);

	return result.str();
}

std::string DecodeSTR::decode()
{
	std::ostringstream result;

	for (size_t i = 0; i < m_data.length()/2; i++) {
		char tmp = static_cast<char>(strtol(m_data.substr(i*2, 2).c_str(), NULL, 16));
		if (tmp == 0x00) tmp = 0x20;
		result << tmp;
	}

	return result.str().substr(0, result.str().length()-1);
}

std::string DecodeBCD::decode()
{
	std::ostringstream result;
	unsigned char src = strtol(m_data.c_str(), NULL, 16);

	if ((src & 0x0F) > 0x09 || ((src >> 4) & 0x0F) > 0x09)
		result << static_cast<short>(0xFF * m_factor);
	else
		result << static_cast<short>(( ( ((src & 0xF0) >> 4) * 10) + (src & 0x0F) ) * m_factor);

	return result.str();
}

std::string DecodeD1B::decode()
{
	std::ostringstream result;
	unsigned char src = strtol(m_data.c_str(), NULL, 16);

	if ((src & 0x80) == 0x80)
		result << static_cast<short>((- ( ((unsigned char) (~ src)) + 1) ) * m_factor);
	else
		result << static_cast<short>(src * m_factor);

	return result.str();
}

std::string DecodeD1C::decode()
{
	std::ostringstream result;
	unsigned char src = strtol(m_data.c_str(), NULL, 16);

	if (src > 0xC8)
		result << static_cast<float>(0xFF * m_factor);
	else
		result << static_cast<float>((src / 2.0) * m_factor);

	return result.str();
}

std::string DecodeD2B::decode()
{
	std::ostringstream result;
	unsigned char src_lsb = static_cast<char>(strtol(m_data.substr(0, 2).c_str(), NULL, 16));
	unsigned char src_msb = static_cast<char>(strtol(m_data.substr(2, 2).c_str(), NULL, 16));

	if ((src_msb & 0x80) == 0x80)
		result << static_cast<float>
			((- ( ((unsigned char) (~ src_msb)) +
			 (  ( ((unsigned char) (~ src_lsb)) + 1) / 256.0) ) ) * m_factor);

	else
		result << static_cast<float>((src_msb + (src_lsb / 256.0)) * m_factor);

	return result.str();
}

std::string DecodeD2C::decode()
{
	std::ostringstream result;
	unsigned char src_lsb = static_cast<char>(strtol(m_data.substr(0, 2).c_str(), NULL, 16));
	unsigned char src_msb = static_cast<char>(strtol(m_data.substr(2, 2).c_str(), NULL, 16));

	if ((src_msb & 0x80) == 0x80)
		result << static_cast<float>
		((- ( ( ( ((unsigned char) (~ src_msb)) * 16.0) ) +
		      ( ( ((unsigned char) (~ src_lsb)) & 0xF0) >> 4) +
		    ( ( ( ((unsigned char) (~ src_lsb)) & 0x0F) +1 ) / 16.0) ) ) * m_factor);

	else
		result << static_cast<float>(( (src_msb * 16.0) + ((src_lsb & 0xF0) >> 4) +
					      ((src_lsb & 0x0F) / 16.0) ) * m_factor);

	return result.str();
}

std::string DecodeHDA::decode()
{
	std::ostringstream result;
	short dd = static_cast<short>(strtol(m_data.substr(0, 2).c_str(), NULL, 16));
	short mm = static_cast<short>(strtol(m_data.substr(2, 2).c_str(), NULL, 16));
	short yy = static_cast<short>(strtol(m_data.substr(4, 2).c_str(), NULL, 16));

	result << std::setw(2) << std::setfill('0') << dd << "."
	       << std::setw(2) << std::setfill('0') << mm << "."
	       << yy+2000;

	return result.str();
}

std::string DecodeHTI::decode()
{
	std::ostringstream result;
	short hh = static_cast<short>(strtol(m_data.substr(0, 2).c_str(), NULL, 16));
	short mm = static_cast<short>(strtol(m_data.substr(2, 2).c_str(), NULL, 16));
	short ss = static_cast<short>(strtol(m_data.substr(4, 2).c_str(), NULL, 16));

	result << std::setw(2) << std::setfill('0') << hh << ":"
	       << std::setw(2) << std::setfill('0') << mm << ":"
	       << std::setw(2) << std::setfill('0') << ss;

	return result.str();
}

std::string DecodeHDY::decode()
{
	const char *days[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "Err"};

	std::ostringstream result;
	short day = static_cast<short>(strtol(m_data.c_str(), NULL, 16));

	if (day < 0 || day > 6)
		day = 7;

	result << days[day];

	return result.str();
}

std::string DecodeTTM::decode()
{
	std::ostringstream result;
	short hh = static_cast<short>(strtol(m_data.c_str(), NULL, 16)) / 6;
	short mm = static_cast<short>(strtol(m_data.c_str(), NULL, 16)) % 6 * 10;

	result << std::setw(2) << std::setfill('0') << hh << ":"
	       << std::setw(2) << std::setfill('0') << mm;

	return result.str();
}


Encode::Encode(const std::string& data, const std::string& factor)
	: m_data(data)
{
	if ((factor.find_first_not_of("0123456789.") == std::string::npos) == true)
		m_factor = static_cast<float>(strtod(factor.c_str(), NULL));
	else
		m_factor = 1.0;
}


std::string EncodeD1C::encode()
{
	std::ostringstream result;
	float src = static_cast<float>(strtod(m_data.c_str(), NULL) / m_factor);

	if (src < 0.0 || src > 100.0)
		result << std::setw(2) << std::hex << std::setfill('0')
		       << static_cast<unsigned>(0xFF);
	else
		result << std::setw(2) << std::hex << std::setfill('0')
		       << static_cast<unsigned>(src * 2.0);

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

