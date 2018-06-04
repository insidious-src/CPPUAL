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

#ifndef CPPUAL_GFX_GL_EXTENSION_H_
#define CPPUAL_GFX_GL_EXTENSION_H_
#ifdef __cplusplus

#include <string>
#include <cppual/gfx/gl/glbase.h>

using std::string;

namespace cppual { namespace Graphics { namespace GL {

bool isExtSupported (cchar* name) noexcept;
//bool isExtSupported (string const& name) noexcept;
bool loadExtension (string const& name);

} } } // GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_EXTENSION_H_
