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

#include "Command.hpp"
#include <string>
#include <vector>

namespace libebus
{

enum FileType {CSV, XML};

class Configfile
{

public:
	Configfile() {}
	virtual ~Configfile() {}

	virtual void readFile(std::istream& is, Command& command) = 0;
	
};

class ConfigfileCSV : public Configfile
{
	
public:
	ConfigfileCSV() {}
	~ConfigfileCSV() {}

	void readFile(std::istream& is, Command& command);
	
};

class ConfigfileXML : public Configfile
{
	
public:
	ConfigfileXML() {}
	~ConfigfileXML() {}

	void readFile(std::istream& is, Command& command);
	
};


class Config
{
	
public:
	Config(const std::string path, const FileType type);
	~Config();
	
	void setType(const FileType type);
	void printFiles() const;
	void getCommands(Command& command);
	
private:
	Configfile* m_configfile;
	std::string m_path;
	std::string m_extension;
	std::vector<std::string> m_files;

	void addFiles(const std::string path, const std::string extension);
	
};


} //namespace

#endif //LIBEBUS_CONFIG_HPP_
