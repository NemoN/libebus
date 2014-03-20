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

#ifndef LIBEBUS_CONFIG_HPP_
#define LIBEBUS_CONFIG_HPP_

#include "commands.hpp"
#include <string>
#include <vector>

namespace libebus
{

enum FileType { CSV, XML };

class ConfigFile
{

public:
	virtual ~ConfigFile() {}

	virtual void readFile(std::istream& is, Commands& commands) = 0;
	
};

class ConfigFileCSV : public ConfigFile
{
	
public:
	~ConfigFileCSV() {}

	void readFile(std::istream& is, Commands& commands);
	
};

class ConfigFileXML : public ConfigFile
{
	
public:
	~ConfigFileXML() {}

	void readFile(std::istream& is, Commands& commands);
	
};


class ConfigCommands
{
	
public:
	ConfigCommands(const std::string path, const FileType type);
	~ConfigCommands() { delete m_configfile; }
	
	void setType(const FileType type);
	//~ void printFiles() const;
	Commands* getCommands();
	
private:
	ConfigFile* m_configfile;
	std::string m_path;
	std::string m_extension;
	std::vector<std::string> m_files;

	void addFiles(const std::string path, const std::string extension);
	
};


} //namespace

#endif // LIBEBUS_CONFIG_HPP_
