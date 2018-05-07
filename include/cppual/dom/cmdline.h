/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#ifndef CPPUAL_CMDLINE_H_
#define CPPUAL_CMDLINE_H_
#if defined (_MSC_VER) and (_MSC_VER > 1000)
#pragma once
#endif
#ifdef __cplusplus

#include <cppual/mstring.h>

namespace cppual
{

class CommandLine
{
protected:
    ushort m_uArgCount;
    ctchar* m_pArgArray;

    CommandLine& operator = (const CommandLine&);
    bool IsSpace ();
    ctchar* IsolateQuotes ();

    void SetArgCount (ctchar* pArgArray) { m_uArgCount = CalcLength (pArgArray); }

public:
    ctchar* Parse (ctchar*);
    short FindArgument (ctchar*);

    CommandLine (ctchar* pArgArray) : m_uArgCount (), m_pArgArray (Parse (pArgArray)) { }
    ushort GetArgCount () const { return (m_uArgCount); }
    ctchar GetArgument (uint uIndex) const { return (m_pArgArray[uIndex]); }
    ctchar operator [] (uint uIndex) const { return (m_pArgArray[uIndex]); }
};

inline bool CommandLine::IsSpace ()
{
    return false;
}

inline ctchar* CommandLine::IsolateQuotes ()
{
    return 0;
}

ctchar* CommandLine::Parse (ctchar* pArray)
{
    return 0;
}

inline short CommandLine::FindArgument (ctchar* pArgArray)
{
    for (ushort n = 0; n < m_uArgCount; ++n) if (m_pArgArray[n] == pArgArray) return n;
    return -1;
}

} /* cppual */

#endif /* __cplusplus */
#endif /* CPPUAL_CMDLINE_H_ */
