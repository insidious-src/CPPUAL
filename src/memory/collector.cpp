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

#include <cppual/memory/collector.h>

#ifdef OS_STD_UNIX
#	include <unistd.h>
#	include <sys/mman.h>
#	include <sys/resource.h>
//#	include <sys/types.h>
//#	include <sys/param.h>
#	ifdef OS_GNU_LINUX
#		include <fcntl.h>
#		include <stdio.h>
#	elif defined (OS_MAC)
#		include <mach/mach.h>
#	elif defined (OS_AIX) or defined (OS_SOLARIS) or defined (OS_SOLARIS)
#		include <fcntl.h>
#		include <procfs.h>
#	elif defined (OS_BSD)
#		include <sys/sysctl.h>
#	endif
#elif defined (OS_WINDOWS)
#	include <windows.h>
#	include <psapi.h>
#endif

namespace cppual { namespace Memory {

// Get size of the total physical memory installed
std::size_t size ()
{
#if defined(OS_WINDOWS) and (defined(__CYGWIN__) or defined(__CYGWIN32__))

	/* Cygwin under Windows. ------------------------------------ */
	/* New 64-bit MEMORYSTATUSEX isn't available.  Use old 32.bit */
	MEMORYSTATUS status;
	status.dwLength = sizeof (status);
	GlobalMemoryStatus (&status);
	return static_cast<std::size_t> (status.dwTotalPhys);

#elif defined(OS_WINDOWS)

	/* Windows. ------------------------------------------------- */
	/* Use new 64-bit MEMORYSTATUSEX, not old 32-bit MEMORYSTATUS */
	MEMORYSTATUSEX status;
	status.dwLength = sizeof (status);
	GlobalMemoryStatusEx (&status);
	return static_cast<std::size_t> (status.ullTotalPhys);

#elif defined (OS_STD_UNIX) or defined (OS_STD_MAC)

	/* UNIX variants. ------------------------------------------- */
	/* Prefer sysctl() over sysconf() except sysctl() HW_REALMEM and HW_PHYSMEM */

#if defined (CTL_HW) and (defined (HW_MEMSIZE) or defined (HW_PHYSMEM64))

	int mib[2];
	mib[0] = CTL_HW;
#if defined (HW_MEMSIZE)
	mib[1] = HW_MEMSIZE;            /* OSX. --------------------- */
#elif defined (HW_PHYSMEM64)
	mib[1] = HW_PHYSMEM64;          /* NetBSD, OpenBSD. --------- */
#endif
	int64 size = 0;               /* 64-bit */
	std::size_t len = sizeof (size);
	if (sysctl (mib, 2, &size, &len, nullptr, 0) == 0)
		return static_cast<std::size_t> (size);
	return 0;			/* Failed? */

#elif defined(_SC_AIX_REALMEM)
	/* AIX. ----------------------------------------------------- */
	return static_cast<std::size_t> (sysconf (_SC_AIX_REALMEM)) * 1024;

#elif defined (_SC_PHYS_PAGES) and defined (_SC_PAGESIZE)

	/* FreeBSD, Linux, OpenBSD, and Solaris. -------------------- */
	return static_cast<std::size_t> (sysconf (_SC_PHYS_PAGES) *
									 sysconf (_SC_PAGESIZE  ));

#elif defined (_SC_PHYS_PAGES) and defined (_SC_PAGE_SIZE)

	/* Legacy. -------------------------------------------------- */
	return static_cast<std::size_t> (sysconf (_SC_PHYS_PAGES) *
									 sysconf (_SC_PAGE_SIZE ));

#elif defined(CTL_HW) and (defined(HW_PHYSMEM) or defined(HW_REALMEM))
	/* DragonFly BSD, FreeBSD, NetBSD, OpenBSD, and OSX. -------- */
	int mib[2];
	mib[0] = CTL_HW;
#if defined(HW_REALMEM)
	mib[1] = HW_REALMEM;		/* FreeBSD. ----------------- */
#elif defined(HW_PYSMEM)
	mib[1] = HW_PHYSMEM;		/* Others. ------------------ */
#endif
	uint size = 0;		/* 32-bit */
	std::size_t len = sizeof (size);
	if (sysctl (mib, 2, &size, &len, NULL, 0) == 0) return size;
	return 0;			/* Failed? */
#endif /* sysctl and sysconf variants */

#else
	return 0;			/* Unknown OS. */
#endif
}

// =========================================================

// Get usable memory size
std::size_t maxSize ()
{
#	ifdef OS_GNU_LINUX
#	elif defined (OS_WINDOWS)
#	endif

	return 0;
}

// =========================================================

// Get size the current process allocated memory
std::size_t workingSize ()
{
#if defined (OS_WINDOWS)

	/* Windows -------------------------------------------------- */
	PROCESS_MEMORY_COUNTERS info;
	GetProcessMemoryInfo (GetCurrentProcess (), &info, sizeof (info));
	return static_cast<std::size_t> (info.WorkingSetSize);

#elif defined (OS_STD_MAC)

	/* OSX ------------------------------------------------------ */
	mach_task_basic_info info;
	mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
	if (task_info (mach_task_self ( ), MACH_TASK_BASIC_INFO,
				   (task_info_t)&info, &infoCount) != KERN_SUCCESS)
		return 0;		/* Can't access? */
	return static_cast<std::size_t> (info.resident_size);

#elif defined (OS_GNU_LINUX)

	/* Linux ---------------------------------------------------- */
	long  rss = 0;
	FILE* fp  = nullptr;

	if (!(fp = ::fopen ("/proc/self/statm", "r"))) return 0;

	if (::fscanf (fp, "%*s%ld", &rss) != 1)
	{
		::fclose (fp);
		return 0;		/* Can't read? */
	}

	::fclose (fp);
	return static_cast<std::size_t> (rss * sysconf (_SC_PAGESIZE));

#else

	/* AIX, BSD, Solaris, and Unknown OS ------------------------ */
	return 0;			/* Unsupported. */
#endif
}

} } // namespace Memory
