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

#ifndef CPPUAL_NONCOPYABLE_H_
#define CPPUAL_NONCOPYABLE_H_
#ifdef __cplusplus

namespace cppual {

struct NonConstructible
{
    NonConstructible () = delete;
    NonConstructible (NonConstructible const&) = delete;
    NonConstructible& operator = (NonConstructible const&) = delete;
};

// ====================================================

struct NonDefaultConstructible
{
    NonDefaultConstructible () = delete;
};

// ====================================================

struct NonDefaultConstructibleVirtual
{
    NonDefaultConstructibleVirtual () = delete;
    inline virtual ~NonDefaultConstructibleVirtual () { }
};

// ====================================================

struct NonCopyable
{
    constexpr NonCopyable () = default;
    NonCopyable (NonCopyable const&) = delete;
    NonCopyable& operator = (NonCopyable const&) = delete;
};

// ====================================================

struct NonCopyableVirtual
{
    constexpr NonCopyableVirtual () = default;
    NonCopyableVirtual (NonCopyableVirtual const&) = delete;
    NonCopyableVirtual& operator = (NonCopyableVirtual const&) = delete;
    virtual ~NonCopyableVirtual () { }
};

// ====================================================

struct NonCopyConstructible
{
    constexpr NonCopyConstructible () = default;
    NonCopyConstructible (NonCopyConstructible const&) = delete;
};

// ====================================================

struct NonCopyConstructibleVirtual
{
    constexpr NonCopyConstructibleVirtual () = default;
    NonCopyConstructibleVirtual (NonCopyConstructibleVirtual const&) = delete;
    virtual ~NonCopyConstructibleVirtual () noexcept { }
};

// ====================================================

struct NonCopyAssignable
{
    constexpr NonCopyAssignable () = default;
    NonCopyAssignable& operator = (NonCopyAssignable  const&) = delete;
};

// ====================================================

struct NonCopyAssignableVirtual
{
    constexpr NonCopyAssignableVirtual () = default;
    NonCopyAssignableVirtual& operator = (NonCopyAssignableVirtual const&) = delete;
    virtual ~NonCopyAssignableVirtual () noexcept { }
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_NONCOPYABLE_H_
