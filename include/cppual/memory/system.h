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

#ifndef CPPUAL_MEMORY_MODEL_H_
#define CPPUAL_MEMORY_MODEL_H_
#ifdef __cplusplus

#include <cppual/types.h>

#include <cstddef>

namespace cppual { namespace Memory {

static_assert (sizeof (std::size_t) == sizeof (u8*),
               "size_t is not equal to the size of a pointer!");

static_assert (alignof (std::size_t) == alignof (u8*),
               "The alignment is wrong!");

// =========================================================

std::size_t size        (); // system memory size
std::size_t maxSize     (); // largest available system memory block
std::size_t workingSize (); // process' current memory usage

// =========================================================

namespace Model {

#if defined(__clang__) || defined(__GNUC__)
# define MALLOC_ATTRIBUTE __attribute__((__malloc__))
# define MALLOC_CALL
#elif defined(_MSC_VER)
# define MALLOC_ATTRIBUTE
# define MALLOC_CALL __declspec(restrict)
#else
# define MALLOC_ATTRIBUTE
# define MALLOC_CALL
#endif

//! Flag to rpaligned_realloc to not preserve content in reallocation
#define NO_PRESERVE 1

typedef struct GlobalStatistics {
    //! Current amount of virtual memory mapped (only if ENABLE_STATISTICS=1)
    std::size_t mapped;
    //! Current amount of memory in global caches for small and medium sizes (<64KiB)
    std::size_t cached;
    //! Current amount of memory in global caches for large sizes (>=64KiB)
    std::size_t cached_large;
    //! Total amount of memory mapped (only if ENABLE_STATISTICS=1)
    std::size_t mapped_total;
    //! Total amount of memory unmapped (only if ENABLE_STATISTICS=1)
    std::size_t unmapped_total;
} GlobalStatistics;

typedef struct ThreadStatistics {
    //! Amount of memory currently requested in allocations (only if ENABLE_STATISTICS=1)
    std::size_t requested;
    //! Amount of memory actually allocated in memory blocks (only if ENABLE_STATISTICS=1)
    std::size_t allocated;
    //! Current number of bytes available for allocation from active spans
    std::size_t active;
    //! Current number of bytes available in thread size class caches
    std::size_t sizecache;
    //! Current number of bytes available in thread span caches
    std::size_t spancache;
    //! Current number of bytes in pending deferred deallocations
    std::size_t deferred;
    //! Total number of bytes transitioned from thread cache to global cache
    std::size_t thread_to_global;
    //! Total number of bytes transitioned from global cache to thread cache
    std::size_t global_to_thread;
} ThreadStatistics;

extern int
initialize(void);

extern void
finalize(void);

extern void
thread_initialize(void);

extern void
thread_reset(void);

extern void
thread_collect(void);

extern int
is_thread_initialized(void);

extern void
thread_statistics(ThreadStatistics* stats);

extern void
global_statistics(GlobalStatistics* stats);

extern MALLOC_CALL void*
alloc(std::size_t size) MALLOC_ATTRIBUTE;

extern void
free(void* ptr);

extern MALLOC_CALL void*
calloc(std::size_t num, std::size_t size) MALLOC_ATTRIBUTE;

extern void*
realloc(void* ptr, std::size_t size);

extern void*
aligned_realloc(void* ptr, std::size_t alignment, std::size_t size, std::size_t oldsize, uint flags);

extern MALLOC_CALL void*
aligned_alloc(std::size_t alignment, std::size_t size) MALLOC_ATTRIBUTE;

extern std::size_t
alloc_usable_size(void* ptr);

} } } // namespace Model

#endif // __cplusplus
#endif // CPPUAL_MEMORY_MODEL_H_
