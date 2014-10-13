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

#include "bus.h"
#include <iostream>
#include <iomanip>

namespace libebus
{

const unsigned char ESC = 0xA9;
const unsigned char SYN = 0xAA;
const unsigned char ACK = 0x00;
const unsigned char NAK = 0xFF;
const unsigned char BROADCAST = 0xFE;

#define RECV_TIMEOUT 10000

bool isMaster(unsigned char addr) {
	unsigned char addrHi = (addr&0xf0)>>4;
	unsigned char addrLo = (addr&0x0f);
	return ((addrHi==0x0) || (addrHi==0x1) || (addrHi==0x3) || (addrHi==0x7) || (addrHi==0xf))
		&& ((addrLo==0x0) || (addrLo==0x1) || (addrLo==0x3) || (addrLo==0x7) || (addrLo==0xf));
}


BusCommand::BusCommand(const std::string command)
{
	// esc
	m_command = esc(command);

	// crc + esc
	std::string crc = calc_crc(m_command);
	m_command += esc(crc);

	unsigned char dstAddress = strtoul(command.substr(2, 2).c_str(), NULL, 16);
	if (dstAddress==BROADCAST) {
		m_type = broadcast;
	} else if ( isMaster(dstAddress) ) {
		m_type = masterMaster;
	} else {
		m_type = masterSlave;
	}
}

const char* BusCommand::getTypeCStr() {
	switch (m_type) {
		case broadcast: return "BC";
		case masterMaster: return "MM";
		case masterSlave: return "MS";
		default: return "?";
	}
}

const char* BusCommand::getResultCodeCStr() {
	switch (m_resultCode) {
		case RESULT_ERR_SEND: return "ERR_SEND: send error";
		case RESULT_ERR_EXTRA_DATA: return "ERR_EXTRA_DATA: received bytes > sent bytes";
		case RESULT_ERR_NAK: return "ERR_NAK: NAK received";
		case RESULT_ERR_CRC: return "ERR_CRC: CRC error";
		case RESULT_ERR_ACK: return "ERR_ACK: ACK error";
		case RESULT_ERR_TIMEOUT: return "ERR_TIMEOUT: read timeout";
		case RESULT_ERR_SYN: return "ERR_SYN: SYN received";
		case RESULT_ERR_ESC:return "ERR_ESC: invalid escape sequence received";
		default: return "success";
	}
}


Bus::Bus(const std::string deviceName, const bool noDeviceCheck, const long recvTimeout,
	const std::string dumpFile, const long dumpSize, const bool dumpState)
	: m_recvTimeout(recvTimeout), m_dumpState(dumpState)
{
	m_port = new Port(deviceName, noDeviceCheck);
	m_dump = new Dump(dumpFile, dumpSize);
}

Bus::~Bus()
{
	if (isConnected() == true)
		disconnect();

	delete m_port;
	delete m_dump;
}

void Bus::printBytes() const
{
	unsigned char byte;
	ssize_t bytes_read;

	bytes_read = m_port->recv(0);

	for (int i = 0; i < bytes_read; i++) {
		byte = m_port->byte();
		std::cout << std::nouppercase << std::hex << std::setw(2)
		          << std::setfill('0') << static_cast<unsigned>(byte);
		if (byte == SYN)
			std::cout << std::endl;
	}
}

int Bus::proceed()
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	int result = RESULT_AUTO_SYN;

	// fetch new message and get bus
	if (m_sendBuffer.size() != 0 && m_sstr.str().size() == 0) {
		BusCommand* busCommand = m_sendBuffer.front();
		result = getBus(busCommand->getByte(0));

		return result;
	}

	// wait for new data
	bytes_recv = m_port->recv(0);

	for (int i = 0; i < bytes_recv; i++) {

		// fetch next byte
		byte_recv = recvByte();

		// store byte
		result = proceedCycData(byte_recv);
	}

	return result;
}

