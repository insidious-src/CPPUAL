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

#ifndef CPPUAL_MESH_INTERFACE_H_
#define CPPUAL_MESH_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/noncopyable>

// =========================================================

namespace cppual {

// =========================================================

class layer_interface : public non_copyable_virtual
{
public:
    typedef layer_interface self_type;

    void              add_upper_node (self_type*);
    void              add_lower_node (self_type*);
    self_type*        uppest_node    () const;
    self_type*        lowest_node    () const;

    constexpr self_type* upper_node () const noexcept { return _M_pUpperProt; }
    constexpr self_type* lower_node () const noexcept { return _M_pLowerProt; }

private:
    self_type* _M_pUpperProt;
    self_type* _M_pLowerProt;
};

// =========================================================

class list_interface : public non_copyable_virtual
{
public:
    typedef list_interface self_type;

    void              add_next_node (self_type*);
    void              add_prev_node (self_type *);
    self_type*        first_node    () const;
    self_type*        last_node     () const;

    constexpr self_type* next_node () const noexcept { return _M_pNextProt; }
    constexpr self_type* prev_node () const noexcept { return _M_pPrevProt; }

private:
    self_type* _M_pNextProt;
    self_type* _M_pPrevProt;
};

// =========================================================

class grid_interface : public layer_interface, public list_interface
{
public:
    typedef grid_interface  self_type ;
    typedef layer_interface layer_type;
    typedef list_interface  list_type ;

    void add_next_interface  (self_type* layer) noexcept
    { add_next_node (layer); }

    void add_prev_interface  (self_type* layer) noexcept
    { add_prev_node (layer); }

    void add_upper_interface (self_type* layer) noexcept
    { add_upper_node (layer); }

    void add_lower_interface (self_type* layer) noexcept
    { add_lower_node (layer); }

    self_type* last_interface () const noexcept
    { return static_cast<self_type*> (last_node ()); }

    self_type* fist_interface () const noexcept
    { return static_cast<self_type*> (first_node ()); }

    self_type* uppest_interface () const noexcept
    { return static_cast<self_type*> (uppest_node ()); }

    self_type* lowest_interface () const noexcept
    { return static_cast<self_type*> (lowest_node ()); }

    constexpr self_type* upper_interface () const noexcept
    { return static_cast<self_type*> (upper_node ()); }

    constexpr self_type* lower_interface () const noexcept
    { return static_cast<self_type*> (lower_node ()); }

    constexpr self_type* next_interface  () const noexcept
    { return static_cast<self_type*> (next_node ()); }

    constexpr self_type* prev_interface  () const noexcept
    { return static_cast<self_type*> (prev_node ()); }
};

// =========================================================

} // cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_MESH_INTERFACE_H_
