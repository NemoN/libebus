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
#include "decode.hpp"
#include <iostream>
#include <iomanip>

namespace libebus
{


BusCommand::BusCommand(const std::string type, const std::string data) : m_type(type)
{
	// esc
	m_data = esc(data);
	
	// crc + esc
	std::string crc = calc_crc(m_data);
	//~ const char crc = calc_crc((const unsigned char*)(m_data.c_str()), m_data.size());
	m_data += esc(crc);
}


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

void Bus::printBytes() const
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

int Bus::proceed()
{
	unsigned char byte;
	ssize_t bytes_recv;
	int result = 99;

	// fetch new message and get bus
	if (m_cycBuffer.size() == 0 && m_sendBuffer.size() != 0) {
		BusCommand* busCommand = m_sendBuffer.front();
		return getBus(busCommand->getQQ());
	}

	// wait for new data
	bytes_recv = m_port->recv();

	for (int i = 0; i < bytes_recv; i++) {
		
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
			result = 2;
		}
	}

	return result;
}

std::string Bus::getCycData()
{
	std::string data;

	if (m_cycBuffer.empty() == false) {
		data = m_cycBuffer.front();
		m_cycBuffer.pop();
	}
	
	return data;
}

BusCommand* Bus::recvCommand()
{
	BusCommand* busCommand = m_recvBuffer.front();
	m_recvBuffer.pop();
	return busCommand;
}

int Bus::getBus(unsigned char byte)
{
	ssize_t bytes_sent, bytes_recv;
	//~ struct timeval tact, tlast, tdiff;

	//~ // start time
	//~ gettimeofday(&tlast, NULL);

	// send QQ
	bytes_sent = m_port->send(&byte, 1);
	if (bytes_sent <= 0)
		return -1;

	//~ // act time
	//~ gettimeofday(&tact, NULL);
	//~ eb_diff_time(&tact, &tlast, &tdiff);

	//~ // wait ~4200 usec
	//~ if (4000.0 - tdiff.tv_usec > 0.0 && 4000.0 - tdiff.tv_usec <= 4000.0)
		//~ usleep(4000.0 - tdiff.tv_usec);
	//~ else 
		//~ return -2;

	// receive 1 byte - must be QQ
	bytes_recv = m_port->recv();	
	
	for (int i = 0; i < bytes_recv; i++) {
		
		// fetch next byte
		byte = m_port->byte();

		if (m_dumpState == true)
			m_dump->write((const char*) &byte);

		// compare sent and received byte
		if (bytes_recv == 1 && byte == 0xFF)
			return 1;

	}

	return -1;
}

int Bus::sendCommand()
{
	BusCommand* busCommand = m_sendBuffer.front();
	m_sendBuffer.pop();

	busCommand->setResult(busCommand->getData());
	m_recvBuffer.push(busCommand);
	return 0;
}


} //namespace

