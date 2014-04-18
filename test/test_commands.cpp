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

#include "libebus.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace libebus;

// will be part of cfg csv class
void readCSV(std::istream& is, Commands& commands){
	std::string line;
	
	// read lines
	while (std::getline(is, line) != 0) {
		std::vector<std::string> row;
		std::string column;
		int count;

		count = 0;
		
		std::istringstream stream(line);
		
		// walk through columns
		while (std::getline(stream, column, ';') != 0) {
			row.push_back(column);
			count++;
		}
		
		commands.addCommand(row);
	}
}



int main()
{
	Commands* commands = ConfigCommands("test", CSV).getCommands();
	std::cout << "Commandlines: " << commands->size() << std::endl;
	
	commands->printCommands();

		//~ std::cout << commands[2][9] << std::endl;

	std::size_t index = commands->findCommand("get vr903 RaumTempSelfHeatingOffset");
	std::cout << "found at index: " << index << std::endl;

	//~ commands->printCommand();

	return 0;
}


