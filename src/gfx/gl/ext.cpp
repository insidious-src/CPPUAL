/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

#include <cppual/gfx/gl/ext.h>
#include <cstring>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

using string;

namespace cppual { namespace Graphics { namespace GL {

bool isExtSupported (cchar* extension) noexcept
{
    cchar* list  = IGLContext::current ()->version ().major == 2 ?
                       (cchar*) glGetString (GL_EXTENSIONS) : nullptr;
    cchar* where = strchr (extension, ' ');
    cchar* terminator;

    /* Extension names should not have spaces. */
    if (where or *extension == '\0') return false;

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    while ((where = strstr (list, extension)) != nullptr)
    {
        terminator = where + strlen (extension);

        if (where == list or *(where - 1) == ' ')
        {
            if (*terminator == ' ' or *terminator == '\0')
                return true;
        }

        list = terminator;
    }

    return false;
}

bool loadExtension (string const&)
{
    return false;
}

} } } // GL
