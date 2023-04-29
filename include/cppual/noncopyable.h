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

#ifndef CPPUAL_NONCOPYABLE_H_
#define CPPUAL_NONCOPYABLE_H_
#ifdef __cplusplus

namespace cppual {

struct non_constructible
{
    non_constructible () = delete;
    non_constructible (non_constructible const&) = delete;
    non_constructible& operator = (non_constructible const&) = delete;
};

// ====================================================

struct non_default_constructible
{
    non_default_constructible () = delete;
};

// ====================================================

struct non_default_constructible_virtual
{
    non_default_constructible_virtual () = delete;
    inline virtual ~non_default_constructible_virtual () { }
};

// ====================================================

struct non_copyable
{
    constexpr non_copyable () = default;
    non_copyable (non_copyable const&) = delete;
    non_copyable& operator = (non_copyable const&) = delete;
};

// ====================================================

struct non_copyable_virtual
{
    constexpr non_copyable_virtual () = default;
    non_copyable_virtual (non_copyable_virtual const&) = delete;
    non_copyable_virtual& operator = (non_copyable_virtual const&) = delete;
    virtual ~non_copyable_virtual () { }
};

// ====================================================

struct non_copyable_movable
{
    constexpr non_copyable_movable () = default;
    non_copyable_movable (non_copyable&&) = delete;
    non_copyable_movable (non_copyable const&) = delete;
    non_copyable_movable& operator = (non_copyable&&) = delete;
    non_copyable_movable& operator = (non_copyable_movable const&) = delete;
};

// ====================================================

struct non_copyable_movable_virtual
{
    constexpr non_copyable_movable_virtual () = default;
    non_copyable_movable_virtual (non_copyable_movable_virtual&&) = delete;
    non_copyable_movable_virtual (non_copyable_movable_virtual const&) = delete;
    non_copyable_movable_virtual& operator = (non_copyable_movable_virtual&&) = delete;
    non_copyable_movable_virtual& operator = (non_copyable_movable_virtual const&) = delete;
    virtual ~non_copyable_movable_virtual () { }
};

// ====================================================

struct non_copy_constructible
{
    constexpr non_copy_constructible () = default;
    non_copy_constructible (non_copy_constructible const&) = delete;
};

// ====================================================

struct non_copy_constructible_virtual
{
    constexpr non_copy_constructible_virtual () = default;
    non_copy_constructible_virtual (non_copy_constructible_virtual const&) = delete;
    virtual ~non_copy_constructible_virtual () noexcept { }
};

// ====================================================

struct non_copy_assignable
{
    constexpr non_copy_assignable () = default;
    non_copy_assignable& operator = (non_copy_assignable  const&) = delete;
};

// ====================================================

struct non_copy_assignable_virtual
{
    constexpr non_copy_assignable_virtual () = default;
    non_copy_assignable_virtual& operator = (non_copy_assignable_virtual const&) = delete;
    virtual ~non_copy_assignable_virtual () noexcept { }
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_NONCOPYABLE_H_
