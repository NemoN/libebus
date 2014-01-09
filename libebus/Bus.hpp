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

#include "Port.hpp"
#include <cstring>
#include <queue>

namespace libebus
{


class Bus
{

public:
	Bus(const char *device);
	~Bus();

	void connectBus();
	void disconnectBus();
	bool isConnected();
	
	void getBytes();
	void printBytes();
	
	//~ bool getBus(void);
	//~ void freeBus(void);
	//~ sendMsg();
	//~ recvMsg();
	
	
private:
	const char *m_device;
	Port m_port;
	bool m_connected;
	//~ bool waitSyn();
	std::queue<unsigned char> m_ByteBuffer;
	
	
};


} //namespace

#endif //LIBEBUS_BUS_HPP_
