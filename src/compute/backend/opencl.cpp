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


#include "opencl.h"

#include <cppual/compute/backend_iface.h>

#include <cstring>

namespace cppual { namespace Compute { namespace CL {

std::string Platform::info (Info nfo)
{
    std::size_t n = 0;
    std::string text;

    ::clGetPlatformInfo (handle (), nfo, 0, nullptr, &n);
    text.resize (n);
    ::clGetPlatformInfo (handle (), nfo, n, &text[0], nullptr);
    text.resize (--n);

    return text;
}

// =========================================================

bool Platform::has_extension (cchar* pFeature)
{
    std::size_t uSize;

    if (::clGetPlatformInfo (handle (), PlatformExtensions,
                             0, nullptr, &uSize) != CL_SUCCESS or !uSize)
        return false;

    char text[uSize]; // VLA

    if (::clGetPlatformInfo (handle (), PlatformExtensions,
                             uSize, &text[0], nullptr) != CL_SUCCESS)
        return false;

    text[--uSize] = 0;
    text[--uSize] = 0;

    for (cchar* pEnd = ::strchr (text, ' '), *pBegin = text; ;
         pEnd = ::strchr (pBegin = ++pEnd, ' '))
    {
        if (!pEnd) return !::strcmp (pBegin, pFeature) ? true : false;
        else
        {
            text[pEnd - text] = 0;
            if (!::strcmp (pBegin, pFeature)) return true;
        }
    }

    // return false;
}

} } } // namespace CL
