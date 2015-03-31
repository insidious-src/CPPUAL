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

#ifndef CPPUAL_PROCESS_OBJECT_H_
#define CPPUAL_PROCESS_OBJECT_H_
#ifdef __cplusplus

#include <thread>

using std::thread;

namespace cppual {

class ThreadObject
{
public:
    inline ThreadObject () noexcept
    : m_uThreadOwner (std::this_thread::get_id ())
    { }

    inline virtual ~ThreadObject ()
    { }

    inline thread::id threadOwner () const noexcept
    { return m_uThreadOwner; }

private:
    thread::id const m_uThreadOwner;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_PROCESS_OBJECT_H_
