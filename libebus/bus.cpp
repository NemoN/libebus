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
#include <unistd.h>
#include <iostream>
#include <iomanip>

namespace libebus
{

//~ Bus::~Bus()
//~ {
	//~ if (isConnected() == true)
		//~ disconnectBus();
//~ }
//~ 
//~ void Bus::connectBus()
//~ {
	//~ m_port.openPort(m_device);
	//~ m_connected = m_port.isOpen();
//~ }
//~ 
//~ void Bus::disconnectBus()
//~ {
	//~ m_port.closePort();
	//~ m_connected = m_port.isOpen();
//~ }
//~ 
//~ void Bus::getBytes()
//~ {
	//~ unsigned char byte;
	//~ ssize_t bytes_read;
	//~ 
	//~ bytes_read = m_port.recvBytes();
//~ 
	//~ for (int i = 0; i < bytes_read; i++) {
		//~ byte = m_port.getByte();
		//~ std::cout << std::hex << std::setw(2) << std::setfill('0')
			  //~ << static_cast<unsigned int>(byte);
		//~ if (byte == 0xAA)
			//~ std::cout << std::endl;
	//~ }
//~ 
//~ }
//~ 
//~ void Bus::printBytes()
//~ {
	//~ unsigned char byte;
	//~ 
	//~ while (m_ByteBuffer.empty() == false) {
		//~ byte = m_ByteBuffer.front();
		//~ std::cout << std::hex << std::setw(2) << std::setfill('0')
		//~ << static_cast<unsigned int>(byte) << std::endl;
		//~ m_ByteBuffer.pop();
	//~ }
//~ }
//~ 

	

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

