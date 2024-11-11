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

#ifndef CPPUAL_ERROR_H_
#define CPPUAL_ERROR_H_
#ifdef __cplusplus

namespace cppual {

struct error
{
    enum type
    {
        already               =   0,
        failed                =  -1,
        invalid               =  -2,
        denied                =  -3,
        no_resources          =  -4,
        no_access             =  -5,
        terminated            =  -6,
        incomplete            =  -7,
        not_supported         =  -8,
        connection_lost       =  -9,
        undefined             = -10
    };
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_ERROR_H_
