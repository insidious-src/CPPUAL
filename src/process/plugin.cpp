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

#include <cppual/process/plugin.h>
#include <iostream>

#ifdef OS_STD_POSIX
#   include <dlfcn.h>
#elif defined (OS_WINDOWS)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
namespace cppual { namespace Process {

namespace { // optimize for internal unit usage

constexpr const char* lib_ext () noexcept
{
#    if defined (OS_GNU_LINUX) or defined (OS_BSD)
    return ".so";
#    elif defined (OS_STD_MAC)
    return ".dylib";
#    elif defined (OS_WINDOWS)
    return ".dll";
#    endif
}

} // anonymous namespace

DynLoader::DynLoader (string_type   pPath,
                      bool          bAttach,
                      ResolvePolicy eResolve) noexcept
: m_pHandle  (),
  m_gLibPath (pPath + lib_ext ()),
  m_eResolve (eResolve)
{
    if (bAttach) attach ();
}

bool DynLoader::attach () noexcept
{
    if (m_pHandle) return true;
    int nLibMode = 0;

#   ifdef OS_STD_POSIX

    switch (m_eResolve)
    {
    case ResolvePolicy::Static:
        nLibMode = RTLD_GLOBAL;
        break;
    case ResolvePolicy::Immediate:
        nLibMode = RTLD_NOW;
        break;
    case ResolvePolicy::Lazy:
        nLibMode = RTLD_LAZY;
        break;
    }

    m_pHandle = ::dlopen (m_gLibPath.c_str (), nLibMode);
    if (!m_pHandle) std::cerr << ::dlerror () << std::endl;

#   elif defined (OS_WINDOWS)

    switch (m_eResolve)
    {
    case ResolvePolicy::Static:
        nLibMode |= LOAD_LIBRARY_AS_DATAFILE;
        break;
    case ResolvePolicy::Lazy:
        nLibMode |= DONT_RESOLVE_DLL_REFERENCES;
        break;
    case ResolvePolicy::Immediate:
        break;
    }

    nLibMode |= LOAD_LIBRARY_SEARCH_DEFAULT_DIRS;

    m_pHandle = ::LoadLibraryEx (m_gLibPath.data (), nullptr, nLibMode);
    if (!m_pHandle) std::cerr << ::GetLastError () << std::endl;

#   endif

    return m_pHandle;
}

void DynLoader::detach () noexcept
{
    if (!m_pHandle) return;

#    ifdef OS_STD_POSIX

    if (::dlclose (m_pHandle) != 0) std::cerr << ::dlerror () << std::endl;

#    elif defined (OS_WINDOWS)

    if (!::FreeLibrary (m_pHandle.get<HMODULE> ()))
        std::cerr << ::GetLastError () << std::endl;

#    endif

    m_pHandle = nullptr;
}

void* DynLoader::get_address (string_type::const_pointer pName) const noexcept
{
#   ifdef OS_STD_POSIX

    void* pAddr = ::dlsym (m_pHandle, pName);
    if (!pAddr) std::cerr << ::dlerror () << std::endl;
    return pAddr;

#   elif defined (OS_WINDOWS)

    union { function_type func; void* obj; } convert;
    convert.func = ::GetProcAddress (m_pHandle.get<HMODULE> (), pName);
    if (!convert.func)
        std::cerr << "error: " << ::GetLastError () << "\naddress not found!\n";
    return convert.obj;

#   endif
}

DynLoader::function_type
DynLoader::get_function (string_type::const_pointer pName) const noexcept
{
#   ifdef OS_STD_POSIX

    union { void* obj; function_type func; } convert;
    convert.obj = ::dlsym (m_pHandle, pName);
    if (!convert.obj) std::cerr << ::dlerror () << std::endl;
    return convert.func;

#   elif defined (OS_WINDOWS)

    function_type func = ::GetProcAddress (m_pHandle.get<HMODULE> (), pName);
    if (!func)
        std::cerr << "error: " << ::GetLastError () << "\nfunction not found!\n";
    return func;

#   endif
}

} } // namespace Process
