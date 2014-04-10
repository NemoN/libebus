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
	m_data += esc(crc);
}


Bus::Bus(const std::string deviceName, const std::string dumpFile, const long dumpSize, const bool dumpState)
	: m_deviceName(deviceName), m_getBusWait(false),
	  m_dumpFile(dumpFile), m_dumpSize(dumpSize), m_dumpState(dumpState)
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
	int result = 4;

	// fetch new message and get bus
	if (m_cycBuffer.size() == 0 && m_sendBuffer.size() != 0 && m_getBusWait == false) {
		BusCommand* busCommand = m_sendBuffer.front();
	
		result = getBus(busCommand->getByte(0));
		
		if (result != 1)
			m_getBusWait = true;

		return result;
	}

	// wait for new data
	bytes_recv = m_port->recv();

	for (int i = 0; i < bytes_recv; i++) {
		
		// fetch next byte
		byte = m_port->byte();

		if (m_dumpState == true)
			m_dump->write((const char*) &byte);

		// store byte
		result = proceedCycData(byte);
	}

	return result;
}

int Bus::proceedCycData(const unsigned char byte)
{
	if (byte != 0xAA) {
		m_sstr << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte);
		m_getBusWait = true;
		return 3;	
	}
	
	if (byte == 0xAA && m_sstr.str().empty() == false) {
			m_cycBuffer.push(m_sstr.str());
			m_sstr.str(std::string());
			m_getBusWait = false;
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
	//~ struct timeval tact, tlast, tdiff;

	//~ // start time
	//~ gettimeofday(&tlast, NULL);

	// send QQ
	bytes_sent = m_port->send(&byte_sent, 1);
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
		byte_recv = m_port->byte();

		if (m_dumpState == true)
			m_dump->write((const char*) &byte_recv);

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
	std::string crc_calc, crc_recv, slaveData, result;
	int retval = 0;
	
	BusCommand* busCommand = m_sendBuffer.front();
	m_sendBuffer.pop();

	// send ZZ PB SB NN Dx CRC
	for (size_t i = 2; i < busCommand->getDataSize(); i = i + 2) {
		retval = sendByte(busCommand->getByte(i));
		if (retval < 0)
			goto on_exit;
	}

	// BC -> send SYN
	if (busCommand->getType() == "BC") {
		sendByte(SYN);
		goto on_exit;
	}

	// receive ACK
	byte_recv = recvByte();	

	// is slave ACK negative?
	if (byte_recv == NAK) {
		
		// send data (full)
		for (size_t i = 0; i < busCommand->getDataSize(); i = i + 2) {
			retval = sendByte(busCommand->getByte(i));
			if (retval < 0)
				goto on_exit;
		}
	
		// receive ACK
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
	if (busCommand->getType() == "MM") {
		sendByte(SYN);
		goto on_exit;
	}

	// receive NN
	slaveData = recvSlaveData();

	// calculate CRC
	crc_calc = calc_crc(slaveData);

	// receive CRC
	crc_recv = recvCRC();

	// are calculated and received CRC equal?
	if (crc_calc != crc_recv) {

		// send NAK
		retval = sendByte(NAK);
		if (retval < 0)
			goto on_exit;
					
		// receive NN
		slaveData = recvSlaveData();

		// calculate CRC
		crc_calc = calc_crc(slaveData);

		// receive CRC
		crc_recv = recvCRC();
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
		result = "send error";
		break;
	case -2:
		result = "received bytes > sent bytes";
		break;
	case -3:
		result = "NAK received";
		break;
	case -4:
		result = "CRC error";
		break;
	case -5:
		result = "SYN received instead of ACK";
		break;
	case 0:
	default:
		if (busCommand->getType() == "MS") {
			result += unesc(slaveData);
			result += crc_recv;
		} else {
			result = "success";
		}
		
		break;
	}
		
	busCommand->setResult(result.c_str());
	m_recvBuffer.push(busCommand);
	return retval;

}

int Bus::sendByte(const unsigned char byte_sent) const
{
	unsigned char byte_recv;
	ssize_t bytes_sent, bytes_recv;
	
	bytes_sent = m_port->send(&byte_sent, 1);

	// receive 1 byte - must be equal
	bytes_recv = m_port->recv();
	byte_recv = m_port->byte();

	if (m_dumpState == true)
		m_dump->write((const char*) &byte_recv);

	if (bytes_sent != bytes_recv)
		return -2;

	if (byte_sent != byte_recv)
		return -1;

	return 0;
}

unsigned char Bus::recvByte()
{
	unsigned char byte_recv;

	m_port->recv();
	byte_recv = m_port->byte();

	if (m_dumpState == true)
		m_dump->write((const char*) &byte_recv);
		
	return byte_recv;
}

std::string Bus::recvSlaveData()
{
	unsigned char byte_recv;
	std::stringstream sstr;
	
	// receive NN
	byte_recv = recvByte();
	sstr << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned>(byte_recv);

	int NN = byte_recv;
	
	// receive Dx
	for (int j = 0; j < NN; j++) {
		byte_recv = recvByte();
		sstr << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned>(byte_recv);

		// escape sequence increase NN
		if (byte_recv == 0xA9)
			NN++;
	}

	return sstr.str();
}

std::string Bus::recvCRC()
{
	unsigned char byte_recv;
	std::stringstream sstr;

	// receive CRC
	byte_recv = recvByte();
	sstr << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned>(byte_recv);
	
	if (byte_recv == 0xA9) {
		byte_recv = recvByte();
		sstr << std::setw(2) << std::hex << std::setfill('0') << static_cast<unsigned>(byte_recv);
	}

	return unesc(sstr.str());	
}


} //namespace

