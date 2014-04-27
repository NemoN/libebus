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

#ifndef LIBEBUS_DECODE_H_
#define LIBEBUS_DECODE_H_

#include <string>

namespace libebus
{


class Decode
{

public:
	Decode(std::string data) : m_data(data) {}
	virtual ~Decode() {}

	virtual std::string decode() = 0;

protected:
	std::string m_data;

};

class DecodeHEX : public Decode
{

public:
	DecodeHEX(std::string data) : Decode(data) {}
	~DecodeHEX() {}

	std::string decode();

};

class DecodeUCH : public Decode
{

public:
	DecodeUCH(std::string data) : Decode(data) {}
	~DecodeUCH() {}

	std::string decode();

};

class DecodeSCH : public Decode
{

public:
	DecodeSCH(std::string data) : Decode(data) {}
	~DecodeSCH() {}

	std::string decode();

};

class DecodeUIN : public Decode
{

public:
	DecodeUIN(std::string data) : Decode(data) {}
	~DecodeUIN() {}

	std::string decode();

};

class DecodeSIN : public Decode
{

public:
	DecodeSIN(std::string data) : Decode(data) {}
	~DecodeSIN() {}

	std::string decode();

};

class DecodeULG : public Decode
{

public:
	DecodeULG(std::string data) : Decode(data) {}
	~DecodeULG() {}

	std::string decode();

};

class DecodeSLG : public Decode
{

public:
	DecodeSLG(std::string data) : Decode(data) {}
	~DecodeSLG() {}

	std::string decode();

};

class DecodeFLT : public Decode
{

public:
	DecodeFLT(std::string data) : Decode(data) {}
	~DecodeFLT() {}

	std::string decode();

};


std::string esc(const std::string& data);
std::string unesc(const std::string& data);

unsigned char calc_crc_byte(unsigned char byte, const unsigned char init_crc);
std::string calc_crc(const std::string& data);


} //namespace

#endif // LIBEBUS_DECODE_H_
