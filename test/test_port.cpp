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

#include "libebus.hpp"
#include <iostream>
#include <iomanip>

using namespace libebus;

int main ()
{
	Port port;

	port.openPort("/dev/ttyUSB0");

	if(port.isOpen())
		std::cout << "openPort successful." << std::endl;

	int count = 0;

	while(count < 100) {
		ssize_t bytes_read;

		bytes_read = port.recvBytes();

		for (int i = 0; i < bytes_read; i++)
			std::cout << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<unsigned int>(port.getByte()) << std::endl;
		
		bytes_read = 0;

		count++;
	} 

	port.closePort();

	if(!port.isOpen())
		std::cout << "closePort successful." << std::endl;
			
	return 0;

}
