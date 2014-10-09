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

#ifndef LIBEBUS_BUS_H_
#define LIBEBUS_BUS_H_

#include "port.h"
#include "dump.h"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <queue>

namespace libebus
{


class BusCommand
{

public:
	BusCommand(const std::string type, const std::string command);

	std::string getType() const { return m_type; }
	std::string getCommand() const { return m_command; }
	unsigned char getByte(const int index) const { return strtoul(m_command.substr(index, 2).c_str(), NULL, 16); }
	size_t getSize() const { return m_command.size(); }
	std::string getResult() const { return m_result; }
	void setResult(const std::string result) { m_result = result; }

private:
	std::string m_type;
	std::string m_command;
	std::string m_result;

};

class Bus
{

public:
	Bus(const std::string deviceName, const bool noDeviceCheck, const long recvTimeout,
		const std::string dumpFile, const long dumpSize, const bool dumpState);
	~Bus();

	void connect() { m_port->open(); }
	void disconnect() { if (m_port->isOpen() == true) m_port->close(); }
	bool isConnected() { return m_port->isOpen(); }

	void printBytes() const;

	int proceed();
	std::string getCycData();

	void addCommand(BusCommand* busCommand) { m_sendBuffer.push(busCommand); }
	BusCommand* recvCommand();

	int getBus(const unsigned char byte);
	int sendCommand();
	void delCommand() { m_sendBuffer.pop(); }

	void setDumpState(const bool dumpState) { m_dumpState = dumpState; }

private:
	Port* m_port;
	std::stringstream m_sstr;
	std::queue<std::string> m_cycBuffer;
	std::queue<BusCommand*> m_sendBuffer;
	std::queue<BusCommand*> m_recvBuffer;

	const long m_recvTimeout;

	Dump* m_dump;
	bool m_dumpState;

	int proceedCycData(const unsigned char byte);
	int sendByte(const unsigned char byte_sent);
	unsigned char recvByte();
	int recvSlaveData(std::string& result);
	int recvCRC(std::string& result);

};


std::string esc(const std::string& data);
std::string unesc(const std::string& data);

unsigned char calc_crc_byte(unsigned char byte, const unsigned char init_crc);
std::string calc_crc(const std::string& data);


} //namespace

#endif // LIBEBUS_BUS_HPP_
