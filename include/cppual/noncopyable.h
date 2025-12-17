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

#ifndef CPPUAL_NON_COPYABLE_H_
#define CPPUAL_NON_COPYABLE_H_
#ifdef __cplusplus

namespace cppual {

// ====================================================

struct empty_constructible { };

// ====================================================

struct non_constructible
{
private:
    non_constructible () = delete;
    non_constructible (non_constructible &&)     = delete;
    non_constructible (non_constructible const&) = delete;
};

// ====================================================

struct non_constructible_virtual
{
public:
    constexpr virtual ~non_constructible_virtual () = default;

private:
    non_constructible_virtual () = delete;
    non_constructible_virtual (non_constructible_virtual &&)     = delete;
    non_constructible_virtual (non_constructible_virtual const&) = delete;
};

// ====================================================

struct non_default_constructible
{
private:
    non_default_constructible () = delete;
};

// ====================================================

struct non_default_constructible_virtual
{
public:
    constexpr virtual ~non_default_constructible_virtual () = default;

private:
    non_default_constructible_virtual () = delete;
};

// ====================================================

struct non_copyable
{
public:
    constexpr non_copyable () = default;

private:
    non_copyable (non_copyable const&) = delete;
    non_copyable& operator = (non_copyable const&) = delete;
};

// ====================================================

struct non_copyable_virtual
{
public:
    constexpr non_copyable_virtual () = default;
    constexpr virtual ~non_copyable_virtual () = default;

private:
    non_copyable_virtual (non_copyable_virtual const&) = delete;
    non_copyable_virtual& operator = (non_copyable_virtual const&) = delete;
};

// ====================================================

struct non_copyable_movable
{
public:
    constexpr non_copyable_movable () = default;

private:
    non_copyable_movable (non_copyable_movable &&) = delete;
    non_copyable_movable (non_copyable_movable const&) = delete;
    non_copyable_movable& operator = (non_copyable_movable &&) = delete;
    non_copyable_movable& operator = (non_copyable_movable const&) = delete;
};

// ====================================================

struct non_copyable_movable_virtual
{
public:
    constexpr          non_copyable_movable_virtual () = default;
    constexpr virtual ~non_copyable_movable_virtual () = default;

private:
    non_copyable_movable_virtual (non_copyable_movable_virtual &&) = delete;
    non_copyable_movable_virtual (non_copyable_movable_virtual const&) = delete;
    non_copyable_movable_virtual& operator = (non_copyable_movable_virtual &&) = delete;
    non_copyable_movable_virtual& operator = (non_copyable_movable_virtual const&) = delete;
};

// ====================================================

struct non_copy_constructible
{
public:
    constexpr non_copy_constructible () = default;

private:
    non_copy_constructible (non_copy_constructible const&) = delete;
};

// ====================================================

struct non_copy_constructible_virtual
{
public:
    constexpr non_copy_constructible_virtual () = default;
    constexpr virtual ~non_copy_constructible_virtual () = default;

private:
    non_copy_constructible_virtual (non_copy_constructible_virtual const&) = delete;
};

// ====================================================

struct non_copy_assignable
{
public:
    constexpr non_copy_assignable () = default;

private:
    non_copy_assignable& operator = (non_copy_assignable const&) = delete;
};

// ====================================================

struct non_copy_assignable_virtual
{
public:
    constexpr non_copy_assignable_virtual () = default;
    constexpr virtual ~non_copy_assignable_virtual () = default;

private:
    non_copy_assignable_virtual& operator = (non_copy_assignable_virtual const&) = delete;
};

// ====================================================

struct non_move_constructible
{
public:
    constexpr non_move_constructible () = default;

private:
    non_move_constructible (non_move_constructible&&) = delete;
};

// ====================================================

struct non_move_constructible_virtual
{
public:
    constexpr non_move_constructible_virtual () = default;
    constexpr virtual ~non_move_constructible_virtual () = default;

private:
    non_move_constructible_virtual (non_move_constructible_virtual&&) = delete;
};

// ====================================================

struct non_move_assignable
{
public:
    constexpr non_move_assignable () = default;

private:
    non_move_assignable& operator = (non_move_assignable&&) = delete;
};

// ====================================================

struct non_move_assignable_virtual
{
public:
    constexpr non_move_assignable_virtual () = default;
    constexpr virtual ~non_move_assignable_virtual () = default;

private:
    non_move_assignable_virtual& operator = (non_move_assignable_virtual&&) = delete;
};

// ====================================================

struct non_copy_move_constructible
{
public:
    constexpr non_copy_move_constructible () = default;
    constexpr non_copy_move_constructible& operator = (non_copy_move_constructible &&) = default;
    constexpr non_copy_move_constructible& operator = (non_copy_move_constructible const&) = default;

private:
    non_copy_move_constructible (non_copy_move_constructible &&) = delete;
    non_copy_move_constructible (non_copy_move_constructible const&) = delete;
};

// ====================================================

struct non_copy_move_constructible_virtual
{
public:
    constexpr non_copy_move_constructible_virtual () = default;
    constexpr virtual ~non_copy_move_constructible_virtual () = default;

    constexpr non_copy_move_constructible_virtual& operator = (non_copy_move_constructible_virtual &&) = default;
    constexpr non_copy_move_constructible_virtual& operator = (non_copy_move_constructible_virtual const&) = default;

private:
    non_copy_move_constructible_virtual (non_copy_move_constructible_virtual &&) = delete;
    non_copy_move_constructible_virtual (non_copy_move_constructible_virtual const&) = delete;
};

// ====================================================

struct non_copy_move_assignable
{
public:
    constexpr non_copy_move_assignable () = default;
    non_copy_move_assignable (non_copy_move_assignable &&) = default;
    non_copy_move_assignable (non_copy_move_assignable const&) = default;

private:
    non_copy_move_assignable& operator = (non_copy_move_assignable &&) = delete;
    non_copy_move_assignable& operator = (non_copy_move_assignable const&) = delete;
};

// ====================================================

struct non_copy_move_assignable_virtual
{
public:
    constexpr non_copy_move_assignable_virtual () = default;
    constexpr virtual ~non_copy_move_assignable_virtual () = default;

    constexpr non_copy_move_assignable_virtual (non_copy_move_assignable_virtual &&) = default;
    constexpr non_copy_move_assignable_virtual (non_copy_move_assignable_virtual const&) = default;

private:
    non_copy_move_assignable_virtual& operator = (non_copy_move_assignable_virtual &&) = delete;
    non_copy_move_assignable_virtual& operator = (non_copy_move_assignable_virtual const&) = delete;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_NON_COPYABLE_H_
