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

#ifndef CPPUAL_CONTAINERS_H_
#define CPPUAL_CONTAINERS_H_
#ifdef __cplusplus

#include <cppual/memory/allocator.h>

#include <unordered_map>
#include <unordered_set>
#include <forward_list>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <set>
#include <map>

// ====================================================

namespace cppual {

// ====================================================

template <typename T, std::size_t N>
using array = std::array<T, N>;

template <typename T>
using dyn_array = std::vector<T, memory::allocator<T>>;

template <typename T>
using deque = std::deque<T, memory::allocator<T>>;

template <typename T>
using list = std::list<T, memory::allocator<T>>;

template <typename T>
using forward_list = std::forward_list<T, memory::allocator<T>>;

template <class K, class V, class Hash = std::hash<K>, class Pred = std::equal_to<K>>
using unordered_map =
std::unordered_map<K, V, Hash, Pred, memory::allocator<std::pair<K const, V>>>;

template <class K, class V, class Hash = std::hash<K>, class Pred = std::equal_to<K>>
using unordered_multimap =
std::unordered_multimap<K, V, Hash, Pred, memory::allocator<std::pair<K const, V>>>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_set =
std::unordered_set<T, Hash, Pred, memory::allocator<T>>;

template <class T, class Hash = std::hash<T>, class Pred = std::equal_to<T>>
using unordered_multiset =
std::unordered_multiset<T, Hash, Pred, memory::allocator<T>>;

template <class Key, class Tp, class Compare = std::less<Key>>
using map =
std::map<Key, Compare, memory::allocator<std::pair<Key const, Tp>>>;

template <class Key, class Tp, class Compare = std::less<Key>>
using multimap =
std::multimap<Key, Compare, memory::allocator<std::pair<Key const, Tp>>>;

template <class Key, class Compare = std::less<Key>>
using set = std::set<Key, Compare, memory::allocator<Key>>;

template <class Key, class Compare = std::less<Key>>
using multiset = std::multiset<Key, Compare, memory::allocator<Key>>;

// ====================================================

typedef std::basic_string<char  , std::char_traits<char>  , memory::allocator<char>>   string    ;
typedef std::basic_string<char8 , std::char_traits<char8> , memory::allocator<char8>>  u8string  ;
typedef std::basic_string<char16, std::char_traits<char16>, memory::allocator<char16>> u16string ;
typedef std::basic_string<char32, std::char_traits<char32>, memory::allocator<char32>> u32string ;
typedef std::basic_string<wchar , std::char_traits<wchar> , memory::allocator<wchar>>  wstring   ;

// ====================================================

typedef std::basic_istringstream<char  , std::char_traits<char>  , memory::allocator<char>>
    istringstream;
typedef std::basic_istringstream<char8 , std::char_traits<char8> , memory::allocator<char8>>
    u8istringstream;
typedef std::basic_istringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
    u16istringstream;
typedef std::basic_istringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
    u32istringstream;
typedef std::basic_istringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
    wistringstream;

// ====================================================

typedef std::basic_ostringstream<char  , std::char_traits<char>  , memory::allocator<char>>
    ostringstream;
typedef std::basic_ostringstream<char8 , std::char_traits<char8> , memory::allocator<char8>>
    u8ostringstream;
typedef std::basic_ostringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
    u16ostringstream;
typedef std::basic_ostringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
    u32ostringstream;
typedef std::basic_ostringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
    wostringstream;

// ====================================================

typedef std::basic_stringstream<char  , std::char_traits<char>  , memory::allocator<char>>
    stringstream;
typedef std::basic_stringstream<char8 , std::char_traits<char8> , memory::allocator<char8>>
    u8stringstream;
typedef std::basic_stringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
    u16stringstream;
typedef std::basic_stringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
    u32stringstream;
typedef std::basic_stringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
    wstringstream;

// ====================================================

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_CONTAINERS_H_
