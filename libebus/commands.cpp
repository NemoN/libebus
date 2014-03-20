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
#include <iomanip>
#include <vector>

namespace libebus
{


//~ void Commands::printCommand(const cmd_t& command) const
//~ {
	//~ if (command.size() == 0)
		//~ return;
	//~ 
	//~ for (cmdCI_t i = command.begin(); i != command.end(); i++)
		//~ std::cout << *i << ';';
//~ }
//~ 
//~ void Commands::printCommands() const
//~ {
	//~ if (m_cmdDB.size() == 0)
		//~ return;
	//~ 
	//~ for (cmdDBCI_t i = m_cmdDB.begin(); i != m_cmdDB.end(); i++) {
		//~ printCommand(*i);
		//~ std::cout << std::endl;
	//~ }	
//~ }

bool Commands::compareString(const std::string& src1, const std::string& src2) const
{
	std::string tgt1;
	tgt1.resize(src1.size());
	std::transform(src1.begin(), src1.end(), tgt1.begin(), toupper);

	std::string tgt2;
	tgt2.resize(src2.size());
	std::transform(src2.begin(), src2.end(), tgt2.begin(), toupper);
//~ std::cout << "tgt1 :" << tgt1.c_str() << ": tgt2 :" << tgt2.c_str() << ":" << std::endl;
	if (tgt1 == tgt2)
		return true;
	else
		return false;
}

int Commands::findCommand(const std::string& data) const
{
	// no commands definend
	if (m_cmdDB.size() == 0)
		return -2;

	// preapre string for searching command
	std::string token;
	std::istringstream stream(data);
	std::vector<std::string> cmd;
	
	// split stream
	while (std::getline(stream, token, ' ') != 0)
		cmd.push_back(token);

	std::size_t index;
	cmdDBCI_t i = m_cmdDB.begin();

	// walk through commands
	for (index = 0; i != m_cmdDB.end(); i++, index++) {
		
		// empty line
		if ((*i).size() == 0)
			continue;

		if (compareString((*i)[0], cmd[0]) &&
		    compareString((*i)[1], cmd[1]) &&
		    compareString((*i)[2], cmd[2]))
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

		if (compareString(command, search.substr(0,command.length())))
			return index;

	}	

	// command not found
	return -1;
}

} //namespace

