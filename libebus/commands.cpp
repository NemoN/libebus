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

#include "commands.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

namespace libebus
{


void Commands::printCommand(const cmd_t& command) const
{
	if (!command.size())
		return;
	
	for (cmdCI_t i = command.begin(); i != command.end(); i++)
		std::cout << *i << ';';
}

void Commands::printCommands() const
{
	if (!m_cmdDB.size())
		return;
	
	for (cmdDBCI_t i = m_cmdDB.begin(); i != m_cmdDB.end(); i++) {
		printCommand(*i);
		std::cout << std::endl;
	}	
}

bool Commands::compareString(const std::string src1, const std::string src2) const
{
	std::string tgt1;
	tgt1.resize(src1.size());
	std::transform(src1.begin(), src1.end(), tgt1.begin(), toupper);

	std::string tgt2;
	tgt2.resize(src2.size());
	std::transform(src2.begin(), src2.end(), tgt2.begin(), toupper);
	
	if (tgt1 == tgt2)
		return true;
	else
		return false;
}

std::size_t Commands::findCommand(const std::string string) const
{
	if (!m_cmdDB.size())
		return -1;

	std::string delimiter = " ";
	std::string token;
	std::istringstream stream(string);
	std::vector<std::string> cmd;
	
	// split stream
	while (std::getline(stream, token, ' ')) {
		std::cout << token << std::endl;
		cmd.push_back(token);
	}

	std::size_t index;
	cmdDBCI_t i = m_cmdDB.begin();

	// walk through commands
	for (index = 0; i != m_cmdDB.end(); i++, index++) {

		// const cmd_t command = *i; --> (*i) <-> command

		if (!(*i).size())
			continue;

		if (compareString((*i)[0], cmd[0]) &&
		    compareString((*i)[1], cmd[1]) &&
		    compareString((*i)[2], cmd[2]))
			return index;

	}	
	
	return index;
}	


} //namespace

