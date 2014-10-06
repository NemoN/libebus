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

#ifndef LIBEBUS_COMMANDS_H_
#define LIBEBUS_COMMANDS_H_

#include "command.h"
#include <string>
#include <vector>
#include <map>

namespace libebus
{


typedef std::map<int, Command*> map_t;
typedef map_t::const_iterator mapCI_t;
typedef std::pair<int, Command*> pair_t;

class Commands
{

public:
	void addCommand(const cmd_t& command) { m_cmdDB.push_back(command); }
	void printCommands() const;

	std::size_t size() const { return m_cmdDB.size(); }

	cmd_t const& operator[](const std::size_t& index) const { return m_cmdDB[index]; }

	int findCommand(const std::string& data) const;
	int findData(const std::string& data) const;
	std::string getType(const int index) const { return std::string(m_cmdDB.at(index)[4]); }
	std::string getEbusCommand(const int index) const;

private:
	cmdDB_t m_cmdDB;

	void printCommand(const cmd_t& command) const;

};


} //namespace

#endif // LIBEBUS_COMMANDS_H_
