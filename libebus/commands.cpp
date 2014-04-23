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


std::string Command::getResult(const cmd_t& cmd)
{
	int elements = atoi(m_command[9].c_str());
	
	if (cmd.size() > 3) {
		bool found = false;
		
		for (size_t i = 3; i < cmd.size(); i++) {
			int j;
			
			for (j = 0; j < elements; j++) {
				if (m_command[10 + j*8] == cmd[i]) {
					found = true;
					break;
				}
			}
			
			if (found == true) {
				found = false;
std::cout << "1 decode: " << m_command[10 + j*8] << std::endl;
				// decode
				calcResult(m_command[11 + j*8], m_command[12 + j*8],
					   m_command[13 + j*8], m_command[14 + j*8]);
			}
			
		}
		
	} else {
		for (int j = 0; j < elements; j++) {
std::cout << "2 decode: " << m_command[10 + j*8] << std::endl;
			// decode
			calcResult(m_command[11 + j*8], m_command[12 + j*8],
				   m_command[13 + j*8], m_command[14 + j*8]);
		}
	}

	return m_result;
}

void Command::calcResult(const std::string& part, const std::string& position,
			 const std::string& type, const std::string& factor)
{
	std::string data;
	
	// Master Data
	if (strcasecmp(part.c_str(), "MD") == 0) {
		
		// QQ ZZ PB SB NN
		int md_pos = 10;
		int md_len = atoi(m_command[7].c_str())*2;
		data = m_data.substr(md_pos, md_len);
std::cout << "MD: " << data.c_str() << std::endl;
	}
	
	// Slave Acknowledge
	else if (strcasecmp(part.c_str(), "SA") == 0) {
		// QQ ZZ PB SB NN + Dx + CRC
		int sa_pos = 10 + (atoi(m_command[7].c_str())*2) + 2;
		int sa_len = 2;
		data = m_data.substr(sa_pos, sa_len);
std::cout << "SA: " << data.c_str() << std::endl;
	}

	// Slave Data
	else if (strcasecmp(part.c_str(), "SD") == 0) {
		// QQ ZZ PB SB NN + Dx + CRC ACK NN
		int sd_pos = 10 + (atoi(m_command[7].c_str())*2) + 6;
		int sd_len = m_data.length() - (10 + (atoi(m_command[7].c_str())*2) + 6) - 4;
		data = m_data.substr(sd_pos, sd_len);
std::cout << "SD: " << data.c_str() << std::endl;
	}

	// Master Acknowledge
	else if (strcasecmp(part.c_str(), "MA") == 0) {
		// QQ ZZ PB SB NN + Dx + CRC ACK NN + Dx
		int ma_pos = m_data.length() - 2;
		int ma_len = 2;
		data = m_data.substr(ma_pos, ma_len);
std::cout << "MA: " << data.c_str() << std::endl;
	}

	decode(data, position, type, factor);
}

void Command::decode(const std::string& data, const std::string& position,
		     const std::string& type, const std::string& factor)
{
	std::ostringstream result;
	
	// prepare position
	std::string token;
	std::istringstream stream(position);
	std::vector<int> pos;
	
	while (std::getline(stream, token, ',') != 0)
		pos.push_back(atoi(token.c_str()));

	if (strcasecmp(type.c_str(), "HEX") == 0) {
		std::cout << "HEX" << std::endl;
		result << data;
	}
	//~ else if (strcasecmp(type.c_str(), "UCH") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "SCH") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "UIN") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "SIN") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "ULG") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "SLG") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "FLT") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "STR") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "D1B") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "D1C") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "D2B") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "D2C") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "BDA") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "HDA") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "BTI") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "HTI") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "BDY") == 0) {
	//~ }
	//~ else if (strcasecmp(type.c_str(), "HDY") == 0) {
	//~ }
	else {
		result << "type '" << type.c_str() << "' not implemented."; 
	}

	m_result = result.str();
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

