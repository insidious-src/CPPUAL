/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/gfx/matrix.h>

namespace cppual { namespace Graphics {

Matrix2& Matrix2::transpose () noexcept
{
    std::swap ((*this)[1], (*this)[2]);
    return *this;
}

// =========================================================

Matrix3& Matrix3::transpose () noexcept
{
    std::swap ((*this)[1], (*this)[3]);
    std::swap ((*this)[2], (*this)[6]);
    std::swap ((*this)[5], (*this)[7]);

    return *this;
}

// =========================================================

Matrix4& Matrix4::transpose () noexcept
{
    std::swap ((*this)[ 1], (*this)[ 4]);
    std::swap ((*this)[ 2], (*this)[ 8]);
    std::swap ((*this)[ 3], (*this)[12]);
    std::swap ((*this)[ 6], (*this)[ 9]);
    std::swap ((*this)[ 7], (*this)[13]);
    std::swap ((*this)[11], (*this)[14]);

    return *this;
}

} } // namespace Graphics
