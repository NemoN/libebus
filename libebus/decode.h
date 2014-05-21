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
	Decode(const std::string& data, const std::string& factor = "");
	virtual ~Decode() {}

	virtual std::string decode() = 0;

protected:
	std::string m_data;
	float m_factor;

};


class DecodeHEX : public Decode
{

public:
	DecodeHEX(const std::string& data) : Decode(data) {}
	~DecodeHEX() {}

	std::string decode();

};

class DecodeUCH : public Decode
{

public:
	DecodeUCH(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeUCH() {}

	std::string decode();

};

class DecodeSCH : public Decode
{

public:
	DecodeSCH(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeSCH() {}

	std::string decode();

};

class DecodeUIN : public Decode
{

public:
	DecodeUIN(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeUIN() {}

	std::string decode();

};

class DecodeSIN : public Decode
{

public:
	DecodeSIN(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeSIN() {}

	std::string decode();

};

class DecodeULG : public Decode
{

public:
	DecodeULG(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeULG() {}

	std::string decode();

};

class DecodeSLG : public Decode
{

public:
	DecodeSLG(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeSLG() {}

	std::string decode();

};

class DecodeFLT : public Decode
{

public:
	DecodeFLT(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeFLT() {}

	std::string decode();

};

class DecodeSTR : public Decode
{

public:
	DecodeSTR(std::string data) : Decode(data) {}
	~DecodeSTR() {}

	std::string decode();

};

class DecodeBCD : public Decode
{

public:
	DecodeBCD(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeBCD() {}

	std::string decode();

};

class DecodeD1B : public Decode
{

public:
	DecodeD1B(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeD1B() {}

	std::string decode();

};

class DecodeD1C : public Decode
{

public:
	DecodeD1C(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeD1C() {}

	std::string decode();

};

class DecodeD2B : public Decode
{

public:
	DecodeD2B(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeD2B() {}

	std::string decode();

};

class DecodeD2C : public Decode
{

public:
	DecodeD2C(const std::string& data, const std::string& factor)
		: Decode(data, factor) {}
	~DecodeD2C() {}

	std::string decode();

};

class DecodeBDA : public Decode
{

public:
	DecodeBDA(const std::string& data) : Decode(data) {}
	~DecodeBDA() {}

	std::string decode();

};

class DecodeHDA : public Decode
{

public:
	DecodeHDA(const std::string& data) : Decode(data) {}
	~DecodeHDA() {}

	std::string decode();

};

class DecodeBTI : public Decode
{

public:
	DecodeBTI(const std::string& data) : Decode(data) {}
	~DecodeBTI() {}

	std::string decode();

};

class DecodeHTI : public Decode
{

public:
	DecodeHTI(const std::string& data) : Decode(data) {}
	~DecodeHTI() {}

	std::string decode();

};

class DecodeBDY : public Decode
{

public:
	DecodeBDY(const std::string& data) : Decode(data) {}
	~DecodeBDY() {}

	std::string decode();

};

class DecodeHDY : public Decode
{

public:
	DecodeHDY(const std::string& data) : Decode(data) {}
	~DecodeHDY() {}

	std::string decode();

};

class DecodeTTM : public Decode
{

public:
	DecodeTTM(const std::string& data) : Decode(data) {}
	~DecodeTTM() {}

	std::string decode();

};


class Encode
{

public:
	Encode(const std::string& data, const std::string& factor = "");
	virtual ~Encode() {}

	virtual std::string encode() = 0;

protected:
	std::string m_data;
	float m_factor;

};


class EncodeHEX : public Encode
{

public:
	EncodeHEX(const std::string& data) : Encode(data) {}
	~EncodeHEX() {}

	std::string encode();

};

class EncodeUCH : public Encode
{

public:
	EncodeUCH(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeUCH() {}

	std::string encode();

};

class EncodeSCH : public Encode
{

public:
	EncodeSCH(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeSCH() {}

	std::string encode();

};

class EncodeUIN : public Encode
{

public:
	EncodeUIN(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeUIN() {}

	std::string encode();

};

class EncodeSIN : public Encode
{

public:
	EncodeSIN(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeSIN() {}

	std::string encode();

};

class EncodeULG : public Encode
{

public:
	EncodeULG(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeULG() {}

	std::string encode();

};

class EncodeSLG : public Encode
{

public:
	EncodeSLG(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeSLG() {}

	std::string encode();

};

class EncodeFLT : public Encode
{

public:
	EncodeFLT(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeFLT() {}

	std::string encode();

};

class EncodeSTR : public Encode
{

public:
	EncodeSTR(const std::string& data) : Encode(data) {}
	~EncodeSTR() {}

	std::string encode();

};

class EncodeBCD : public Encode
{

public:
	EncodeBCD(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeBCD() {}

	std::string encode();

};

class EncodeD1B : public Encode
{

public:
	EncodeD1B(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeD1B() {}

	std::string encode();

};

class EncodeD1C : public Encode
{

public:
	EncodeD1C(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeD1C() {}

	std::string encode();

};

class EncodeD2B : public Encode
{

public:
	EncodeD2B(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeD2B() {}

	std::string encode();

};

class EncodeD2C : public Encode
{

public:
	EncodeD2C(const std::string& data, const std::string& factor)
		: Encode(data, factor) {}
	~EncodeD2C() {}

	std::string encode();

};

class EncodeBDA : public Encode
{

public:
	EncodeBDA(const std::string& data) : Encode(data) {}
	~EncodeBDA() {}

	std::string encode();

};

class EncodeHDA : public Encode
{

public:
	EncodeHDA(const std::string& data) : Encode(data) {}
	~EncodeHDA() {}

	std::string encode();

};

class EncodeBTI : public Encode
{

public:
	EncodeBTI(const std::string& data) : Encode(data) {}
	~EncodeBTI() {}

	std::string encode();

};

class EncodeHTI : public Encode
{

public:
	EncodeHTI(const std::string& data) : Encode(data) {}
	~EncodeHTI() {}

	std::string encode();

};

class EncodeBDY : public Encode
{

public:
	EncodeBDY(const std::string& data) : Encode(data) {}
	~EncodeBDY() {}

	std::string encode();

};

class EncodeHDY : public Encode
{

public:
	EncodeHDY(const std::string& data) : Encode(data) {}
	~EncodeHDY() {}

	std::string encode();

};

class EncodeTTM : public Encode
{

public:
	EncodeTTM(const std::string& data) : Encode(data) {}
	~EncodeTTM() {}

	std::string encode();

};

std::string esc(const std::string& data);
std::string unesc(const std::string& data);

unsigned char calc_crc_byte(unsigned char byte, const unsigned char init_crc);
std::string calc_crc(const std::string& data);


} //namespace

#endif // LIBEBUS_DECODE_H_