int Bus::proceedCycData(const unsigned char byte)
{
	if (byte != SYN) {
		m_sstr << std::nouppercase << std::hex << std::setw(2)
		       << std::setfill('0') << static_cast<unsigned>(byte);
		return RESULT_DATA;
	}

	if (byte == SYN && m_sstr.str().size() != 0) {
		m_cycBuffer.push(m_sstr.str());
		m_sstr.str(std::string());
		return RESULT_SYN;
	}

	return RESULT_AUTO_SYN;
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

int Bus::getBus(const unsigned char byte_sent)
{
	unsigned char byte_recv;
	ssize_t bytes_sent, bytes_recv;

	// send QQ
	bytes_sent = m_port->send(&byte_sent, 1);
	if (bytes_sent <= 0)
		return RESULT_ERR_SEND;

	// receive 1 byte - must be QQ
	bytes_recv = m_port->recv(0);

	for (int i = 0; i < bytes_recv; i++) {

		// fetch next byte
		byte_recv = recvByte();

		// compare sent and received byte
		if (bytes_recv == 1 && byte_sent == byte_recv)
			return RESULT_BUS_ACQUIRED;

		// store byte
		proceedCycData(byte_recv);
	}

	return RESULT_ERR_BUS_LOST;
}

int Bus::sendCommand()
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	std::string crc_calc, crc_recv, slaveData, result;
	int retval = RESULT_OK;

	BusCommand* busCommand = m_sendBuffer.front();
	m_sendBuffer.pop();

	// send ZZ PB SB NN Dx CRC
	for (size_t i = 2; i < busCommand->getSize(); i = i + 2) {
		retval = sendByte(busCommand->getByte(i));
		if (retval < 0)
			goto on_exit;
	}

	// BC -> send SYN
	if (busCommand->getType()==broadcast) {
		sendByte(SYN);
		goto on_exit;
	}

	// receive ACK
	bytes_recv = m_port->recv(m_recvTimeout);
	if (bytes_recv > 1) {
		retval = RESULT_ERR_EXTRA_DATA;
		goto on_exit;
	} else if (bytes_recv < 0) {
		retval = RESULT_ERR_TIMEOUT;
		goto on_exit;
	}

	byte_recv = recvByte();

	// is received byte SYN?
	if (byte_recv == SYN) {
		retval = RESULT_ERR_SYN;
		goto on_exit;
	}

	// is slave ACK negative?
	if (byte_recv == NAK) {

		// send QQ ZZ PB SB NN Dx CRC again
		for (size_t i = 0; i < busCommand->getSize(); i = i + 2) {
			retval = sendByte(busCommand->getByte(i));
			if (retval < 0)
				goto on_exit;
		}

		// receive ACK
		bytes_recv = m_port->recv(m_recvTimeout);
		if (bytes_recv > 1) {
			retval = RESULT_ERR_EXTRA_DATA;
			goto on_exit;
		} else if (bytes_recv < 0) {
			retval = RESULT_ERR_TIMEOUT;
			goto on_exit;
		}

		byte_recv = recvByte();

		// is received byte SYN?
		if (byte_recv == SYN) {
			retval = RESULT_ERR_SYN;
			goto on_exit;
		}

		// is slave ACK negative?
		if (byte_recv == NAK) {
			retval = sendByte(SYN);
			if (retval == 0)
				retval = RESULT_ERR_NAK;

			goto on_exit;
		}
	}

	// MM -> send SYN
	if (busCommand->getType()==masterMaster) {
		sendByte(SYN);
		goto on_exit;
	}

	// receive NN
	retval = recvSlaveData(slaveData);
	if (retval < 0)
		goto on_exit;

	// calculate CRC
	crc_calc = calc_crc(slaveData);

	// receive CRC
	retval = recvCRC(crc_recv);
	if (retval < 0)
		goto on_exit;

	// are calculated and received CRC equal?
	if (crc_calc != crc_recv) {

		// send NAK
		retval = sendByte(NAK);
		if (retval < 0)
			goto on_exit;

		// receive NN
		retval = recvSlaveData(slaveData);
		if (retval < 0)
			goto on_exit;

		// calculate CRC
		crc_calc = calc_crc(slaveData);

		// receive CRC
		retval = recvCRC(crc_recv);
		if (retval < 0)
			goto on_exit;

	}

	// are calculated and received CRC equal?
	if (crc_calc != crc_recv) {
		// send NAK
		retval = sendByte(NAK);
		if (retval < 0)
			goto on_exit;

		retval = RESULT_ERR_CRC;
	} else {
		// send ACK
		retval = sendByte(ACK);
		if (retval == -1) {
			retval = RESULT_ERR_ACK;
			goto on_exit;
		}
	}

	// MS -> send SYN
	sendByte(SYN);

on_exit:
	if (retval>=0) {
		if (busCommand->getType()==masterSlave) {
			result = busCommand->getCommand();
			result += "00";
			result += unesc(slaveData);
			result += crc_recv;
			result += "00";
		} else {
			result = "success";
		}
	}

	// empty receive buffer
	while (m_port->size() != 0)
		byte_recv = recvByte();

	busCommand->setResult(result.c_str(), retval);
	m_recvBuffer.push(busCommand);
	return retval;

}

