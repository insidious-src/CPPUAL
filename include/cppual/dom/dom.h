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

#ifndef CPPUAL_FORMAT_DOM_H_
#define CPPUAL_FORMAT_DOM_H_
#ifdef __cplusplus

#include <string>
#include <map>
#include <list>

namespace cppual {

template <typename Key = char, typename Alloc = std::allocator<Key>>
class Element
{
public:
    static_assert (typeid (Key) == typeid (std::allocator_traits<Alloc>::value_type),
                   "Key allocator mismatch!");

    typedef std::basic_string<Key, std::char_traits<Key>, Alloc> string_type;

    string_type name;
    std::map<string_type, string_type, std::less<string_type> > attribs;
    std::list<Element> children;
};

class DOMParser
{
public:
    typedef std::string string_type;

    inline DOMParser () = default;

    inline
    explicit DOMParser (string_type const& gDelimBegin  = "<" ,
                        string_type const& gDelimBegin2 = "</",
                        string_type const& gDelimEnd    = "/>",
                        string_type const& gDelimEnd2   =  ">")
    : m_gDelimBegin  (gDelimBegin ),
      m_gDelimBegin2 (gDelimBegin2),
      m_gDelimEnd    (gDelimEnd   ),
      m_gDelimEnd2   (gDelimEnd2  )
    { }

private:
    string_type m_gDelimBegin, m_gDelimBegin2, m_gDelimEnd, m_gDelimEnd2;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FORMAT_DOM_H_
