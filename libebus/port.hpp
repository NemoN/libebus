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
#include <string>
#include <queue>

namespace libebus
{

enum DeviceType { SERIAL, NETWORK };

class Device
{

public:
	Device() : m_fd(-1), m_open(false) {}
	virtual ~Device() {}

	virtual void openDevice(const std::string deviceName) = 0;
	virtual void closeDevice() = 0;
	bool isOpen();

	ssize_t sendBytes(const unsigned char* buffer, size_t nbytes);
	ssize_t recvBytes();

	unsigned char getByte();
	
protected:
	int m_fd;
	bool m_open;
	std::queue<unsigned char> m_recvBuffer;

private:	
	bool isValid();

};

class DeviceSerial : public Device
{

public:
	~DeviceSerial() { closeDevice(); }
	
	void openDevice(const std::string deviceName);
	void closeDevice();
	
private:
	termios m_oldSettings;

};

class DeviceNetwork : public Device
{

public:
	~DeviceNetwork() { closeDevice(); }

	void openDevice(const std::string deviceName);
	void closeDevice();
	
private:

};


class Port
{

public:
	Port(const std::string deviceName);
	~Port() { delete m_device; }

	void open() { m_device->openDevice(m_deviceName); }
	void close() { m_device->closeDevice(); }
	bool isOpen() { return m_device->isOpen(); }

	ssize_t send(const unsigned char* buffer, size_t nbytes)
		{ return m_device->sendBytes(buffer, nbytes); }
	ssize_t recv() { return m_device->recvBytes(); }

	unsigned char byte() { return m_device->getByte(); }

private:
	Device* m_device;
	std::string m_deviceName;

	void setType(const DeviceType type);
	
};


} //namespace

#endif //LIBEBUS_PORT_HPP_
