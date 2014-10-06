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


typedef std::vector<cmd_t> cmdDB_t;
typedef cmdDB_t::const_iterator cmdDBCI_t;

typedef std::map<int, Command*> map_t;
typedef map_t::const_iterator mapCI_t;
typedef std::pair<int, Command*> pair_t;

class Commands
{

public:
	~Commands();

	void addCommand(const cmd_t& command);
	void printCommands() const;

	std::size_t sizeCmd() const { return m_cmdDB.size(); }
	std::size_t sizeData() const { return m_dataDB.size(); }

	cmd_t const& operator[](const std::size_t& index) const { return m_cmdDB[index]; }

	int findCommand(const std::string& data) const;
	std::string getType(const int index) const { return std::string(m_cmdDB.at(index)[4]); }
	std::string getEbusCommand(const int index) const;

	int storeData(const std::string& data) const;
	std::string getData(int index) const;

private:
	cmdDB_t m_cmdDB;
	map_t m_dataDB;

	void printCommand(const cmd_t& command) const;

};


} //namespace

#endif // LIBEBUS_COMMANDS_H_
