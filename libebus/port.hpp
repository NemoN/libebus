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

#ifndef LIBEBUS_PORT_HPP_
#define LIBEBUS_PORT_HPP_

#include <termios.h>
#include <unistd.h>
#include <queue>

namespace libebus
{


class Port
{

public:
	Port();
	~Port();

	void openPort(const char *device);
	void closePort();
	bool isOpen();

	ssize_t sendBytes(const unsigned char *buffer, size_t nbytes);
	ssize_t recvBytes();

	unsigned char getByte();
	
private:
	int m_fd;
	bool m_open;
	termios m_oldSettings;
	
	std::queue<unsigned char> m_RecvBuffer;

	bool isValid();
};


} //namespace

#endif //LIBEBUS_PORT_HPP_
