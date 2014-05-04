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

#include <string>
#include <vector>
#include <map>

namespace libebus
{


typedef std::vector<std::string> cmd_t;
typedef cmd_t::const_iterator cmdCI_t;

typedef std::vector<cmd_t> cmdDB_t;
typedef cmdDB_t::const_iterator cmdDBCI_t;

class Command
{

public:
	Command(int index, cmd_t command) : m_index(index), m_command(command) {}
	Command(int index, cmd_t command, std::string data)
		: m_index(index), m_command(command), m_data(data) {}

	cmd_t getCommand() const { return m_command; }
	void setData(const std::string& data) { m_data = data; }
	std::string getData() const { return m_data; }
	std::string calcData();

	std::string calcResult(const cmd_t& cmd);

private:
	int m_index;
	cmd_t m_command;
	std::string m_data;
	std::string m_result;
	std::string m_error;

	void calcSub(const std::string& part, const std::string& position,
		     const std::string& type, const std::string& factor);

	void decode(const std::string& data, const std::string& position,
		    const std::string& type, const std::string& factor);

	void encode(const std::string& data, const std::string& type,
		    const std::string& factor);

};

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
