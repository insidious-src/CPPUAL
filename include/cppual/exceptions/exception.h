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

#ifndef CPPUAL_EXCEPTION_H_
#define CPPUAL_EXCEPTION_H_
#ifdef __cplusplus

#include <exception>
#include <stdexcept>
#include <typeinfo>

namespace cppual {

// =========================================================

using std::exception;
using std::bad_exception;
using std::bad_cast;
using std::bad_typeid;
using std::logic_error;
using std::runtime_error;
using std::length_error;
using std::domain_error;
using std::invalid_argument;
using std::out_of_range;
using std::overflow_error;
using std::nested_exception;

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_EXCEPTION_H_