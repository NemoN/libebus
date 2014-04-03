/*
 * Copyright (C) Roland Jax 2014 <roland.jax@liwest.at>
 * crc calculations from http://www.mikrocontroller.net/topic/75698
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

#ifndef LIBEBUS_DECODE_HPP_
#define LIBEBUS_DECODE_HPP_

#include <string>

namespace libebus
{


std::string esc(std::string data);
std::string unesc(std::string data);

unsigned char calc_crc_byte(unsigned char byte, const unsigned char init_crc);
std::string calc_crc(const std::string data);


} //namespace

#endif // LIBEBUS_DECODE_HPP_
