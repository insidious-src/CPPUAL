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

#ifndef CPPUAL_CSV_H
#define CPPUAL_CSV_H
#ifdef __cplusplus

#include <string>
#include <vector>

namespace cppual {

class CSVParser
{
public:
    typedef std::string                           string_type;
    typedef std::vector<std::vector<string_type>> vector_type;
    typedef std::size_t                           size_type  ;

    bool append (string_type const& file_path);

    CSVParser& operator += (string_type const& file_path)
    { append(file_path); return *this; }

    size_type lineCount () const noexcept
    { return m_data.size(); }

    size_type fieldCount () const noexcept
    { return m_data.empty() ? size_type () : m_data[0].size(); }

    template <std::size_t N>
    string_type get (size_type idx) const
    { return m_data[idx][N]; }

private:
    vector_type m_data;
};

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_CSV_H