int Bus::sendByte(const unsigned char byte_sent)
{
	unsigned char byte_recv;
	ssize_t bytes_sent, bytes_recv;

	bytes_sent = m_port->send(&byte_sent, 1);

	// receive 1 byte - must be equal
	bytes_recv = m_port->recv(RECV_TIMEOUT);
	if (bytes_sent != bytes_recv)
		return RESULT_ERR_EXTRA_DATA;

	byte_recv = recvByte();

	if (byte_sent != byte_recv)
		return RESULT_ERR_SEND;

	return RESULT_OK;
}

unsigned char Bus::recvByte()
{
	unsigned char byte_recv;

	// fetch byte
	byte_recv = m_port->byte();

	if (m_dumpState == true)
		m_dump->write((const char*) &byte_recv);

	return byte_recv;
}

int Bus::recvSlaveData(std::string& result)
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	std::stringstream sstr;

	// receive NN
	bytes_recv = m_port->recv(RECV_TIMEOUT);
	if (bytes_recv > 1)
		return RESULT_ERR_EXTRA_DATA;

	byte_recv = recvByte();
	sstr << std::nouppercase << std::setw(2) << std::hex
	     << std::setfill('0') << static_cast<unsigned>(byte_recv);

	int NN = byte_recv;

	// receive Dx
	for (int i = 0; i < NN; i++) {
		bytes_recv = m_port->recv(RECV_TIMEOUT);
		if (bytes_recv > 1)
			return RESULT_ERR_EXTRA_DATA;

		byte_recv = recvByte();
		sstr << std::nouppercase << std::setw(2) << std::hex
		     << std::setfill('0') << static_cast<unsigned>(byte_recv);

		// escape sequence increase NN
		if (byte_recv == ESC)
			NN++;
	}

	result = sstr.str();
	return RESULT_OK;
}

int Bus::recvCRC(std::string& result)
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	std::stringstream sstr;

	// receive CRC
	bytes_recv = m_port->recv(RECV_TIMEOUT);
	if (bytes_recv > 1)
		return RESULT_ERR_EXTRA_DATA;

	byte_recv = recvByte();
	sstr << std::nouppercase << std::setw(2) << std::hex
	     << std::setfill('0') << static_cast<unsigned>(byte_recv);

	if (byte_recv == ESC) {
		// receive CRC
		bytes_recv = m_port->recv(RECV_TIMEOUT);
		if (bytes_recv > 1)
			return RESULT_ERR_EXTRA_DATA;

		byte_recv = recvByte();
		sstr << std::nouppercase << std::setw(2) << std::hex
		     << std::setfill('0') << static_cast<unsigned>(byte_recv);
	}

	result = unesc(sstr.str());

	return RESULT_OK;
}


std::string esc(const std::string& data)
{
	std::string escaped;

	for (size_t i = 0; i < data.size(); i = i + 2) {
		long test = strtol(data.substr(i, 2).c_str(), NULL, 16);

		if (test == SYN)
			escaped += "a901";
		else if (test == ESC)
			escaped += "a900";
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

		if (test == ESC) {
			found = true;

		} else if (found == true) {
			if (test == 0x01)
				unescaped += "aa";
			else
				unescaped += "a9";

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
	sstr << std::nouppercase << std::setw(2) << std::hex
	     << std::setfill('0') << static_cast<unsigned>(crc);

	return sstr.str();
}


} //namespace

