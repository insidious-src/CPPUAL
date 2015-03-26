/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/input/joystick.h>

#ifdef OS_STD_UNIX
#	ifdef OS_GNU_LINUX
#		include <fcntl.h>
#		include <linux/joystick.h>
#	elif defined (OS_MAC)
#		include <mach/mach.h>
#	elif defined (OS_AIX) or defined (OS_SOLARIS)
#		include <fcntl.h>
#		include <procfs.h>
#	elif defined (OS_BSD)
#		include <sys/sysctl.h>
#	endif
#elif defined (OS_WINDOWS)
#	include <Windows.h>
#endif

namespace cppual { namespace Input {

} } // namespace Joystick
