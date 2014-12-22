/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <iostream>
#include <experimental/optional>
#include <cppual/process/module.h>

#ifdef OS_STD_POSIX
#	include <dlfcn.h>
#elif defined (OS_WINDOWS)
#	include <Windows.h>
#endif

using std::string;

namespace { // optimize for unit internal usage

constexpr const char* shared_ext () noexcept
{
#	if defined (OS_GNU_LINUX) or defined (OS_BSD)
	return ".so";
#	elif defined (OS_STD_MAC)
	return ".dylib";
#	elif defined (OS_WINDOWS)
	return ".dll";
#	endif
}

} // anonymous namespace

namespace cppual { namespace Process {

Module::Module (cchar*        pPath,
				bool          bAttach,
				ResolvePolicy eResolve,
				Flags         gFlags) noexcept
: m_pHandle (),
  m_gLibPath (gFlags.hasBit (AddExt) ? std::move (string (pPath) += shared_ext ()) : pPath),
  m_eResolve (eResolve)
{
	if (bAttach) attach ();
}

bool Module::attach () noexcept
{
	if (m_pHandle) return true;
	int nLibMode = 0;

#	ifdef OS_STD_POSIX

	switch (m_eResolve)
	{
	case ResolvePolicy::Static:
		nLibMode = RTLD_GLOBAL;
		break;
	case ResolvePolicy::Immediate:
		nLibMode = RTLD_NOW;
		break;
	default:
		nLibMode = RTLD_LAZY;
		break;
	}

	m_pHandle = dlopen (m_gLibPath.c_str (), nLibMode);
	if (!m_pHandle) std::cerr << dlerror () << std::endl;

#	elif defined (OS_WINDOWS)

	switch (m_eResolve)
	{
	case ResolvePolicy::Static:
		nLibMode |= LOAD_LIBRARY_AS_DATAFILE;
		break;
	case ResolvePolicy::Unresolved:
		nLibMode |= DONT_RESOLVE_DLL_REFERENCES;
		break;
	}

	nLibMode |= SearchPolicy::Default ?
						LOAD_LIBRARY_SEARCH_DEFAULT_DIRS :
						LOAD_WITH_ALTERED_SEARCH_PATH;

	m_pHandle = LoadLibraryEx (m_gLibPath.data (), nullptr, nLibMode);
	if (!m_pHandle) std::cerr << GetLastError () << std::endl;

#	endif

	return m_pHandle;
}

void Module::detach () noexcept
{
	if (!m_pHandle) return;

#	ifdef OS_STD_POSIX

	if (dlclose (m_pHandle) != 0) std::cerr << dlerror () << std::endl;

#	elif defined (OS_WINDOWS)

	if (!FreeLibrary (m_pHandle)) std::cerr << GetLastError () << std::endl;;

#	endif

	m_pHandle = nullptr;
}

void* Module::address (cchar* pName) const noexcept
{
#	ifdef OS_STD_POSIX

	void* pAddr = dlsym (m_pHandle, pName);
	if (!pAddr) std::cerr << dlerror () << std::endl;
	return pAddr;

#	elif defined (OS_WINDOWS)

	void* pAddr = GetProcAddress (m_pHandle, pName);
	if (!pAddr) std::cerr << GetLastError () << std::endl;
	return pAddr;

#	endif
}

} } // namespace Process
