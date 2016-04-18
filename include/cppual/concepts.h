/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_CONCEPTS_H_
#define CPPUAL_CONCEPTS_H_
#ifdef __cplusplus

#include <cppual/meta.h>

namespace cppual {

template <typename T>
using FundamentalType = typename
std::enable_if<std::is_fundamental<T>::value, T>::type;

template <typename T>
using LiteralType = typename
std::enable_if<std::is_literal_type<T>::value, T>::type;

template <typename T>
using IntegralType = typename
std::enable_if<std::is_integral<T>::value, T>::type;

template <typename T>
using FloatingPoint = typename
std::enable_if<std::is_floating_point<T>::value, T>::type;

template <typename T>
using FunctionType = typename
std::enable_if<std::is_function<typename std::remove_pointer<T>::type>::value or
std::is_member_function_pointer<T>::value, T>::type;

template <typename T>
using CallableType = typename
std::enable_if<std::is_constructible
<typename member_function_to_static<decltype(&std::decay<T>::type::operator())>::type, T>
::value, T>::type;

// ====================================================

template <typename T>
using Arithmetic = typename
std::enable_if<std::is_arithmetic<T>::value, T>::type;

template <typename T>
using Scalar = typename
std::enable_if<std::is_scalar<T>::value, T>::type;

template <typename T>
using Compound = typename
std::enable_if<std::is_compound<T>::value, T>::type;

template <typename T>
using Reference = typename
std::enable_if<std::is_reference<T>::value, T>::type;

template <typename T>
using LValueReference = typename
std::enable_if<std::is_lvalue_reference<T>::value, T>::type;

template <typename T>
using RValueReference = typename
std::enable_if<std::is_rvalue_reference<T>::value, T>::type;

template <typename T>
using CharType = typename
std::enable_if<is_char<T>::value, T>::type;

template <typename T>
using EnumType = typename
std::enable_if<std::is_enum<T>::value, T>::type;

template <typename T>
using ArrayType = typename
std::enable_if<std::is_array<T>::value, T>::type;

template <typename T>
using SignedType = typename
std::enable_if<std::is_signed<T>::value, T>::type;

template <typename T>
using UnsignedType = typename
std::enable_if<std::is_unsigned<T>::value, T>::type;

template <typename T>
using UnionType = typename
std::enable_if<std::is_union<T>::value, T>::type;

template <typename T>
using NullPointer = typename
std::enable_if<std::is_null_pointer<T>::value, T>::type;

template <typename T>
using ObjectType = typename
std::enable_if<std::is_object<T>::value, T>::type;

template <typename T>
using ClassType = typename
std::enable_if<std::is_class<T>::value, T>::type;

template <typename T>
using AbstractType = typename
std::enable_if<std::is_abstract<T>::value, T>::type;

template <typename T>
using PointerType = typename
std::enable_if<std::is_pointer<T>::value, T>::type;

template <typename T>
using MemberPointer = typename
std::enable_if<std::is_member_pointer<T>::value, T>::type;

template <typename T>
using MemberObjectPointer = typename
std::enable_if<std::is_member_object_pointer<T>::value, T>::type;

// ====================================================

template <typename T>
using Constructible = typename
std::enable_if<std::is_constructible<T>::value, T>::type;

template <typename T>
using DefaultConstructible = typename
std::enable_if<std::is_default_constructible<T>::value, T>::type;

template <typename T>
using Destructible = typename
std::enable_if<std::is_destructible<T>::value, T>::type;

template <typename T>
using TriviallyDestructible = typename
std::enable_if<std::is_trivially_destructible<T>::value, T>::type;

template <typename T>
using VirtuallyDestructible = typename
std::enable_if<std::has_virtual_destructor<T>::value, T>::type;

template <typename T>
using CopyableMovable = typename
std::enable_if<std::is_copy_constructible<T>::value and
std::is_copy_assignable<T>::value and
std::is_move_constructible<T>::value and
std::is_move_assignable<T>::value, T>::type;

template <typename T>
using Copyable = typename
std::enable_if<std::is_copy_constructible<T>::value and
std::is_copy_assignable<T>::value, T>::type;

template <typename T>
using CopyAssignable = typename
std::enable_if<std::is_copy_assignable<T>::value, T>::type;

template <typename T>
using CopyConstructible = typename
std::enable_if<std::is_copy_constructible<T>::value, T>::type;

template <typename T>
using Movable = typename
std::enable_if<std::is_move_constructible<T>::value and
std::is_move_assignable<T>::value, T>::type;

template <typename T>
using MoveConstructible = typename
std::enable_if<std::is_move_constructible<T>::value, T>::type;

template <typename T>
using MoveAssignable = typename
std::enable_if<std::is_move_assignable<T>::value, T>::type;

// ====================================================

template <typename T>
using ConstType = typename
std::enable_if<std::is_const<T>::value, T>::type;

template <typename T>
using VolatileType = typename
std::enable_if<std::is_volatile<T>::value, T>::type;

template <typename T>
using StandartLayoutType = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using TrivialType = typename
std::enable_if<std::is_trivial<T>::value, T>::type;

template <typename T>
using PODType = typename
std::enable_if<std::is_pod<T>::value, T>::type;

// ====================================================

template <typename T>
using Container = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using ReversibleContainer = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using SequenceContainer = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using AssociativeContainer = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using UnorderedAssociativeContainer = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using AllocatorAwareContainer = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using Erasable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using DefaultInsertable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using CopyInsertable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using MoveInsertable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using EmplaceConstructible = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

// ====================================================

template <typename T>
using EqualityComparable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using LessThanComparable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using Swappable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using ValueSwappable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using Sortable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

template <typename T>
using Mergeable = typename
std::enable_if<std::is_standard_layout<T>::value, T>::type;

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CONCEPTS_H_
