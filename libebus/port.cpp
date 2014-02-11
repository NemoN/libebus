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

#include "port.hpp"
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace libebus
{


Port::Port(): m_fd(-1), m_open(false)
{
	//~ std::cout << "Port()" << std::endl;
}

Port::~Port()
{
	//~ std::cout << "~Port()" << std::endl;
	if (m_open)
		closePort();
}

void Port::openPort(const char *device)
{
	termios newSettings;

	// open file descriptor from serial device
	m_fd = open(device, O_RDWR | O_NOCTTY);

	if (m_fd < 0) {
		m_open = false;
		
	} else {
		// save current settings of serial device
		tcgetattr(m_fd, &m_oldSettings);
		
		memset(&newSettings, '\0', sizeof(newSettings));

		newSettings.c_cflag = B2400 | CS8 | CLOCAL | CREAD;
		newSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		newSettings.c_iflag = IGNPAR;
		newSettings.c_oflag &= ~OPOST;

		newSettings.c_cc[VMIN]  = 1;
		newSettings.c_cc[VTIME] = 0;

		// empty device buffer
		tcflush(m_fd, TCIOFLUSH);

		// activate new settings of serial device
		tcsetattr(m_fd, TCSANOW, &newSettings);

		// set serial device into blocking mode
		fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) & ~O_NONBLOCK);
		
		m_open = true;		
	}

}

void Port::closePort()
{
	// empty device buffer
	tcflush(m_fd, TCIOFLUSH);
	
	// activate old settings of serial device
	tcsetattr(m_fd, TCSANOW, &m_oldSettings);

	// close file descriptor from serial device
	close(m_fd);

	m_fd = -1;
	m_open = false;
}

bool Port::isOpen()
{
	if (!isValid())
		m_open = false;
	
	return m_open;
}

bool Port::isValid()
{
	int status;

	if (ioctl(m_fd, TIOCMGET, &status) < 0) 
		return false;
	else
		return true;	
}


ssize_t Port::sendBytes(const unsigned char *buffer, size_t nbytes)
{
	if (!isValid())
		return -1;
		
	// write bytes to serial device	
	return write(m_fd, buffer, nbytes);
}

ssize_t Port::recvBytes()
{
	if (!isValid())
		return -1;
		
	size_t nbytes;
	ssize_t bytes_read;
	unsigned char buffer[100];
		
	memset(buffer, '\0', sizeof(buffer));
	nbytes = sizeof(buffer);

	// read bytes from serial device
	bytes_read = read(m_fd, buffer, nbytes);

	for (int i = 0; i < bytes_read; i++)
		m_RecvBuffer.push(buffer[i]);

	return bytes_read;
}

unsigned char Port::getByte()
{
	unsigned char byte;

	if (!m_RecvBuffer.empty()) {
		byte = m_RecvBuffer.front();
		m_RecvBuffer.pop();
	
		return byte;
	}

	return 0;
}


} //namespace



