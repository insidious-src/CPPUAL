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

#ifndef CPPUAL_CONTAINERS_H_
#define CPPUAL_CONTAINERS_H_
#ifdef __cplusplus

#include <cppual/memory/allocator.h>

#include <vector>
#include <deque>

#include <list>
#include <forward_list>

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>

namespace cppual {

// ====================================================

template <typename T>
using vector =
std::vector<T, memory::allocator<T>>;

template <typename T>
using deque =
std::deque<T, memory::allocator<T>>;

template <typename T>
using list =
std::list<T, memory::allocator<T>>;

template <typename T>
using forward_list =
std::forward_list<T, memory::allocator<T>>;

template <class Key, class Tp, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>>
using unordered_map =
std::unordered_map<Key, Tp, Hash, Pred, memory::allocator<std::pair<const Key, Tp>>>;

template <class Key, class Tp, class Hash = std::hash<Key>, class Pred = std::equal_to<Key>>
using unordered_multimap =
std::unordered_multimap<Key, Tp, Hash, Pred, memory::allocator<std::pair<const Key, Tp>>>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_set =
std::unordered_set<T, Hash, Pred, memory::allocator<T>>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_multiset =
std::unordered_multiset<T, Hash, Pred, memory::allocator<T>>;

template <class Key, class Tp, class Compare = std::less<Key>>
using map =
std::map<Key, Compare, memory::allocator<std::pair<const Key, Tp>>>;

template <class Key, class Tp, class Compare = std::less<Key>>
using multimap =
std::multimap<Key, Compare, memory::allocator<std::pair<const Key, Tp>>>;

template <class Key, class Compare = std::less<Key>>
using set =
std::set<Key, Compare, memory::allocator<Key>>;

template <class Key, class Compare = std::less<Key>>
using multiset =
std::multiset<Key, Compare, memory::allocator<Key>>;

// ====================================================

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_CONTAINERS_H_
