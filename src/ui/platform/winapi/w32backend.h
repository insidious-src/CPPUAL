/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_WIN32_DISPLAY_H_
#define CPPUAL_PLATFORM_WIN32_DISPLAY_H_
#ifdef __cplusplus

#include <cppual/ui/display.h>
#ifdef   OS_WINDOWS

namespace cppual { namespace Ui {

struct DECL_EXPORT Win32Display final : public IDisplay
{
    Win32Display       () noexcept;
    ~Win32Display      () noexcept;
    string name        () const noexcept;
    uint   screenCount () const noexcept;
};

} } // namespace Input

#endif // OS_WINDOWS
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_WIN32_DISPLAY_H_