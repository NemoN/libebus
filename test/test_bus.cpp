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
	Bus bus("/dev/ttyUSB20");

	bus.connect();

	if (bus.isConnected() == true)
		std::cout << "connect successful." << std::endl;


	int count = 0;

	while (1) {
		bus.printBytes();
		count++;
	} 

	bus.disconnect();

	if (bus.isConnected() == false)
		std::cout << "disconnect successful." << std::endl;
	
	return 0;

}
