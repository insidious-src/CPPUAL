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

#ifndef CPPUAL_PROCESS_PIPE_H_
#define CPPUAL_PROCESS_PIPE_H_
#ifdef __cplusplus

#include <string>
#include <vector>
#include <cppual/types.h>
#include <cppual/noncopyable.h>

using std::vector;
using std::string;
using std::wstring;
using std::u16string;
using std::u32string;

namespace cppual { namespace Process {

enum class PipeType : unsigned char
{
    Memory,
    Interprocess,
    Network
};

class Pipe : public NonCopyable
{
public:
    Pipe () = delete;
    Pipe (Pipe&&) noexcept;
    Pipe& operator = (Pipe&&) noexcept;
    ~Pipe () noexcept;

    void setBlocking (bool) noexcept;

    inline bool isBlocking () const noexcept
    { return m_bIsBlocking; }

private:
    bool m_bIsBlocking;
};

} } // namespace Process

#endif // __cplusplus
#endif // CPPUAL_PROCESS_PIPE_H_
