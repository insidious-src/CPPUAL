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

#include <cppual/gfx/formats/bmp.h>

namespace cppual {

void BitmapStream::_parse_header ()
{
    //! check header sizes
    seekg (0, end);
    if (static_cast<size_type> (tellg ()) <=
                        sizeof (Header  ) + sizeof (InfoHeader) - 1)
        return;

    //! read Header
    seekg (0);
    read  (reinterpret_cast<char_type*> (&m_header),
                                 sizeof (Header   ));

    //! read InfoHeader
    if (header ().type == Type::BM)
        read (reinterpret_cast<char_type*> (&m_infoHeader),
                                    sizeof (InfoHeader   ));
    else
        close ();
}

BitmapStream::~BitmapStream()
{
}

BitmapStream::size_type BitmapStream::replace (rgb_type const target_clr, rgb_type const new_clr)
{
    if (!valid()) return size_type ();

    pointer pixel_data      (new rgb_type[info ().pixel_count ()]);
    auto    num_changes    = size_type ();
    auto    buffer_size    = info  ().pixel_count () * sizeof (rgb_type);
    auto    prev_seekg     = tellg ();
    auto    bin_target_clr = target_clr.binary ();
    auto    bin_new_clr    = new_clr   .binary ();

    //! skip the whole header and read the pixel data
    seekg (header ().offset);
    read  (reinterpret_cast<char_type*> (pixel_data.get ()),
                static_cast<streamsize> (buffer_size));

    //! read each row and while reading it replace each color cell
    for (u32 row_idx = 0; row_idx < info ().absolute_height (); ++row_idx)
    {
        for (u32 cell_idx = 0; cell_idx < info ().absolute_width (); ++cell_idx)
        {
            auto array_idx = info ().absolute_width () * row_idx + cell_idx;

            if (pixel_data[array_idx] == bin_target_clr)
            {
                pixel_data[array_idx]  = bin_new_clr;
                ++num_changes;
            }
        }
    }

    //! return to the beginning of the pixel data
    seekg (prev_seekg);

    //! if there are any changes - write the data
    if (num_changes)
    {
        auto prev_seekp = tellp ();

        seekp (header ().offset);
        write (reinterpret_cast<char_type*> (pixel_data.get ()),
                    static_cast<streamsize> (buffer_size));
        seekp (prev_seekp);
    }

    return num_changes;
}

} // namespace cppual

