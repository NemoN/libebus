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
	Bus bus("/dev/ttyUSB0");

	bus.connectBus();

	if (bus.isConnected())
		std::cout << "connectBus successful." << std::endl;


	int count = 0;

	while (count < 100) {
		bus.getBytes();
		count++;
	} 

	bus.disconnectBus();

	if (!bus.isConnected())
		std::cout << "disconnectBus successful." << std::endl;
	
	return 0;

}
