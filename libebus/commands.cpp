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


Commands::~Commands()
{
	for (mapCI_t iter = m_polDB.begin(); iter != m_polDB.end(); ++iter)
		delete iter->second;

	m_polDB.clear();

	for (mapCI_t iter = m_cycDB.begin(); iter != m_cycDB.end(); ++iter)
		delete iter->second;

	m_cycDB.clear();

	m_cmdDB.clear();
}

void Commands::addCommand(const cmd_t& command)
{
	m_cmdDB.push_back(command);

	if (strcasecmp(command[0].c_str(),"C") == 0) {
		Command* cmd = new Command(m_cmdDB.size()-1, command);
		m_cycDB.insert(pair_t(m_cmdDB.size()-1, cmd));
	}

	if (strcasecmp(command[0].c_str(),"P") == 0) {
		Command* cmd = new Command(m_cmdDB.size()-1, command);
		m_polDB.insert(pair_t(m_cmdDB.size()-1, cmd));
	}
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

	// walk through commands - GET
	if (strcasecmp(cmd[0].c_str(), "GET") == 0) {
		for (index = 0; i != m_cmdDB.end(); i++, index++) {

			// empty line
			if ((*i).size() == 0)
				continue;

			if (((strcasecmp((*i)[0].c_str(), "R") == 0)
			||   (strcasecmp((*i)[0].c_str(), "P") == 0))
			&&   (strcasecmp((*i)[1].c_str(), cmd[1].c_str()) == 0)
			&&   (strcasecmp((*i)[2].c_str(), cmd[2].c_str()) == 0))
				return index;
		}
	// walk through commands - SET, CYC
	} else {
		// correct type
		if (strcasecmp(cmd[0].c_str(), "SET") == 0)
			cmd[0] = "W";
		else if (strcasecmp(cmd[0].c_str(), "CYC") == 0)
			cmd[0] = "C";

		for (index = 0; i != m_cmdDB.end(); i++, index++) {

			// empty line
			if ((*i).size() == 0)
				continue;

			if (strcasecmp((*i)[0].c_str(), cmd[0].c_str()) == 0 &&
			    strcasecmp((*i)[1].c_str(), cmd[1].c_str()) == 0 &&
			    strcasecmp((*i)[2].c_str(), cmd[2].c_str()) == 0)
				return index;
		}
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

int Commands::storeCycData(const std::string& data) const
{
	// no commands defined
	if (m_cycDB.size() == 0)
		return -2;

	// search skipped - string too short
	if (data.length() < 10)
		return -3;

	// prepare string for searching command
	std::string search(data.substr(2, 8 + strtol(data.substr(8,2).c_str(), NULL, 16) * 2));

	mapCI_t iter = m_cycDB.begin();

	// walk through commands
	for (; iter != m_cycDB.end(); iter++) {

		std::string command = getEbusCommand(iter->first);

		// skip wrong search string length
		if (command.length() > search.length())
			continue;

		if (strcasecmp(command.c_str(), search.substr(0,command.length()).c_str()) == 0) {
			iter->second->setData(data);
			return iter->first;
		}
	}

	// command not found
	return -1;
}

std::string Commands::getCycData(int index) const
{
	mapCI_t iter = m_cycDB.find(index);
	if (iter != m_cycDB.end())
		return iter->second->getData();
	else
		return "";
}

int Commands::nextPolCommand()
{
	size_t index = 0;

	m_polIndex++;

	if (m_polIndex == m_polDB.size())
		m_polIndex = 0;

	mapCI_t iter = m_polDB.begin();

	for (; iter != m_polDB.end(); iter++, index++)
		if (index == m_polIndex)
			return iter->first;

	return -1;
}

void Commands::storePolData(const std::string& data) const
{
	// prepare string for searching command
	std::string search(data.substr(2, 8 + strtol(data.substr(8,2).c_str(), NULL, 16) * 2));

	mapCI_t iter = m_polDB.begin();

	// walk through commands
	for (; iter != m_polDB.end(); iter++) {

		std::string command = getEbusCommand(iter->first);

		// skip wrong search string length
		if (command.length() > search.length())
			continue;

		if (strcasecmp(command.c_str(), search.substr(0,command.length()).c_str()) == 0)
			iter->second->setData(data);

	}
}

std::string Commands::getPolData(int index) const
{
	mapCI_t iter = m_polDB.find(index);
	if (iter != m_polDB.end())
		return iter->second->getData();
	else
		return "";
}

void Commands::printCommand(const cmd_t& command) const
{
	if (command.size() == 0)
		return;

	for (cmdCI_t i = command.begin(); i != command.end(); i++)
		std::cout << *i << ';';
}


} //namespace

