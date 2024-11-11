/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_NETWORK_OS_BSD_H_
#define CPPUAL_NETWORK_OS_BSD_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#ifdef OS_STD_UNIX
#   include <errno.h>
#   include <fcntl.h>
#   include <unistd.h>
#   include <arpa/inet.h>
#   include <netinet/tcp.h>
#   include <netdb.h>
#endif

#endif // __cplusplus
#endif // CPPUAL_NETWORK_OS_BSD_H_