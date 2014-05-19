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
#include "decode.h"
#include <iostream>
#include <iomanip>

namespace libebus
{


BusCommand::BusCommand(const std::string type, const std::string command) : m_type(type)
{
	// esc
	m_command = esc(command);

	// crc + esc
	std::string crc = calc_crc(m_command);
	m_command += esc(crc);
}


Bus::Bus(const std::string deviceName, const bool noDeviceCheck,
	const std::string dumpFile, const long dumpSize, const bool dumpState)
	: m_dumpState(dumpState)
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

	bytes_read = m_port->recv();

	for (int i = 0; i < bytes_read; i++) {
		byte = m_port->byte();
		std::cout << std::nouppercase << std::hex << std::setw(2)
		          << std::setfill('0') << static_cast<unsigned>(byte);
		if (byte == 0xAA)
			std::cout << std::endl;
	}
}

int Bus::proceed()
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	int result = 4;

	// fetch new message and get bus
	if (m_sendBuffer.size() != 0 && m_sstr.str().size() == 0) {
		BusCommand* busCommand = m_sendBuffer.front();
		result = getBus(busCommand->getByte(0));

		return result;
	}

	// wait for new data
	bytes_recv = m_port->recv();

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
	if (byte != 0xAA) {
		m_sstr << std::nouppercase << std::hex << std::setw(2)
		       << std::setfill('0') << static_cast<unsigned>(byte);
		return 3;
	}

	if (byte == 0xAA && m_sstr.str().size() != 0) {
		m_cycBuffer.push(m_sstr.str());
		m_sstr.str(std::string());
		return 2;
	}

	return 4;
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
		return -1;

	// receive 1 byte - must be QQ
	bytes_recv = m_port->recv();

	for (int i = 0; i < bytes_recv; i++) {

		// fetch next byte
		byte_recv = recvByte();

		// compare sent and received byte
		if (bytes_recv == 1 && byte_sent == byte_recv)
			return 1;

		// store byte
		proceedCycData(byte_recv);
	}

	return 0;
}

int Bus::sendCommand()
{
	const unsigned char SYN = 0xAA;
	const unsigned char ACK = 0x00;
	const unsigned char NAK = 0xFF;

	unsigned char byte_recv;
	ssize_t bytes_recv;
	std::string crc_calc, crc_recv, slaveData, result;
	int retval = 0;

	BusCommand* busCommand = m_sendBuffer.front();
	m_sendBuffer.pop();

	// send ZZ PB SB NN Dx CRC
	for (size_t i = 2; i < busCommand->getCommandSize(); i = i + 2) {
		retval = sendByte(busCommand->getByte(i));
		if (retval < 0)
			goto on_exit;
	}

	// BC -> send SYN
	if (strcasecmp(busCommand->getType().c_str(), "BC") == 0) {
		sendByte(SYN);
		goto on_exit;
	}

	// receive ACK
	bytes_recv = m_port->recv();
	if (bytes_recv > 1) {
		retval = -2;
		goto on_exit;
	}

	byte_recv = recvByte();

	// is slave ACK negative?
	if (byte_recv == NAK) {

		// send data (full)
		for (size_t i = 0; i < busCommand->getCommandSize(); i = i + 2) {
			retval = sendByte(busCommand->getByte(i));
			if (retval < 0)
				goto on_exit;
		}

		// receive ACK
		bytes_recv = m_port->recv();
		if (bytes_recv > 1) {
			retval = -2;
			goto on_exit;
		}

		byte_recv = recvByte();

		// is slave ACK negative?
		if (byte_recv == NAK) {
			retval = sendByte(SYN);
			if (retval == 0)
				retval = -3;

			goto on_exit;
		}
	}

	// MM -> send SYN
	if (strcasecmp(busCommand->getType().c_str(), "MM") == 0) {
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

		retval = -4;
	} else {
		// send ACK
		retval = sendByte(ACK);
		if (retval == -1) {
			retval = -5;
			goto on_exit;
		}
	}

	// MS -> send SYN
	sendByte(SYN);

on_exit:
	switch (retval) {
	case -1:
		result = "-1: send error";
		break;
	case -2:
		result = "-2: received bytes > sent bytes";
		break;
	case -3:
		result = "-3: NAK received";
		break;
	case -4:
		result = "-4: CRC error";
		break;
	case -5:
		result = "-5: ACK error";
		break;
	case 0:
	default:
		if (strcasecmp(busCommand->getType().c_str(), "MS") == 0) {
			result = busCommand->getCommand();
			result += "00";
			result += unesc(slaveData);
			result += crc_recv;
			result += "00";
		} else {
			result = "success";
		}

		break;
	}

	// empty receive buffer
	while (m_port->size() != 0)
		byte_recv = recvByte();

	busCommand->setResult(result.c_str());
	m_recvBuffer.push(busCommand);
	return retval;

}

int Bus::sendByte(const unsigned char byte_sent)
{
	unsigned char byte_recv;
	ssize_t bytes_sent, bytes_recv;

	bytes_sent = m_port->send(&byte_sent, 1);

	// receive 1 byte - must be equal
	bytes_recv = m_port->recv();
	if (bytes_sent != bytes_recv)
		return -2;

	byte_recv = recvByte();

	if (byte_sent != byte_recv)
		return -1;

	return 0;
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
	bytes_recv = m_port->recv();
	if (bytes_recv > 1)
		return -2;

	byte_recv = recvByte();
	sstr << std::nouppercase << std::setw(2) << std::hex
	     << std::setfill('0') << static_cast<unsigned>(byte_recv);

	int NN = byte_recv;

	// receive Dx
	for (int i = 0; i < NN; i++) {
		bytes_recv = m_port->recv();
		if (bytes_recv > 1)
			return -2;

		byte_recv = recvByte();
		sstr << std::nouppercase << std::setw(2) << std::hex
		     << std::setfill('0') << static_cast<unsigned>(byte_recv);

		// escape sequence increase NN
		if (byte_recv == 0xA9)
			NN++;
	}

	result = sstr.str();
	return 0;
}

int Bus::recvCRC(std::string& result)
{
	unsigned char byte_recv;
	ssize_t bytes_recv;
	std::stringstream sstr;

	// receive CRC
	bytes_recv = m_port->recv();
	if (bytes_recv > 1)
		return -2;

	byte_recv = recvByte();
	sstr << std::nouppercase << std::setw(2) << std::hex
	     << std::setfill('0') << static_cast<unsigned>(byte_recv);

	if (byte_recv == 0xA9) {
		// receive CRC
		bytes_recv = m_port->recv();
		if (bytes_recv > 1)
			return -2;

		byte_recv = recvByte();
		sstr << std::nouppercase << std::setw(2) << std::hex
		     << std::setfill('0') << static_cast<unsigned>(byte_recv);
	}

	result = unesc(sstr.str());

	return 0;
}


} //namespace

