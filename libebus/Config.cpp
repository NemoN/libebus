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

#include "Config.hpp"
#include <iostream>
#include <sstream>
#include <dirent.h>

namespace libebus
{


Configfile::Configfile()
{
	//~ std::cout << "Configfile()" << std::endl;
};

Configfile::~Configfile()
{
	//~ std::cout << "~Configfile()" << std::endl;
};



ConfigfileCSV::ConfigfileCSV()
{
	//~ std::cout << "ConfigfileCSV()" << std::endl;
};

ConfigfileCSV::~ConfigfileCSV()
{
	//~ std::cout << "~ConfigfileCSV()" << std::endl;
};

void ConfigfileCSV::readFile(std::istream& is, Command& command)
{
	std::string line;
	
	// read lines
	while (std::getline(is, line)) {
		cmd_t row;
		std::string column;
		int count;

		count = 0;
		
		std::istringstream stream(line);
		
		// walk through columns
		while (std::getline(stream, column, ';')) {
			row.push_back(column);
			count++;
		}
		
		command.addCommand(row);
	}
};



ConfigfileXML::ConfigfileXML()
{
	//~ std::cout << "ConfigfileXML()" << std::endl;
};

ConfigfileXML::~ConfigfileXML()
{
	//~ std::cout << "~ConfigfileXML()" << std::endl;
};

void ConfigfileXML::readFile(std::istream& is, Command& command)
{
	;
	// ToDo: Implamantion for xml files
}



Config::Config(const std::string path, const FileType type)
{
	//~ std::cout << "Config()" << std::endl;
	m_path = path;
	m_configfile = NULL;
	setType(type);
	addFiles(m_path, m_extension);
}

Config::~Config()
{
	//~ std::cout << "~Config()" << std::endl;
	delete m_configfile;
};

void Config::setType(const FileType type)
{
	if (m_configfile != NULL)
		delete m_configfile;
	
	switch (type) {
	case CSV:
		m_configfile = new ConfigfileCSV();
		m_extension = "csv";
		break;
	case XML:
		m_configfile = new ConfigfileXML();
		m_extension = "xml";
		break;
	};
};

void Config::addFiles(const std::string path, const std::string extension)
{
        DIR* dir = opendir(path.c_str());

	if (dir == NULL)
		return;
        
        dirent* d = readdir(dir);
        
        while (d) {
		
                if (d->d_type == DT_DIR) {
                        std::string fname = d->d_name;
                        
                        if (fname != "." && fname != "..") {
				const std::string p = path + "/" + d->d_name;
                                addFiles(p, extension);
			}
                                
                } else if (d->d_type == DT_REG) {
                        std::string fname = d->d_name;

                        if (fname.find(extension, (fname.length() - extension.length())) != std::string::npos) {
				const std::string p = path + "/" + d->d_name;
                                m_files.push_back(p);
			}
                }
                
                d = readdir(dir);
        }
};

void Config::printFiles()
{
	std::vector<std::string>::const_iterator i = m_files.begin();
	for(; i != m_files.end(); i++)
		std::cout << *i << std::endl; 	
};




} //namespace

