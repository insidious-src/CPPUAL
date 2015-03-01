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

#ifndef CPPUAL_FORMAT_XML_H_
#define CPPUAL_FORMAT_XML_H_
#ifdef __cplusplus

#include <cppual/dom/dom.h>

using std::string;
using std::fstream;

namespace cppual {

class Xml final
{
public:
	typedef std::size_t	size_type;

	inline Xml () = default;
	string getAttrib (string const&, string const&);
	bool   open (string const&);
	bool   open (fstream const&);
	void   setAttrib (string const&, string const&, string const&);
	void   addElement (string const&);
	void   setElement (string const&, string const&);
	void   appendElement (string const&, string const&);
	void   insertElement (string const&, string const&, bool = true);

	inline ~Xml () { if (m_gFile.is_open ()) m_gFile.close (); }
	inline void close () { if (m_gFile.is_open ()) m_gFile.close (); }
	inline bool isOpen () const noexcept { return m_gFile.is_open (); }

	inline Xml (fstream const& /*gStream*/, bool bIndent = true)
	: m_gFile (),
	  m_bIndent (bIndent)
	{ }

	inline Xml (Xml const& gObj) noexcept
	: m_gFile (), m_bIndent (gObj.m_bIndent)
	{ }

	inline
	explicit Xml (string const& gFilePath, bool bIndent = true)
	: m_gFile (gFilePath),
	  m_bIndent (bIndent)
	{ }

private:
	fstream m_gFile;
	bool	m_bIndent;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FORMAT_XML_H_
