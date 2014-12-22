/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifndef CPPUAL_FAST_FUNC_H_
#define CPPUAL_FAST_FUNC_H_
#ifdef __cplusplus

#include <ios>

using std::string;

namespace cppual { namespace IO {

struct IStream
{
	virtual bool open () = 0;
	virtual void close () = 0;
	virtual bool writable () = 0;
	virtual bool readable () = 0;
	virtual bool seekable () = 0;
};

struct IReadStream : virtual public IStream
{
	virtual void read () = 0;
	virtual void pos () = 0;
};

struct IWriteStream : virtual public IStream
{
	virtual void write (string const&) = 0;
	virtual void append (string const&) = 0;
	virtual void replace (string const&) = 0;
};

struct IProtocol
{
	virtual string location () = 0;
	virtual bool   setLocation (string const&) = 0;
	virtual bool   open (string const&) = 0;
	virtual void   close () = 0;
	virtual bool   move (string const&, string const&) = 0;
	virtual bool   copy (string const&, string const&) = 0;
	virtual bool   save (IStream&, string const&) = 0;
};

} } // namespace IO

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_

