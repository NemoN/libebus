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

#include "commands.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>

namespace libebus
{


std::string Command::calcResult(const cmd_t& cmd)
{
	// if (type class cmd sub) --> find sub
	// else go through all subs
		
	
	for (cmdCI_t i = m_command.begin(); i != m_command.end(); i++)
		std::cout << *i << ';';

	std::cout << std::endl;
	
	int elements = atoi(m_command[9].c_str());
	std::cout << "elements: " << elements << std::endl;
	for (int i = 0; i < elements; i++)
		std::cout << m_command[10 + i*8] << ";"
			  << m_command[11 + i*8] << ";"
			  << m_command[12 + i*8] << ";"
			  << m_command[13 + i*8] << ";"
			  << m_command[14 + i*8] << ";"
			  << m_command[15 + i*8] << ";"
			  << m_command[16 + i*8] << ";"
			  << m_command[17 + i*8]
			  << std::endl;

	std::cout << std::endl;
	return m_data;
}

void Commands::printCommands() const
{
	if (m_cmdDB.size() == 0)
		return;
	
	for (cmdDBCI_t i = m_cmdDB.begin(); i != m_cmdDB.end(); i++) {
		printCommand(*i);
		std::cout << std::endl;
	}	
}

int Commands::findCommand(const std::string& data) const
{
	// no commands definend
	if (m_cmdDB.size() == 0)
		return -2;

	// preapre string for searching command
	std::string token;
	std::istringstream isstr(data);
	std::vector<std::string> cmd;
	
	// split stream
	while (std::getline(isstr, token, ' ') != 0)
		cmd.push_back(token);

	std::size_t index;
	cmdDBCI_t i = m_cmdDB.begin();

	// walk through commands
	for (index = 0; i != m_cmdDB.end(); i++, index++) {
		
		// empty line
		if ((*i).size() == 0)
			continue;

		if (strcasecmp((*i)[0].c_str(), cmd[0].c_str()) == 0 &&
		    strcasecmp((*i)[1].c_str(), cmd[1].c_str()) == 0 &&
		    strcasecmp((*i)[2].c_str(), cmd[2].c_str()) == 0)
			return index;

	}	
	
	// command not found
	return -1;
}	

int Commands::findData(const std::string& data) const
{
	// no commands definend
	if (m_cmdDB.size() == 0)
		return -2;

	// skip to small search string length
	if (data.length() < 10)
		return -3;
	
	// preapre string for searching command
	std::string search(data.substr(2, 8 + atoi(data.substr(8,2).c_str()) * 2));

	std::size_t index;
	cmdDBCI_t i = m_cmdDB.begin();

	// walk through commands
	for (index = 0; i != m_cmdDB.end(); i++, index++) {
		
		// empty line
		if ((*i).size() == 0)
			continue;

		// prepare string for defined command
		std::string command((*i)[5]);
		command += (*i)[6];
		std::stringstream sstr;
		sstr << std::setw(2) << std::hex << std::setfill('0') << (*i)[7];
		command += sstr.str();
		command += (*i)[8];

		// skip wrong search string length
		if (command.length() > search.length())
			continue;

		//~ if (compareString(command, search.substr(0,command.length())))
		if (strcasecmp(command.c_str(), search.substr(0,command.length()).c_str()) == 0)
			return index;

	}	

	// command not found
	return -1;
}

std::string Commands::getEbusCommand(const int index) const
{
	cmd_t command = m_cmdDB.at(index);
	std::string cmd(command[5]);
	cmd += command[6];
	std::stringstream sstr;
	sstr << std::setw(2) << std::hex << std::setfill('0') << command[7];
	cmd += sstr.str();
	cmd += command[8];
	
	return cmd;
}


void Commands::printCommand(const cmd_t& command) const
{
	if (command.size() == 0)
		return;
	
	for (cmdCI_t i = command.begin(); i != command.end(); i++)
		std::cout << *i << ';';
}


} //namespace

