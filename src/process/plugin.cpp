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

#include <cppual/process/plugin.h>
#include <cppual/types.h>

#include <iostream>

#ifdef OS_STD_POSIX
#   include <dlfcn.h>
#elif defined (OS_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

namespace cppual { namespace process {

// =========================================================

namespace { // optimize for internal unit usage

static dyn_loader::string_type ext() noexcept
{
#   if defined (OS_GNU_LINUX) || defined (OS_BSD) || defined (OS_ANDROID)
    return ".so";
#   elif defined (OS_STD_MAC)
    return ".dylib";
#   elif defined (OS_WINDOWS)
    return ".dll";
#   else
    return dyn_loader::string_type();
#   endif
}

static dyn_loader::string_type format (dyn_loader::string_type const& path) noexcept
{
    static const auto _ext = ext();

    const auto pos = path.rfind(_ext);

    return pos == dyn_loader::string_type::npos ? path + _ext : path;
}

} // anonymous namespace

// =========================================================

dyn_loader::dyn_loader (string_type   strPath,
                        bool          bAttach,
                        resolve_policy eResolve) noexcept
: _M_pHandle  (),
  _M_gLibPath (format(strPath)),
  _M_eResolve (eResolve)
{
    if (bAttach) attach ();
}

dyn_loader::string_type dyn_loader::extension () noexcept
{
    return ext();
}

dyn_loader::dyn_loader (dyn_loader&& obj) noexcept
: _M_pHandle  (obj._M_pHandle),
  _M_gLibPath (std::move (obj._M_gLibPath)),
  _M_eResolve (obj._M_eResolve)
{
    obj._M_pHandle = nullptr;
}

dyn_loader& dyn_loader::operator = (dyn_loader&& obj) noexcept
{
    if (this != &obj)
    {
        _M_pHandle  = obj._M_pHandle;
        _M_gLibPath = std::move (obj._M_gLibPath);
        _M_eResolve = obj._M_eResolve;

        obj._M_pHandle = nullptr;
    }

    return *this;
}

bool dyn_loader::attach () noexcept
{
    if (_M_pHandle) return true;
    int nLibMode = 0;

#   ifdef OS_STD_POSIX

    switch (_M_eResolve)
    {
    case resolve_policy::statically:
        nLibMode = RTLD_GLOBAL;
        break;
    case resolve_policy::immediate:
        nLibMode = RTLD_NOW;
        break;
    case resolve_policy::lazy:
        nLibMode = RTLD_LAZY;
        break;
    }

    _M_pHandle = ::dlopen (_M_gLibPath.c_str (), nLibMode);
    if (!_M_pHandle) std::cerr << ::dlerror () << std::endl;

#   elif defined (OS_WINDOWS)

    switch (_M_eResolve)
    {
    case resolve_policy::statically:
        nLibMode |= LOAD_LIBRARY_AS_DATAFILE;
        break;
    case resolve_policy::lazy:
        nLibMode |= DONT_RESOLVE_DLL_REFERENCES;
        break;
    case resolve_policy::immediate:
        break;
    }

    nLibMode |= LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;

    _M_pHandle = ::LoadLibraryEx (_M_gLibPath.data (), nullptr, nLibMode);
    if (!_M_pHandle) std::cerr << ::GetLastError () << std::endl;

#   endif

    return _M_pHandle;
}

void dyn_loader::detach () noexcept
{
    if (!_M_pHandle) return;

#    ifdef OS_STD_POSIX

    if (::dlclose (_M_pHandle) != 0) std::cerr << ::dlerror () << std::endl;

#    elif defined (OS_WINDOWS)

    if (!::FreeLibrary (_M_pHandle.get<HMODULE> ()))
        std::cerr << ::GetLastError () << std::endl;

#    endif

    _M_pHandle = nullptr;
}

void* dyn_loader::get_address (string_type::const_pointer pName) const noexcept
{
#   ifdef OS_STD_POSIX

    void* pAddr = ::dlsym (_M_pHandle, pName);
    if (!pAddr) std::cerr << ::dlerror () << std::endl;
    return pAddr;

#   elif defined (OS_WINDOWS)

    auto const convert = direct_cast<void*> (::GetProcAddress (_M_pHandle.get<HMODULE> (), pName));

    if (!convert)
        std::cerr << "error: " << ::GetLastError () << "\naddress not found!" << std::endl;
    return convert;

#   endif
}

dyn_loader::function_type
dyn_loader::get_function (string_type::const_pointer pName) const noexcept
{
#   ifdef OS_STD_POSIX

    auto const convert = direct_cast<function_type> (::dlsym (_M_pHandle, pName));

    if (!convert) std::cerr << ::dlerror () << std::endl;
    return convert;

#   elif defined (OS_WINDOWS)

    function_type func = ::GetProcAddress (_M_pHandle.get<HMODULE> (), pName);
    if (!func)
        std::cerr << "error: " << ::GetLastError () << "\nfunction not found!" << std::endl;
    return func;

#   endif
}

} } // namespace Process
