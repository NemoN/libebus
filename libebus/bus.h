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

#include "symbol.h"
#include "result.h"
#include "port.h"
#include "dump.h"
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <queue>

namespace libebus
{


// the maximum time allowed for retrieving a byte from an addressed slave
#define RECV_TIMEOUT 10000


enum CommandType { invalid, broadcast, masterMaster, masterSlave };


class BusCommand
{

public:
	BusCommand(const std::string commandStr);

	CommandType getType() const { return m_type; }
	SymbolString getCommand() const { return m_command; }
	bool isErrorResult() const { return m_resultCode < 0; }
	const char* getResultCodeCStr();
	SymbolString getResult() const { return m_result; }
	void setResult(const SymbolString result, const int resultCode) { m_result = result; m_resultCode = resultCode; }
	const std::string getMessageStr();

private:
	CommandType m_type;
	SymbolString m_command;
	SymbolString m_result;
	int m_resultCode;
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
	SymbolString getCycData();

	void addCommand(BusCommand* busCommand) { m_sendBuffer.push(busCommand); }
	BusCommand* recvCommand();

	int getBus(const unsigned char byte);
	int sendCommand();
	void delCommand();

	void setDumpState(const bool dumpState) { m_dumpState = dumpState; }

private:
	Port* m_port;
	bool m_previousEscape;
	SymbolString m_sstr;
	std::queue<SymbolString> m_cycBuffer;
	std::queue<BusCommand*> m_sendBuffer;
	std::queue<BusCommand*> m_recvBuffer;

	const long m_recvTimeout;

	Dump* m_dump;
	bool m_dumpState;

	bool m_busLocked;
	bool m_busPriorRetry;

	int proceedCycData(const unsigned char byte);
	int sendByte(const unsigned char byte_sent);
	unsigned char recvByte();
	int recvSlaveDataAndCRC(SymbolString& result);

};


} //namespace

#endif // LIBEBUS_BUS_HPP_
