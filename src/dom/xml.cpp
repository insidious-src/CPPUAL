/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_TEXT_MEMORY_STRING_H_
#define CPPUAL_TEXT_MEMORY_STRING_H_
#ifdef __cplusplus

#include <cppual/dom/xml.h>
#include <cppual/decl.h>

using std::string;
using std::fstream;

namespace cppual {

bool Xml::open (string const& gFilePath)
{
    if (m_gFile.is_open ()) return false;
    m_gFile.open (gFilePath);
    return m_gFile.is_open ();
}

bool Xml::open (fstream const& /*gStream*/)
{
    if (!m_gFile.is_open ())
    {
    }

    return false;
}

string Xml::getAttrib (string const& /*gElem*/, string const& /*gAttrib*/)
{
    if (m_gFile.is_open ())
    {
    }

    return "";
}

void Xml::setAttrib (string const& /*gElem*/,
                      string const& /*gAttrib*/,
                      string const& /*gValue*/)
{
    if (m_gFile.is_open ())
    {
    }
}

void Xml::addElement (string const& gElem)
{
    if (m_gFile.is_open ())
    {
        string gElement;

        gElement.reserve (gElem.size () + 3);
        gElement  = TXT ("<");
        gElement += gElem;
        gElement += TXT (">");
        m_gFile << gElement;

        gElement  = TXT ("</");
        gElement += gElem;
        gElement += TXT (">");
        m_gFile << gElement;
    }
}

void Xml::setElement (string const& /*gElem*/, string const& /*gValue*/)
{
}

void Xml::appendElement (string const& /*gParentElem*/, string const& /*gElem*/)
{

}

void Xml::insertElement (string const& /*gTargetElem*/,
                          string const& /*gElem*/,
                          bool /*bAfter*/)
{
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TEXT_MEMORY_STRING_H_
