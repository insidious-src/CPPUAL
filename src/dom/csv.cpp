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

#include <cppual/dom/csv.h>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace cppual {

bool csv_parser::append(string_type const& strFilePath)
{
    std::fstream file (strFilePath.c_str());
    string_type  line;

    if(!file.is_open ()) return false;

    // optimize memory allocation
    auto lineCount = std::count (std::istreambuf_iterator<char> (file),
                                 std::istreambuf_iterator<char> (), '\n');

    _M_data.reserve (static_cast<size_type> (lineCount) + 1);
    file.seekg (0);

    // read each line
    while (!file.eof ())
    {
        std::getline (file, line);

        if (line == "") continue;

        std::vector<string_type> record;
        istringstream       stream (line);

        // optimize memory allocation
        auto fieldCount = std::count (std::istreambuf_iterator<char> (stream),
                                      std::istreambuf_iterator<char> (), ',');

        record.reserve (static_cast<size_type> (fieldCount) + 1);
        stream.seekg (0);

        // read each field
        while (stream)
        {
            string_type field;

            if (!getline (stream, field, ',')) break;

            for (auto filter_idx = field.find_first_of('\"');
                 filter_idx != string_type::npos;
                 filter_idx  = field.find_first_of('\"'))
            {
                field = field.replace(filter_idx, 1, "");
            }

            record.push_back (field);
        }

        _M_data.push_back (std::move(record));
    }

    return true;
}

} // namespace cppual
