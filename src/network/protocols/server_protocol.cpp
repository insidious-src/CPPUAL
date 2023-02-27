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

#include <cppual/network/protocols/server_protocol.h>
#include <cppual/byte.h>

#include <fstream>

namespace cppual { namespace network {

//================================================

data_protocol::data_protocol(string_type const& firstString, string_type const& secondString, Type type)
{
    _M_gHeader.firstStringSize  = static_cast<size_type>(firstString.size()) ;
    _M_gHeader.secondStringSize = static_cast<size_type>(secondString.size());
    _M_gHeader.type             = type;
    _M_gHeader.dataSize         = 0;

    if(type == Type::Upload)
    {
        std::ifstream file (firstString.c_str ());
        stream_type   bytes;

        if (file.is_open ())
        {
            file.seekg(0, std::ios::end);

            bytes.reserve (static_cast<stream_type::size_type>(file.tellg ()));
            bytes.insert (bytes.begin (), bytes.capacity (), byte());

            file.seekg (0);
            file >> &bytes[0];

            file.close ();
        }

        _M_gData.insert(_M_gData.end (), firstString.begin (), firstString.end ());
        _M_gData.insert(_M_gData.end (), secondString.begin (), secondString.end ());

        if(!bytes.empty ())
        {
            _M_gData.insert(_M_gData.end (), bytes.begin (), bytes.end ());
            _M_gHeader.dataSize = static_cast<size_type>(bytes.size());
        }
    }
    else
    {
        _M_gData.insert(_M_gData.end (), firstString.begin (), firstString.end ());
        _M_gData.insert(_M_gData.end (), secondString.begin (), secondString.end ());
    }
}

data_protocol::data_protocol(stream_type const& bytes)
{
    if (bytes.size() <= sizeof(header))
        throw std::runtime_error("data_protocol: bytes not enough!");

    stream_type hdr  (bytes.begin (), bytes.begin () + sizeof(header));
    stream_type data (bytes.begin () + sizeof(header), bytes.end ());

    _M_gHeader  = bytes_to_binary<header>(hdr.data ());
    _M_gData    = std::move(data);
}

data_protocol::stream_type data_protocol::operator ()() const
{
    char str[sizeof(header)];
    stream_type data (binary_to_bytes(_M_gHeader, str), str + sizeof(header));

    data.insert (data.end (), _M_gData.begin (), _M_gData.end ());
    return data;
}

data_protocol::size_type data_protocol::first_str_size() const noexcept
{
    return _M_gHeader.firstStringSize;
}

data_protocol::size_type data_protocol::second_str_size() const noexcept
{
    return _M_gHeader.secondStringSize;
}

data_protocol::size_type data_protocol::data_size() const noexcept
{
    return _M_gHeader.dataSize;
}

data_protocol::Type data_protocol::type() const noexcept
{
    return static_cast<Type>(_M_gHeader.type);
}

data_protocol::size_type data_protocol::header_size() const noexcept
{
    return static_cast<size_type>(sizeof(header));
}

data_protocol::size_type data_protocol::size() const noexcept
{
    return header_size() + first_str_size() + second_str_size() + data_size();
}

data_protocol::stream_type data_protocol::first_string() const noexcept
{
    stream_type first_string;

    first_string.reserve (first_str_size ());
    first_string.insert  (first_string.end (),
                          _M_gData.begin (),
                          _M_gData.begin () + stream_type::difference_type (first_str_size ()));

    return first_string;
}

data_protocol::stream_type data_protocol::second_string() const noexcept
{
    stream_type second_string;

    second_string.reserve (second_str_size ());
    second_string.insert  (second_string.end (),
                           _M_gData.begin () + stream_type::difference_type (first_str_size ()),
                           _M_gData.begin () + stream_type::difference_type (second_str_size ()));

    return second_string;
}

data_protocol::stream_type data_protocol::data() const noexcept
{
    stream_type data;

    data.reserve (data_size ());
    data.insert  (data.end (),
                  _M_gData.begin () + stream_type::difference_type (first_str_size () + second_str_size ()),
                  _M_gData.end   ());

    return data;
}

void data_protocol::set_data(stream_type const& bytes)
{
    std::copy(bytes.begin (),
              bytes.end (),
              _M_gData.begin () + stream_type::difference_type (first_str_size() + second_str_size()));

    _M_gHeader.dataSize = static_cast<size_type> (bytes.size());
}

void data_protocol::set_type(Type type) noexcept
{
    _M_gHeader.type = type;
}

//================================================

} } // Network