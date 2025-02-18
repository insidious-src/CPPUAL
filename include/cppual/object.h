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

#ifndef CPPUAL_PROCESS_OBJECT_H_
#define CPPUAL_PROCESS_OBJECT_H_
#ifdef __cplusplus

#include <thread>

namespace cppual {

class thread_object
{
public:
    typedef thread_object self_type  ;
    typedef std::thread   thread_type;

    inline thread_object () noexcept
    : _M_uThreadOwner (std::this_thread::get_id ())
    { }

    inline virtual ~thread_object ()
    { }

    inline thread_type::id thread_owner_id () const noexcept
    { return _M_uThreadOwner; }

private:
    thread_type::id const _M_uThreadOwner;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_PROCESS_OBJECT_H_
