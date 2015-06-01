/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/compute/platform.h>
#include <iostream>
#include <cstring>
//#include "cldef.h"

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

//constexpr std::size_t const_hash (cchar* input) noexcept
//{
//    return *input ?
//                static_cast<unsigned int> (*input) + 33 * const_hash (input + 1) : 1;
//}

//inline CL::size_type infotype (Device::Info eType) noexcept
//{
//    switch (eType)
//    {
//    case Device::Info::Name:
//        return CL_DEVICE_NAME;
//    case Device::Info::Board:
//        return CL_DEVICE_VENDOR;
//    case Device::Info::Vendor:
//        return CL_DEVICE_VENDOR;
//    case Device::Info::Profile:
//        return CL_DEVICE_PROFILE;
//    case Device::Info::Version:
//        return CL_DEVICE_VERSION;
//    default:
//        return 0;
//    }
//}

//inline Device::uint_type infou (Device::uint_type uNfo,
//                                u8                uType,
//                                u16               uPlatform,
//                                Device::uint_type uId)
//{
//    Device::uint_type value;

//    if (::clGetDeviceInfo (CL::handle (uType, uPlatform, uId),
//                           uNfo, sizeof (Device::uint_type), &value, nullptr) != CL_SUCCESS)
//        return 0;

//    return value;
//}

//inline Device::size_type infoul (Device::uint_type uNfo,
//                                 u8                uType,
//                                 u16               uPlatform,
//                                 Device::uint_type uId)
//{
//    Device::size_type value;

//    if (::clGetDeviceInfo (CL::handle (uType, uPlatform, uId),
//                           uNfo, sizeof (Device::size_type), &value, nullptr) != CL_SUCCESS)
//        return 0;

//    return value;
//}

}

// anonymous

// =========================================================

Device::size_type Device::count () noexcept
{
    return 0;
}

Device& Device::host () noexcept
{
    static Device dev;
    return dev;
}

//Device::Device (Device const& gObj) noexcept
//: m_uPlatformId (gObj.m_uPlatformId),
//  m_eType (gObj.m_eType),
//  m_uId (gObj.m_uId),
//  m_uNumUnits (gObj.m_uNumUnits)
//{
//}

//Device::Device (Type eType, u16 uPlatform, uint_type uId)
//: m_uPlatformId (uPlatform),
//  m_eType (eType),
//  m_uId (uId),
//  m_uNumUnits (infou (CL::MaxUnits, m_eType, uPlatform, uId))
//{
//}

//Device::Partition Device::part (uint_type)
//{
//    return std::move (Partition (*this));
//}

//string Device::info (Info eInfo)
//{
//    static std::size_t n = 0;
//    static string      text;

//    if (m_uNumUnits)
//    {
//        CL::device_type dev = CL::handle (m_eType, m_uPlatformId, m_uId);

//        if (::clGetDeviceInfo (dev, infotype (eInfo), 0, nullptr, &n) != CL_SUCCESS)
//            return string ();

//        text.resize (n);

//        if (::clGetDeviceInfo (dev, infotype (eInfo), n, &text[0], nullptr) != CL_SUCCESS)
//            return string ();

//        text.resize (--n);
//    }

//    return std::move (text);
//}

//bool Device::available (cchar* feature)
//{
//    if (m_uNumUnits)
//    {
//        static size_type       uSize = 0;
//        const  CL::device_type dev   = CL::handle (m_eType, m_uPlatformId, m_uId);

//        if (::clGetDeviceInfo (dev, CL::PlatformExtensions,
//                               0, nullptr, &uSize) != CL_SUCCESS or !uSize)
//            return false;

//        char text[uSize];

//        if (::clGetDeviceInfo (dev, CL::PlatformExtensions,
//                               uSize, text, nullptr) != CL_SUCCESS)
//            return false;

//        text[--uSize] = 0;
//        text[--uSize] = 0;

//        for (cchar* pEnd = ::strchr (text, ' '), *pBegin = text; ;
//             pEnd = ::strchr (pBegin = ++pEnd, ' '))
//        {
//            if (!pEnd) return !::strcmp (pBegin, feature) ? true : false;
//            else
//            {
//                text[pEnd - text] = 0;
//                if (!::strcmp (pBegin, feature)) return true;
//            }
//        }
//    }

//    return false;
//}

//Device::uint_type Device::dimensions () const
//{
//    return infou (CL::MaxDimensions, m_eType, m_uPlatformId, m_uId);
//}

//Device::size_type Device::groupSize () const
//{
//    return infoul (CL::MaxGroupSize, m_eType, m_uPlatformId, m_uId);
//}

//Device::uint_type Device::items () const
//{
//    return infou (CL::MaxItemCount, m_eType, m_uPlatformId, m_uId);
//}

//Device::size_type Device::cache () const
//{
//    return infoul (CL::CacheSize, m_eType, m_uPlatformId, m_uId);
//}

//Device::uint_type Device::cacheLine () const
//{
//    return infou (CL::CacheLineSize, m_eType, m_uPlatformId, m_uId);
//}

//Device::size_type Device::localMemory () const
//{
//    return infoul (CL::LocalMemSize, m_eType, m_uPlatformId, m_uId);
//}

//Device::size_type Device::constMemory () const
//{
//    return infoul (CL::ConstMemSize, m_eType, m_uPlatformId, m_uId);
//}

//Device::size_type Device::globalMemory () const
//{
//    return infoul (CL::GlobalMemSize, m_eType, m_uPlatformId, m_uId);
//}

//// =========================================================

//Device::Partition::Partition (Device&)
//: Object ()
//{
//}

//Device::Partition::Partition (Device::Partition&& gObj)
//: Object (std::move (gObj))
//{
//}

//Device::Partition::Partition (Device::Partition const& gObj)
//: Object (gObj)
//{
//}

//Device::Partition& Device::Partition::operator = (Device::Partition&& gObj)
//{
//    if (this != &gObj)
//    {
//        Object::operator = (std::move (gObj));
//    }

//    return *this;
//}

//Device::Partition& Device::Partition::operator = (Device::Partition const& gObj)
//{
//    if (this != &gObj)
//    {
//        Object::operator = (gObj);
//    }

//    return *this;
//}

} } // Compute
