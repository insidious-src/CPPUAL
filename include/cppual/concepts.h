/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
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

// ====================================================

template <typename T>
using FundamentalType = typename std::enable_if_t<std::is_fundamental_v<T>, T>;

template <typename T>
using IntegralType = typename std::enable_if_t<std::is_integral_v<T>, T>;

template <typename T>
using FloatingPoint = typename std::enable_if_t<std::is_floating_point_v<T>, T>;

template <typename T>
using FunctionType = typename
std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<T>> ||
std::is_member_function_pointer_v<T>, T>;

template <typename T>
using LambdaNonCapturePtr = typename
std::enable_if_t<std::is_constructible
<member_function_to_static_t<decltype(&std::decay<T>::type::operator())>, T>
::value>*;

template <typename T>
using LambdaCapturePtr = typename
std::enable_if_t<!std::is_constructible
<member_function_to_static_t<decltype(&std::decay<T>::type::operator())>, T>
::value>*;

template <typename T>
using CallableType = typename
std::enable_if_t<std::is_constructible
<member_function_to_static_t<decltype(&std::decay<T>::type::operator())>, T>
::value ||
!std::is_constructible
<member_function_to_static_t<decltype(&std::decay<T>::type::operator())>, T>
::value, typename std::decay<T>::type>;

// ====================================================

template <typename T>
using Arithmetic = typename std::enable_if_t<std::is_arithmetic_v<T>, T>;

template <typename T>
using Scalar = typename std::enable_if_t<std::is_scalar_v<T>, T>;

template <typename T>
using Compound = typename std::enable_if_t<std::is_compound_v<T>, T>;

template <typename T>
using Reference = typename std::enable_if_t<std::is_reference_v<T>, T>;

template <typename T>
using LValueReference = typename std::enable_if_t<std::is_lvalue_reference_v<T>, T>;

template <typename T>
using RValueReference = typename std::enable_if_t<std::is_rvalue_reference_v<T>, T>;

template <typename T>
using CharType = typename std::enable_if_t<is_char_v<T>, T>;

template <typename T>
using EnumType = typename std::enable_if_t<std::is_enum_v<T>, T>;

template <typename T>
using ArrayType = typename std::enable_if_t<std::is_array_v<T>, T>;

template <typename T>
using SignedType = typename std::enable_if_t<std::is_signed_v<T>, T>;

template <typename T>
using UnsignedType = typename std::enable_if_t<std::is_unsigned_v<T>, T>;

template <typename T>
using UnionType = typename std::enable_if_t<std::is_union_v<T>, T>;

template <typename T>
using NullPointer = typename std::enable_if_t<std::is_null_pointer_v<T>, T>;

template <typename T>
using ObjectType = typename std::enable_if_t<std::is_object_v<T>, T>;

template <typename T>
using ClassType = typename std::enable_if_t<std::is_class_v<T>, T>;

template <typename T>
using AbstractType = typename std::enable_if_t<std::is_abstract_v<T>, T>;

template <typename T>
using PointerType = typename std::enable_if_t<std::is_pointer_v<T>, T>;

template <typename T>
using MemberPointer = typename std::enable_if_t<std::is_member_pointer_v<T>, T>;

template <typename T>
using MemberObjectPointer = typename std::enable_if_t<std::is_member_object_pointer_v<T>, T>;

// ====================================================

template <typename T>
using Constructible = typename std::enable_if_t<std::is_constructible_v<T>, T>;

template <typename T>
using DefaultConstructible = typename std::enable_if_t<std::is_default_constructible_v<T>, T>;

template <typename T>
using Destructible = typename std::enable_if_t<std::is_destructible_v<T>, T>;

template <typename T>
using TriviallyDestructible = typename std::enable_if_t<std::is_trivially_destructible_v<T>, T>;

template <typename T>
using VirtuallyDestructible = typename std::enable_if_t<std::has_virtual_destructor_v<T>, T>;

template <typename T>
using CopyableMovable = typename
std::enable_if_t<std::is_copy_constructible_v<T> &&
std::is_copy_assignable_v<T> &&
std::is_move_constructible_v<T> &&
std::is_move_assignable_v<T>, T>;

template <typename T>
using Copyable = typename
std::enable_if_t<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>, T>;

template <typename T>
using CopyAssignable = typename std::enable_if_t<std::is_copy_assignable_v<T>, T>;

template <typename T>
using CopyConstructible = typename std::enable_if_t<std::is_copy_constructible_v<T>, T>;

template <typename T>
using Movable = typename
std::enable_if_t<std::is_move_constructible_v<T> &&
std::is_move_assignable_v<T>, T>;

template <typename T>
using MoveConstructible = typename std::enable_if_t<std::is_move_constructible_v<T>, T>;

template <typename T>
using MoveAssignable = typename std::enable_if_t<std::is_move_assignable_v<T>, T>;

// ====================================================

template <typename T>
using ConstType = typename std::enable_if_t<std::is_const_v<T>, T>;

template <typename T>
using VolatileType = typename std::enable_if_t<std::is_volatile_v<T>, T>;

template <typename T>
using StandartLayoutType = typename std::enable_if_t<std::is_standard_layout_v<T>, T>;

template <typename T>
using TrivialType = typename std::enable_if_t<std::is_trivial_v<T>, T>;

template <typename T>
using PODType = typename std::enable_if_t<std::is_pod_v<T>, T>;

// ====================================================

//template <typename T>
//using Container = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using ReversibleContainer = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using SequenceContainer = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using AssociativeContainer = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using UnorderedAssociativeContainer = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using AllocatorAwareContainer = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Erasable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using DefaultInsertable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using CopyInsertable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using MoveInsertable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using EmplaceConstructible = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

// ====================================================

//template <typename T>
//using EqualityComparable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using LessThanComparable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Swappable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using ValueSwappable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Sortable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Mergeable = typename
//std::enable_if_t<std::is_standard_layout_v<T>, T>;

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CONCEPTS_H_
