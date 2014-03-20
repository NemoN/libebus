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

#include "bus.hpp"
#include <iostream>
#include <iomanip>

namespace libebus
{


Bus::Bus(const std::string deviceName, const std::string dumpFile, const long dumpSize, const bool dumpState)
	: m_deviceName(deviceName), m_connected(false), m_dumpFile(dumpFile), m_dumpSize(dumpSize), m_dumpState(dumpState)
{
	m_port = new Port(m_deviceName);
	m_dump = new Dump(m_dumpFile, m_dumpSize);
}

Bus::~Bus()
{
	if (isConnected() == true)
		disconnect();

	delete m_port;
	delete m_dump;
}

void Bus::connect()
{
	m_port->open();
	m_connected = m_port->isOpen();
}

void Bus::disconnect()
{
	if (m_connected == true) {
		m_port->close();
		m_connected = m_port->isOpen();
	}
}

void Bus::printBytes()
{
	unsigned char byte;
	ssize_t bytes_read;
	
	bytes_read = m_port->recv();

	for (int i = 0; i < bytes_read; i++) {
		byte = m_port->byte();
		std::cout << std::hex << std::setw(2) << std::setfill('0')
			  << static_cast<unsigned>(byte);
		if (byte == 0xAA)
			std::cout << std::endl;
	}
}

bool Bus::waitData()
{
	unsigned char byte;
	ssize_t bytes_read;
	bool result = false;

	// wait for new data
	bytes_read = m_port->recv();

	for (int i = 0; i < bytes_read; i++) {
		
		// fetch next byte
		byte = m_port->byte();

		if (m_dumpState == true)
			m_dump->write((const char*) &byte);

		if (byte != 0xAA)
			m_sstr << std::hex << std::setw(2) << std::setfill('0')
			       << static_cast<unsigned>(byte);

		if (byte == 0xAA && m_sstr.str().empty() == false) {
			m_cycBuffer.push(m_sstr.str());
			m_sstr.str(std::string());
			result = true;
		}
	}

	return result;
}

std::string Bus::cycData()
{
	std::string data;

	if (m_cycBuffer.empty() == false) {
		data = m_cycBuffer.front();
		m_cycBuffer.pop();
	}
	
	return data;
}

//~ bool getBus(void);
//~ {
	//~ 
//~ }
//~ 
//~ void freeBus(void);
//~ {
	//~ 
//~ }


} //namespace

