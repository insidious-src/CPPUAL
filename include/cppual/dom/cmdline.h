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

#ifndef CPPUAL_CMDLINE_H_
#define CPPUAL_CMDLINE_H_
#if defined (_MSC_VER) and (_MSC_VER > 1000)
#pragma once
#endif
#ifdef __cplusplus

#include <cppual/string.h>

namespace cppual {

// =========================================================

class command_line
{
public:
    static ctchar* parse (ctchar*);
    short find_argument (ctchar*);

    command_line (ctchar* pArgArray) : _M_uArgCount (), _M_pArgArray (parse (pArgArray)) { }
    ushort arg_count () const { return (_M_uArgCount); }
    ctchar argument (uint uIndex) const { return (_M_pArgArray[uIndex]); }
    ctchar operator [] (uint uIndex) const { return (_M_pArgArray[uIndex]); }

protected:
    command_line& operator = (command_line const&);

    bool is_space ();
    ctchar* isolate_quotes ();

    void set_arg_count (ctchar* /*pArgArray*/)
    { /*_M_uArgCount = CalcLength (pArgArray);*/ }

protected:
    ushort  _M_uArgCount;
    ctchar* _M_pArgArray;
};

inline bool command_line::is_space ()
{
    return false;
}

inline ctchar* command_line::isolate_quotes ()
{
    return nullptr;
}

ctchar* command_line::parse (ctchar* /*pArray*/)
{
    return nullptr;
}

inline short command_line::find_argument (ctchar* /*pArgArray*/)
{
    //for (ushort n = 0U; n < _M_uArgCount; ++n) if (_M_pArgArray[n] == pArgArray) return n;
    return -1;
}

// =========================================================

} /* cppual */

#endif /* __cplusplus */
#endif /* CPPUAL_CMDLINE_H_ */
