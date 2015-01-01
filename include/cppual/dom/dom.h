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

#ifndef CPPUAL_FORMAT_DOM_H_
#define CPPUAL_FORMAT_DOM_H_
#ifdef __cplusplus

#include <fstream>
#include <cppual/mstring.h>

using std::string;
using std::fstream;

namespace cppual {

class DOMParser
{
public:
	inline DOMParser () = default;

	inline
	explicit DOMParser (string const& gPath,
						string const& gDelimBegin  = "<",
						string const& gDelimBegin2 = "</",
						string const& gDelimEnd	   = "/>",
						string const& gDelimEnd2   = ">")
	: m_gFile (gPath),
	  m_gDelimBegin (gDelimBegin),
	  m_gDelimBegin2 (gDelimBegin2),
	  m_gDelimEnd (gDelimEnd),
	  m_gDelimEnd2 (gDelimEnd2)
	{ }

private:
	fstream m_gFile;
	string  m_gDelimBegin, m_gDelimBegin2, m_gDelimEnd, m_gDelimEnd2;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FORMAT_DOM_H_
