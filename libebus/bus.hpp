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

#ifndef LIBEBUS_BUS_HPP_
#define LIBEBUS_BUS_HPP_

#include "port.hpp"
#include "dump.hpp"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <queue>

namespace libebus
{


class BusCommand
{

public:
	BusCommand(const std::string type, const std::string data);

	std::string getType() const { return m_type; }
	std::string getData() const { return m_data; }
	unsigned char getByte(const int index) const { return strtol(m_data.substr(index, 2).c_str(), NULL, 16); }
	size_t getDataSize() const { return m_data.size(); }
	std::string getResult() const { return m_result; }
	void setResult(const std::string result) { m_result = result; }

private:
	std::string m_type;
	std::string m_data;
	std::string m_result;

};

class Bus
{

public:
	Bus(const std::string deviceName, const std::string dumpFile, const long dumpSize, const bool dumpState);
	~Bus();

	void connect();
	void disconnect();
	bool isConnected() { return m_connected; }
	
	void printBytes() const;

	int proceed();
	std::string getCycData();

	void addCommand(BusCommand* busCommand) { m_sendBuffer.push(busCommand); }
	BusCommand* recvCommand();

	int getBus(unsigned char byte);
	int sendCommand();
		
	void setDumpState(const bool dumpState) { m_dumpState = dumpState; }
	
private:
	std::string m_deviceName;
	Port* m_port;
	bool m_connected;
	std::stringstream m_sstr;
	std::queue<std::string> m_cycBuffer;
	std::queue<BusCommand*> m_sendBuffer;
	std::queue<BusCommand*> m_recvBuffer;
	bool m_getBusWait;

	Dump* m_dump;
	std::string m_dumpFile; 
	long m_dumpSize;
	bool m_dumpState;

	int proceedCycData(unsigned char byte);
	int sendCheckDump(unsigned char byte_sent);

};


} //namespace

#endif // LIBEBUS_BUS_HPP_
