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

#include <cppual/memory/system.h>
#include <cppual/memory/allocator.h>

#ifdef OS_GNU_LINUX
#   include "os/linux.h"
#elif defined (OS_MACX)
#   include "os/mac.h"
#elif defined (OS_AIX)
#   include "os/aix.h"
#elif defined (OS_SOLARIS)
#   include "os/solaris.h"
#elif defined (OS_BSD)
#   include "os/bsd.h"
#elif defined (OS_WINDOWS)
#   include "os/win.h"
#elif defined (OS_ANDROID)
#   include "os/android.h"
#elif defined (OS_IOS)
#   include "os/ios.h"
#endif

#include <new>
#include <atomic>
#include <memory>
#include <iostream>

#include <cstdlib>
#include <cstring>
#include <assert.h>

namespace { // internal unit optimization

using namespace cppual;

inline static void initializer ()
{
    static auto ret = memory::model::initialize ();

    UNUSED(ret);
    memory::model::thread_initialize ();
}

// =========================================================

class system_memory_resource final : public memory::memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    bool is_thread_safe () const noexcept
    {
        return true;
    }

    size_type max_size () const noexcept
    {
        auto const max = memory::max_size ();
        return max ? max : memory_resource::max_size();
    }

    size_type capacity () const noexcept
    {
        auto const cap = memory::capacity ();
        return cap ? cap : memory_resource::capacity();
    }

private:
    void* do_allocate(size_type bytes, size_type /*alignment*/)
    {
        if (!memory::model::is_thread_initialized ()) initializer ();
        return memory::model::allocate (bytes);
    }

    void* do_reallocate(void* p, size_type old_size, size_type new_size, size_type /*alignment*/)
    {
        if (!memory::model::is_thread_initialized ()) initializer ();
        return p != nullptr ? memory::model::reallocate(p, old_size, new_size) :
                              memory::model::allocate(new_size);
    }

    void do_deallocate(void* p, size_type /*bytes*/, size_type /*alignment*/)
    {
        if (!memory::model::is_thread_initialized ()) initializer ();
        memory::model::deallocate (p);
    }

    bool do_is_equal(base_type const& other) const noexcept
    {
        return this == &other;
    }
};

} // anonymous namespace

// =========================================================

#ifdef CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

void* operator new (std::size_t size)
{
    if (!cppual::memory::Model::is_thread_initialized ()) initializer ();
    return cppual::memory::Model::alloc (size);
}

void operator delete (void* ptr) noexcept
{
    if (cppual::memory::Model::is_thread_initialized ())
        cppual::memory::Model::free (ptr);
}

void*
operator new [] (std::size_t size)
{
    if (!cppual::memory::Model::is_thread_initialized ()) initializer ();
    return cppual::memory::Model::alloc (size);
}

void operator delete [] (void* ptr) noexcept
{
    if (!cppual::memory::Model::is_thread_initialized ()) initializer ();
    cppual::memory::Model::free (ptr);
}

void* operator new (std::size_t size, const std::nothrow_t&) noexcept
{
    if (!cppual::memory::Model::is_thread_initialized ()) initializer();
    return cppual::memory::Model::alloc (size);
}

void* operator new [] (std::size_t size, const std::nothrow_t&) noexcept
{
    if (!cppual::memory::Model::is_thread_initialized ()) initializer();
    return cppual::memory::Model::alloc (size);
}

void operator delete (void* ptr, const std::nothrow_t&) noexcept
{
    if (cppual::memory::Model::is_thread_initialized ())
        cppual::memory::Model::free (ptr);
}

void operator delete [] (void* ptr, const std::nothrow_t&) noexcept
{
    if (cppual::memory::Model::is_thread_initialized ())
        cppual::memory::Model::free (ptr);
}

void operator delete (void* ptr, std::size_t) noexcept
{
    if (cppual::memory::Model::is_thread_initialized ())
        cppual::memory::Model::free (ptr);
}

void operator delete [] (void* ptr, std::size_t) noexcept
{
    if (cppual::memory::Model::is_thread_initialized ())
        cppual::memory::Model::free (ptr);
}

#endif // CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

// =========================================================

namespace cppual { namespace memory {

// =========================================================

memory_resource* system_resource ()
{
    static system_memory_resource rc;
    return &rc;
}

// =========================================================

std::size_t capacity ()
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

#if defined (CTL_HW) && (defined (HW_MEMSIZE) or defined (HW_PHYSMEM64))

    int mib[2];
    mib[0] = CTL_HW;
#if defined (HW_MEMSIZE)
    mib[1] = HW_MEMSIZE;            /* OSX. --------------------- */
#elif defined (HW_PHYSMEM64)
    mib[1] = HW_PHYSMEM64;          /* NetBSD, OpenBSD. --------- */
#endif
    i64 size = 0;               /* 64-bit */
    std::size_t len = sizeof (size);
    if (sysctl (mib, 2, &size, &len, nullptr, 0) == 0)
        return static_cast<std::size_t> (size);

    std::cerr << __func__ << " :: Error :: failed to read system memory size!" << std::endl;
    return 0;            /* Failed? */

#elif defined(_SC_AIX_REALMEM)
    /* AIX. ----------------------------------------------------- */
    return static_cast<std::size_t> (sysconf (_SC_AIX_REALMEM)) * 1024;

#elif defined (_SC_PHYS_PAGES) && defined (_SC_PAGESIZE)

    /* FreeBSD, Linux, OpenBSD, and Solaris. -------------------- */
    return static_cast<std::size_t> (sysconf (_SC_PHYS_PAGES) *
                                     sysconf (_SC_PAGESIZE  ));

#elif defined (_SC_PHYS_PAGES) && defined (_SC_PAGE_SIZE)

    /* Legacy. -------------------------------------------------- */
    return static_cast<std::size_t> (sysconf (_SC_PHYS_PAGES) *
                                     sysconf (_SC_PAGE_SIZE ));

#elif defined(CTL_HW) and (defined(HW_PHYSMEM) or defined(HW_REALMEM))
    /* DragonFly BSD, FreeBSD, NetBSD, OpenBSD, and OSX. -------- */
    int mib[2];
    mib[0] = CTL_HW;
#if defined(HW_REALMEM)
    mib[1] = HW_REALMEM;        /* FreeBSD. ----------------- */
#elif defined(HW_PYSMEM)
    mib[1] = HW_PHYSMEM;        /* Others. ------------------ */
#endif
    u32 size = 0;        /* 32-bit */
    std::size_t len = sizeof (size);
    if (sysctl (mib, 2, &size, &len, NULL, 0) == 0) return size;

    std::cerr << __func__ << " :: Error :: failed to read system memory size!" << std::endl;
    return 0;            /* Failed? */
#endif /* sysctl and sysconf variants */

#else
    return 0;            /* Unknown OS. */
#endif
}

// =========================================================

std::size_t max_size ()
{
#   ifdef OS_GNU_LINUX
#   elif defined (OS_WINDOWS)
#   endif

    return capacity();
}

// =========================================================

std::size_t working_size ()
{
#if defined (OS_WINDOWS)

    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo (GetCurrentProcess (), &info, sizeof (info));
    return static_cast<std::size_t> (info.WorkingSetSize);

#elif defined (OS_STD_APPLE)

    /* OSX ------------------------------------------------------ */
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info (mach_task_self (), MACH_TASK_BASIC_INFO,
                   (task_info_t)&info, &infoCount) != KERN_SUCCESS)
    {
        std::cerr << __func__ << " :: Error :: Cannot access working memory size!" << std::endl;
        return 0;        /* Can't access? */
    }

    return static_cast<std::size_t> (info.resident_size);

#elif defined (OS_STD_UNIX)

    /* Linux ---------------------------------------------------- */
    long  rss = 0;
    FILE* fp  = nullptr;

    if (!(fp = ::fopen ("/proc/self/statm", "r")))
    {
        std::cerr << __func__ << " :: Error :: Cannot access '/proc/self/statm'!" << std::endl;
        return 0;
    }

    if (::fscanf (fp, "%*s%ld", &rss) != 1)
    {
        ::fclose (fp);
        std::cerr << __func__ << " :: Error :: Cannot read '/proc/self/statm'!" << std::endl;
        return 0;        /* Can't read? */
    }

    ::fclose (fp);
    return static_cast<std::size_t> (rss * sysconf (_SC_PAGESIZE));

#else

    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    std::cerr << __func__ << " :: Error :: unsupported OS!" << std::endl;
    return 0;            /* Unsupported. */
#endif
}

#if defined(OS_ANDROID) || defined(OS_IOS)
#define USE_PTHREADS_OVER_TLS 1
#endif

#ifdef USE_PTHREADS_OVER_TLS
#define THREAD_LOCAL static_assert(false, "Thread Local Storage is unsupported on this platform! Use Pthreads.");
#elif defined (OS_MACX)
#define THREAD_LOCAL __thread
#else
#define THREAD_LOCAL thread_local
#endif

#ifdef USE_PTHREADS_OVER_TLS
#include <pthread.h>
#define DEFINE_THREAD_LOCAL(TYPE, NAME) \
        pthread_key_t NAME; \
        static pthread_once_t NAME##OnceControl = PTHREAD_ONCE_INIT; \
        void Initialize##NAME##TLS() { pthread_key_create(&NAME, nullptr); }
#define INITIALIZE_THREAD_LOCAL(NAME) pthread_once(&NAME##OnceControl, &Initialize##NAME##TLS);
#define FREE_THREAD_LOCAL(NAME) pthread_key_delete(NAME);
#define GET_THREAD_LOCAL(TYPE, NAME) ((TYPE)pthread_getspecific(NAME))
#define GET_THREAD_LOCAL_LITERAL_VALUE(TYPE, NAME) ((TYPE)((size_t)pthread_getspecific(NAME)))
#define SET_THREAD_LOCAL(NAME, VALUE) pthread_setspecific(NAME, (void*)VALUE);
#else
#define DEFINE_THREAD_LOCAL(TYPE, NAME) THREAD_LOCAL TYPE NAME
#define INITIALIZE_THREAD_LOCAL(NAME)
#define FREE_THREAD_LOCAL(NAME)
#define GET_THREAD_LOCAL(TYPE, NAME) NAME
#define GET_THREAD_LOCAL_LITERAL_VALUE(TYPE, NAME) NAME
#define SET_THREAD_LOCAL(NAME, VALUE) NAME = VALUE
#endif

// Build time configurable limits

// Presets, if none is defined it will default to performance priority
//#define ENABLE_UNLIMITED_CACHE
//#define DISABLE_CACHE
#define ENABLE_SPACE_PRIORITY_CACHE

// Presets for cache limits
#if defined(ENABLE_UNLIMITED_CACHE)
// Unlimited caches
#define MIN_SPAN_CACHE_RELEASE 16
#define MAX_SPAN_CACHE_DIVISOR 1
#elif defined(DISABLE_CACHE)
//Disable cache
#define MIN_SPAN_CACHE_RELEASE 1
#define MAX_SPAN_CACHE_DIVISOR 0
#elif defined(ENABLE_SPACE_PRIORITY_CACHE)
// Space priority cache limits
#define MIN_SPAN_CACHE_SIZE 8
#define MIN_SPAN_CACHE_RELEASE 8
#define MAX_SPAN_CACHE_DIVISOR 16
#define GLOBAL_SPAN_CACHE_MULTIPLIER 1
#else
// Default - performance priority cache limits
//! Limit of thread cache in number of spans for each page count class (undefine for unlimited cache - i.e never release spans to global cache unless thread finishes)
//! Minimum cache size to remain after a release to global cache
#define MIN_SPAN_CACHE_SIZE 8
//! Minimum number of spans to transfer between thread and global cache
#define MIN_SPAN_CACHE_RELEASE 16
//! Maximum cache size divisor (max cache size will be max allocation count divided by this divisor)
#define MAX_SPAN_CACHE_DIVISOR 8
//! Multiplier for global span cache limit (max cache size will be calculated like thread cache and multiplied with this)
#define GLOBAL_SPAN_CACHE_MULTIPLIER 4
#endif

//! Size of heap hashmap
#define HEAP_ARRAY_SIZE           79

#ifndef ENABLE_VALIDATE_ARGS
//! Enable validation of args to public entry points
#define ENABLE_VALIDATE_ARGS      0
#endif

#if ENABLE_VALIDATE_ARGS && defined(OS_WINDOWS)
#include <Intsafe.h>
#endif

#ifndef ENABLE_STATISTICS
//! Enable statistics collection
#define ENABLE_STATISTICS         0
#endif

//! Memory page size
#define MALLOC_PAGE_SIZE        512

//! Granularity of all memory page spans for small & medium block allocations
#define SPAN_ADDRESS_GRANULARITY  8192
//! Maximum size of a span of memory pages
#define SPAN_MAX_SIZE             (SPAN_ADDRESS_GRANULARITY)
//! Mask for getting the start of a span of memory pages
#define SPAN_MASK                 (~(static_cast<uptr>(SPAN_MAX_SIZE) - 1))
//! Maximum number of memory pages in a span
#define SPAN_MAX_PAGE_COUNT       (SPAN_MAX_SIZE / MALLOC_PAGE_SIZE)

#define QUICK_ALLOCATION_PAGES_COUNT SPAN_MAX_PAGE_COUNT
#define SPANS_PER_SEGMENT 32

//! Granularity of a small allocation block
#define SMALL_GRANULARITY         16
//! Small granularity shift count
#define SMALL_GRANULARITY_SHIFT   4
//! Number of small block size classes
#define SMALL_CLASS_COUNT         (((MALLOC_PAGE_SIZE - SPAN_HEADER_SIZE) >> 1) >> SMALL_GRANULARITY_SHIFT)
//! Maximum size of a small block
#define SMALL_SIZE_LIMIT          (SMALL_CLASS_COUNT * SMALL_GRANULARITY)

//! Granularity of a medium allocation block
#define MEDIUM_GRANULARITY        32
//! Medimum granularity shift count
#define MEDIUM_GRANULARITY_SHIFT  5
//! Number of medium block size classes
#define MEDIUM_CLASS_COUNT        60
//! Maximum size of a medium block
#define MEDIUM_SIZE_LIMIT         (SMALL_SIZE_LIMIT + (MEDIUM_GRANULARITY * MEDIUM_CLASS_COUNT) - SPAN_HEADER_SIZE)

//! Total number of small + medium size classes
#define SIZE_CLASS_COUNT          (SMALL_CLASS_COUNT + MEDIUM_CLASS_COUNT)

//! Number of large block size classes
#define LARGE_CLASS_COUNT         4
//! Maximum number of memory pages in a large block
#define LARGE_MAX_PAGES           (SPAN_MAX_PAGE_COUNT * LARGE_CLASS_COUNT)
//! Maximum size of a large block
#define LARGE_SIZE_LIMIT          ((LARGE_MAX_PAGES * MALLOC_PAGE_SIZE) - SPAN_HEADER_SIZE)

#define SPAN_LIST_LOCK_TOKEN      (reinterpret_cast<void*>(1))

#define SINGLE_SEGMENT_MARKER 1

#define pointer_offset(ptr, ofs) static_cast<void*>(reinterpret_cast<char*>(ptr) + static_cast<std::ptrdiff_t>(ofs))
#define pointer_diff(first, second) reinterpret_cast<ptrdiff_t>(reinterpret_cast<char*>(first) - reinterpret_cast<cchar*>(second))

//! Size of a span header
#define SPAN_HEADER_SIZE          48 // Not exactly right, but keeps memory 16-aligned

#if defined (ARCH_64BITS)
typedef i64 offset_t;
#else
typedef i32 offset_t;
#endif
typedef u32 count_t;

#if ENABLE_VALIDATE_ARGS
//! Maximum allocation size to avoid integer overflow
#define MAX_ALLOC_SIZE ((std::size_t(-1)) - MALLOC_PAGE_SIZE)
#endif

namespace model {

static void thread_yield();
void _acquire_segments_lock_write();
void _release_segments_lock_write();

// Data types

//! A memory heap, per thread
struct Heap;
//! Span of memory pages
struct Span;
//! Size class definition
struct SizeClass;
//! Span block bookkeeping
struct SpanBlock;
//! Span data union, usage depending on span state
union  SpanData;
//! Cache data
struct SpanCounter;
//! Segment - holding spans
struct Segment;

struct SpanBlock
{
    //! Free list
    uint16_t free_list;
    //! First autolinked block
    uint16_t first_autolink;
    //! Free count
    uint16_t free_count;
    //! Padding
    uint16_t padding;
};

union SpanData
{
    //! Span data
    SpanBlock block;
    //! List size (used when span is part of a list)
    u32 list_size;
};

struct Span
{
    //!	Heap ID
    std::atomic<i32> heap_id;
    //! Size class
    count_t  size_class;
    //! Span data
    SpanData data;
    //! Next span
    Span*    next_span;
    //! Previous span
    Span*    prev_span;
    //! The segment that owns this span
    //! or SINGLE_SEGMENT_MARKER if the span is not part of a segment
    Segment* owner_segment;
};

static_assert(sizeof(Span) <= SPAN_HEADER_SIZE, "Span size mismatch");

struct SpanCounter
{
    //! Allocation high water mark
    u32 max_allocations;
    //! Current number of allocations
    u32 current_allocations;
    //! Cache limit
    u32 cache_limit;
};

struct Heap
{
    //! Deferred deallocation
    std::atomic<void*> defer_deallocate;
    //! Free count for each size class active span
    SpanBlock active_block[SIZE_CLASS_COUNT];
    //! Active span for each size class
    Span*      active_span[SIZE_CLASS_COUNT];
    //! List of demi-used spans with free blocks for each size class (double linked list)
    Span*      size_cache[SIZE_CLASS_COUNT];
    //! List of free spans for each page count (single linked list)
    Span*      span_cache;
    //! Allocation counters
    SpanCounter  span_counter;
    //! Heap ID
    i32 id;
    //! List of free spans for each large class count (single linked list)
    Span*        large_cache[LARGE_CLASS_COUNT];
    //! Allocation counters for large blocks
    SpanCounter  large_counter[LARGE_CLASS_COUNT];
#if ENABLE_STATISTICS
    //! Number of bytes currently reqeusted in allocations
    std::size_t  requested;
    //! Number of bytes current allocated
    std::size_t  allocated;
    //! Number of bytes transitioned thread -> global
    std::size_t  thread_to_global;
    //! Number of bytes transitioned global -> thread
    std::size_t  global_to_thread;
#endif
};

struct SizeClass
{
    //! Size of blocks in this class
    uint16_t size;
    //! Number of pages to allocate for a chunk
    uint16_t page_count;
    //! Number of blocks in each chunk
    uint16_t block_count;
    //! Class index this class is merged with
    uint16_t class_idx;
};

static_assert(sizeof(SizeClass) == 8, "SizeClass size mismatch");

struct Segment
{
    std::atomic<void*> next_segment;
    Span* first_span;
    std::atomic<u32> free_markers;
};

//! Global size classes
static SizeClass _memory_size_class[SIZE_CLASS_COUNT];

//! Heap ID counter
static std::atomic<i32> _memory_heap_id;

//! Heaps array lock
static std::atomic_flag _heaps_lock = ATOMIC_FLAG_INIT;

//! Free segments
static std::atomic<void*> _segments_head;

//! Segment RW lock
static std::atomic<i32> _segments_rw_lock;

//! Global span cache
static std::atomic<void*> _memory_span_cache;

//! Global large cache
static std::atomic<void*> _memory_large_cache[LARGE_CLASS_COUNT];

//! Current thread heap
static DEFINE_THREAD_LOCAL(Heap*, _memory_thread_heap);

//! Preferred heap of this thread
static DEFINE_THREAD_LOCAL(u32, _memory_preferred_heap);

//! All heaps
static std::atomic<void*> _memory_heaps[HEAP_ARRAY_SIZE];

//! Adaptive cache max allocation count
static u32 _memory_max_allocation;

//! Adaptive cache max allocation count
static u32 _memory_max_allocation_large[LARGE_CLASS_COUNT];

#if ENABLE_STATISTICS
//! Total number of mapped memory pages
static std::atomic<i32> _mapped_pages;
//! Running counter of total number of mapped memory pages since start
static std::atomic<i32> _mapped_total;
//! Running counter of total number of unmapped memory pages since start
static std::atomic<i32> _unmapped_total;
#endif

static Span*
_memory_map(size_t page_count);

static void
_memory_unmap(Span* ptr);

static void*
_memory_allocate_external(size_t bytes);

static void
_memory_deallocate_external(void* ptr);

static int
_memory_deallocate_deferred(Heap* heap, size_t size_class);

Heap* _get_heap_ptr(void* heap) {
    return reinterpret_cast<Heap*>(reinterpret_cast<uptr>(heap) & ~1UL);
}

//! Lookup a memory heap from heap ID
static Heap*
_memory_heap_lookup(i32 id)
{
    return _get_heap_ptr(_memory_heaps[id].load());
}

//! Increase an allocation counter
static void
_memory_counter_increase(SpanCounter* counter, u32* global_counter)
{
    if (++counter->current_allocations > counter->max_allocations)
    {
        counter->max_allocations = counter->current_allocations;
#if MAX_SPAN_CACHE_DIVISOR > 0
        counter->cache_limit = counter->max_allocations / MAX_SPAN_CACHE_DIVISOR;
#endif
        if (counter->max_allocations > *global_counter)
            *global_counter = counter->max_allocations;
    }
}

//! Insert the given list of memory page spans in the global cache for small/medium blocks
static void
_memory_global_cache_insert(Span* first_span, size_t list_size)
{
    assert((list_size == 1) || (first_span->next_span != 0));
#if MAX_SPAN_CACHE_DIVISOR > 0
    while (1)
    {
        void* global_span_ptr = _memory_span_cache.load();
        if (global_span_ptr != SPAN_LIST_LOCK_TOKEN)
        {
            uptr global_list_size = reinterpret_cast<uptr>(global_span_ptr) & ~SPAN_MASK;
            Span* global_span =
                    reinterpret_cast<Span*>(
                        reinterpret_cast<void*>(reinterpret_cast<uptr>(global_span_ptr) & SPAN_MASK));

#ifdef GLOBAL_SPAN_CACHE_MULTIPLIER
            size_t cache_limit = GLOBAL_SPAN_CACHE_MULTIPLIER * (_memory_max_allocation / MAX_SPAN_CACHE_DIVISOR);
            if ((global_list_size >= cache_limit) && (global_list_size > MIN_SPAN_CACHE_SIZE))
                break;
#endif
            //We only have 16 bits for size of list, avoid overflow
            if ((global_list_size + list_size) > 0xFFFF)
                break;

            //Use prev_span as skip pointer over this sublist range of spans
            first_span->data.list_size = static_cast<u32>(list_size);
            first_span->prev_span = global_span;

            //Insert sublist into global cache
            global_list_size += list_size;
            void* first_span_ptr = reinterpret_cast<void*>(
                        reinterpret_cast<uptr>(first_span) | global_list_size);
            if (std::atomic_compare_exchange_strong(&_memory_span_cache, &global_span_ptr, first_span_ptr))
                return;
        }
        else
        {
            //Atomic operation failed, yield timeslice and retry
            thread_yield();
            std::atomic_thread_fence(std::memory_order_acquire);
        }
    }
#endif
    //Global cache full, release pages
    for (size_t ispan = 0; ispan < list_size; ++ispan) {
        assert(first_span);
        Span* next_span = first_span->next_span;
        _memory_unmap(first_span);
        first_span = next_span;
    }
}

//! Extract a number of memory page spans from the global cache for small/medium blocks
static Span*
_memory_global_cache_extract() {
    Span* span = nullptr;
    std::atomic<void*>* cache = &_memory_span_cache;
    std::atomic_thread_fence(std::memory_order_acquire);
    void* global_span_ptr = cache->load();
    while (global_span_ptr) {
        if ((global_span_ptr != SPAN_LIST_LOCK_TOKEN) &&
            std::atomic_compare_exchange_strong(cache, &global_span_ptr, SPAN_LIST_LOCK_TOKEN)) {
            //Grab a number of thread cache spans, using the skip span pointer
            //stored in prev_span to quickly skip ahead in the list to get the new head
            uptr global_span_count = reinterpret_cast<uptr>(global_span_ptr) & ~SPAN_MASK;
            span = reinterpret_cast<Span*>(
                        reinterpret_cast<void*>(reinterpret_cast<uptr>(global_span_ptr) & SPAN_MASK));
            assert((span->data.list_size == 1) || (span->next_span != 0));

            Span* new_global_span = span->prev_span;
            global_span_count -= span->data.list_size;

            //Set new head of global cache list
            void* new_cache_head = global_span_count ?
                ((void*)((uptr)new_global_span | global_span_count)) :
                nullptr;
            *cache = new_cache_head;
            std::atomic_thread_fence(std::memory_order_release);
            break;
        }

        //List busy, yield timeslice and retry
        thread_yield();
        std::atomic_thread_fence(std::memory_order_acquire);
        global_span_ptr = cache->load();
    }

    return span;
}

/*! Insert the given list of memory page spans in the global cache for large blocks,
similar to _memory_global_cache_insert */
static void
_memory_global_cache_large_insert(Span* span_list, size_t list_size, size_t span_count) {
    assert((list_size == 1) || (span_list->next_span != 0));
    assert(span_list->size_class == (SIZE_CLASS_COUNT + (span_count - 1)));
#if MAX_SPAN_CACHE_DIVISOR > 0
    std::atomic<void*>* cache = &_memory_large_cache[span_count - 1];
    while (1) {
        void* global_span_ptr = cache->load();
        if (global_span_ptr != SPAN_LIST_LOCK_TOKEN) {
            uptr global_list_size = (uptr)global_span_ptr & ~SPAN_MASK;
            Span* global_span = (Span*)((void*)((uptr)global_span_ptr & SPAN_MASK));

#ifdef GLOBAL_SPAN_CACHE_MULTIPLIER
            size_t cache_limit = GLOBAL_SPAN_CACHE_MULTIPLIER * (_memory_max_allocation_large[span_count - 1] / MAX_SPAN_CACHE_DIVISOR);
            if ((global_list_size >= cache_limit) && (global_list_size > MIN_SPAN_CACHE_SIZE))
                break;
#endif
            if ((global_list_size + list_size) > 0xFFFF)
                break;

            span_list->data.list_size = (u32)list_size;
            span_list->prev_span = global_span;

            global_list_size += list_size;
            void* new_global_span_ptr = (void*)((uptr)span_list | global_list_size);
            if (std::atomic_compare_exchange_strong(cache, &global_span_ptr, new_global_span_ptr))
                return;
        }
        else {
            thread_yield();
            std::atomic_thread_fence(std::memory_order_acquire);
        }
    }
#endif
    //Global cache full, release spans
    for (size_t ispan = 0; ispan < list_size; ++ispan) {
        assert(span_list);
        Span* next_span = span_list->next_span;
        _memory_unmap(span_list);
        span_list = next_span;
    }
}

/*! Extract a number of memory page spans from the global cache for large blocks,
similar to _memory_global_cache_extract */
static Span*
_memory_global_cache_large_extract(size_t span_count) {
    Span* span = nullptr;
    std::atomic<void*>* cache = &_memory_large_cache[span_count - 1];
    std::atomic_thread_fence(std::memory_order_acquire);
    void* global_span_ptr = cache->load();
    while (global_span_ptr) {
        if ((global_span_ptr != SPAN_LIST_LOCK_TOKEN) &&
            std::atomic_compare_exchange_strong(cache, &global_span_ptr, SPAN_LIST_LOCK_TOKEN)) {
            uptr global_list_size = (uptr)global_span_ptr & ~SPAN_MASK;
            span = (Span*)((void*)((uptr)global_span_ptr & SPAN_MASK));
            assert((span->data.list_size == 1) || (span->next_span != 0));
            assert(span->size_class == (SIZE_CLASS_COUNT + (span_count - 1)));

            Span* new_global_span = span->prev_span;
            global_list_size -= span->data.list_size;

            void* new_global_span_ptr = global_list_size ?
                ((void*)((uptr)new_global_span | global_list_size)) :
                nullptr;
            *cache = new_global_span_ptr;
            std::atomic_thread_fence(std::memory_order_release);
            break;
        }

        thread_yield();
        std::atomic_thread_fence(std::memory_order_acquire);
        global_span_ptr = cache->load();
    }
    return span;
}

//! Allocate a small/medium sized memory block from the given heap
static void*
_memory_allocate_from_heap(Heap* heap, size_t size) {
#if ENABLE_STATISTICS
    //For statistics we need to store the requested size in the memory block
    size += sizeof(size_t);
#endif

    //Calculate the size class index and do a dependent lookup of the final class index (in case of merged classes)
    const size_t class_idx = _memory_size_class[(size <= SMALL_SIZE_LIMIT) ?
        ((size + (SMALL_GRANULARITY - 1)) >> SMALL_GRANULARITY_SHIFT) - 1 :
        SMALL_CLASS_COUNT + ((size - SMALL_SIZE_LIMIT + (MEDIUM_GRANULARITY - 1)) >> MEDIUM_GRANULARITY_SHIFT) - 1].class_idx;

    SpanBlock* active_block = heap->active_block + class_idx;
    SizeClass* size_class = _memory_size_class + class_idx;
    const count_t class_size = size_class->size;

#if ENABLE_STATISTICS
    heap->allocated += class_size;
    heap->requested += size;
#endif

    //Step 1: Try to get a block from the currently active span. The span block bookkeeping
    //        data for the active span is stored in the heap for faster access
use_active:
    if (active_block->free_count) {
        //Happy path, we have a span with at least one free block
        Span* span = heap->active_span[class_idx];
        count_t offset = class_size * active_block->free_list;
        u32* block = static_cast<u32*>(pointer_offset(span, SPAN_HEADER_SIZE + offset));
        assert(span);

        --active_block->free_count;
        if (!active_block->free_count) {
            //Span is now completely allocated, set the bookkeeping data in the
            //span itself and reset the active span pointer in the heap
            span->data.block.free_count = 0;
            span->data.block.first_autolink = (uint16_t)size_class->block_count;
            heap->active_span[class_idx] = nullptr;
        }
        else {
            //Get the next free block, either from linked list or from auto link
            if (active_block->free_list < active_block->first_autolink) {
                active_block->free_list = (uint16_t)(*block);
            }
            else {
                ++active_block->free_list;
                ++active_block->first_autolink;
            }
            assert(active_block->free_list < size_class->block_count);
        }

#if ENABLE_STATISTICS
        //Store the requested size for statistics
        *(size_t*)pointer_offset(block, class_size - sizeof(size_t)) = size;
#endif

        return block;
    }

    //Step 2: No active span, try executing deferred deallocations and try again if there
    //        was at least one of the reqeusted size class
    if (_memory_deallocate_deferred(heap, class_idx)) {
        if (active_block->free_count)
            goto use_active;
    }

    //Step 3: Check if there is a semi-used span of the requested size class available
    if (heap->size_cache[class_idx]) {
        //Promote a pending semi-used span to be active, storing bookkeeping data in
        //the heap structure for faster access
        Span* span = heap->size_cache[class_idx];
        *active_block = span->data.block;
        assert(active_block->free_count > 0);
        Span* next_span = span->next_span;
        heap->size_cache[class_idx] = next_span;
        heap->active_span[class_idx] = span;
        goto use_active;
    }

    //Step 4: No semi-used span available, try grab a span from the thread cache
    Span* span = heap->span_cache;
    if (!span) {
        //Step 5: No span available in the thread cache, try grab a list of spans from the global cache
        span = _memory_global_cache_extract();
#if ENABLE_STATISTICS
        if (span)
            heap->global_to_thread += (size_t)span->data.list_size * size_class->page_count * MALLOC_PAGE_SIZE;
#endif
    }
    if (span) {
        if (span->data.list_size > 1) {
            //We got a list of spans, we will use first as active and store remainder in thread cache
            Span* next_span = span->next_span;
            assert(next_span);
            next_span->data.list_size = span->data.list_size - 1;
            heap->span_cache = next_span;
        }
        else {
            heap->span_cache = nullptr;
        }
    }
    else {
        //Step 6: All caches empty, map in new memory pages
        span = _memory_map(size_class->page_count);
    }

    //Mark span as owned by this heap and set base data
    span->heap_id = heap->id;
    std::atomic_thread_fence(std::memory_order_release);

    span->size_class = (count_t)class_idx;

    //If we only have one block we will grab it, otherwise
    //set span as new span to use for next allocation
    if (size_class->block_count > 1) {
        //Reset block order to sequential auto linked order
        active_block->free_count = (uint16_t)(size_class->block_count - 1);
        active_block->free_list = 1;
        active_block->first_autolink = 1;
        heap->active_span[class_idx] = span;
    }
    else {
        span->data.block.free_count = 0;
        span->data.block.first_autolink = (uint16_t)size_class->block_count;
    }

    //Track counters
    _memory_counter_increase(&heap->span_counter, &_memory_max_allocation);

#if ENABLE_STATISTICS
    //Store the requested size for statistics
    *(size_t*)pointer_offset(span, SPAN_HEADER_SIZE + class_size - sizeof(size_t)) = size;
#endif

    //Return first block if memory page span
    return pointer_offset(span, SPAN_HEADER_SIZE);
}

//! Allocate a large sized memory block from the given heap
static void*
_memory_allocate_large_from_heap(Heap* heap, size_t size) {
    //Calculate number of needed max sized spans (including header)
    size += SPAN_HEADER_SIZE;
    size_t num_spans = size / SPAN_MAX_SIZE;
    if (size % SPAN_MAX_SIZE)
        ++num_spans;
    size_t idx = num_spans - 1;

    if (!idx) {
        Span* span = heap->span_cache;
        if (!span) {
            _memory_deallocate_deferred(heap, 0);
            span = heap->span_cache;
        }
        if (!span) {
            //Step 5: No span available in the thread cache, try grab a list of spans from the global cache
            span = _memory_global_cache_extract();
#if ENABLE_STATISTICS
            if (span)
                heap->global_to_thread += (size_t)span->data.list_size * SPAN_MAX_PAGE_COUNT * MALLOC_PAGE_SIZE;
#endif
        }
        if (span) {
            if (span->data.list_size > 1) {
                //We got a list of spans, we will use first as active and store remainder in thread cache
                Span* next_span = span->next_span;
                assert(next_span);
                next_span->data.list_size = span->data.list_size - 1;
                heap->span_cache = next_span;
            }
            else {
                heap->span_cache = nullptr;
            }
        }
        else {
            //Step 6: All caches empty, map in new memory pages
            span = _memory_map(SPAN_MAX_PAGE_COUNT);
        }

        //Mark span as owned by this heap and set base data
        span->heap_id = heap->id;
        std::atomic_thread_fence(std::memory_order_release);

        span->size_class = SIZE_CLASS_COUNT;

        //Track counters
        _memory_counter_increase(&heap->span_counter, &_memory_max_allocation);

        return pointer_offset(span, SPAN_HEADER_SIZE);
    }

use_cache:
    //Step 1: Check if cache for this large size class (or the following, unless first class) has a span
    while (!heap->large_cache[idx] && (idx < (LARGE_CLASS_COUNT - 1)) && (idx < (num_spans + 1)))
        ++idx;
    Span* span = heap->large_cache[idx];
    if (span) {
        //Happy path, use from cache
        if (span->data.list_size > 1) {
            Span* new_head = span->next_span;
            assert(new_head);
            new_head->data.list_size = span->data.list_size - 1;
            heap->large_cache[idx] = new_head;
        }
        else {
            heap->large_cache[idx] = nullptr;
        }

        span->size_class = SIZE_CLASS_COUNT + (count_t)idx;

        // Mark span as owned by this heap
        span->heap_id = heap->id;
        std::atomic_thread_fence(std::memory_order_release);

        //Increase counter
        _memory_counter_increase(&heap->large_counter[idx], &_memory_max_allocation_large[idx]);

        return pointer_offset(span, SPAN_HEADER_SIZE);
    }

    //Restore index, we're back to smallest fitting span count
    idx = num_spans - 1;

    //Step 2: Process deferred deallocation
    if (_memory_deallocate_deferred(heap, SIZE_CLASS_COUNT + idx))
        goto use_cache;
    assert(!heap->large_cache[idx]);

    //Step 3: Extract a list of spans from global cache
    span = _memory_global_cache_large_extract(num_spans);
    if (span) {
#if ENABLE_STATISTICS
        heap->global_to_thread += (size_t)span->data.list_size * num_spans * SPAN_MAX_SIZE;
#endif
        //We got a list from global cache, store remainder in thread cache
        if (span->data.list_size > 1) {
            Span* new_head = span->next_span;
            assert(new_head);
            new_head->prev_span = nullptr;
            new_head->data.list_size = span->data.list_size - 1;
            heap->large_cache[idx] = new_head;
        }
    }
    else {
        //Step 4: Map in more memory pages
        span = _memory_map(num_spans * SPAN_MAX_PAGE_COUNT);
    }
    //Mark span as owned by this heap
    span->heap_id = heap->id;
    std::atomic_thread_fence(std::memory_order_release);

    span->size_class = SIZE_CLASS_COUNT + (count_t)idx;

    //Increase counter
    _memory_counter_increase(&heap->large_counter[idx], &_memory_max_allocation_large[idx]);

    return pointer_offset(span, SPAN_HEADER_SIZE);
}

//! Allocate a new heap
static Heap*
_memory_allocate_heap(void) {
    Heap* heap;
    //Try getting an orphaned heap
    std::atomic_thread_fence(std::memory_order_acquire);

    //Map in pages for a new heap
    heap = static_cast<Heap*>(_memory_allocate_external(sizeof(Heap)));
    new (heap) Heap ();

    //Get a new heap ID
    do {
        heap->id = (++_memory_heap_id);
        if (_memory_heap_lookup(heap->id))
            heap->id = 0;
    } while (!heap->id);
    return heap;
}

//! Add a span to a double linked list
static void
_memory_list_add(Span** head, Span* span) {
    if (*head) {
        (*head)->prev_span = span;
        span->next_span = *head;
    }
    else {
        span->next_span = nullptr;
    }
    *head = span;
}

//! Remove a span from a double linked list
static void
_memory_list_remove(Span** head, Span* span) {
    if (*head == span) {
        *head = span->next_span;
    }
    else {
        if (span->next_span)
            span->next_span->prev_span = span->prev_span;
        span->prev_span->next_span = span->next_span;
    }
}

//! Insert span into thread cache, releasing to global cache if overflow
static void
_memory_heap_cache_insert(Heap* heap, Span* span) {
#if MAX_SPAN_CACHE_DIVISOR == 0
    (void)sizeof(heap);
    _memory_global_cache_insert(span, 1);
#else
    Span** cache = &heap->span_cache;
    span->next_span = *cache;
    if (*cache)
        span->data.list_size = (*cache)->data.list_size + 1;
    else
        span->data.list_size = 1;
    *cache = span;
#if MAX_SPAN_CACHE_DIVISOR > 1
    //Check if cache exceeds limit
    if ((span->data.list_size >= (MIN_SPAN_CACHE_RELEASE + MIN_SPAN_CACHE_SIZE)) &&
        (span->data.list_size > heap->span_counter.cache_limit)) {
        //Release to global cache
        count_t list_size = 1;
        Span* next = span->next_span;
        Span* last = span;
        while (list_size < MIN_SPAN_CACHE_RELEASE) {
            last = next;
            next = next->next_span;
            ++list_size;
        }
        next->data.list_size = span->data.list_size - list_size;
        last->next_span = nullptr; //Terminate list
        *cache = next;
        _memory_global_cache_insert(span, list_size);
#if ENABLE_STATISTICS
        heap->thread_to_global += list_size * QUICK_ALLOCATION_PAGES_COUNT * MALLOC_PAGE_SIZE;
#endif
    }
#endif
#endif
}

//! Deallocate the given small/medium memory block from the given heap
static void
_memory_deallocate_to_heap(Heap* heap, Span* span, void* p) {
    //Check if span is the currently active span in order to operate
    //on the correct bookkeeping data
    const count_t class_idx = span->size_class;
    SizeClass* size_class = _memory_size_class + class_idx;
    int is_active = (heap->active_span[class_idx] == span);
    SpanBlock* block_data = is_active ?
        heap->active_block + class_idx :
        &span->data.block;

#if ENABLE_STATISTICS
    heap->allocated -= size_class->size;
    heap->requested -= *(size_t*)pointer_offset(p, size_class->size - sizeof(size_t));
#endif

    //Check if the span will become completely free
    if (block_data->free_count == ((count_t)size_class->block_count - 1)) {
        //Track counters
        assert(heap->span_counter.current_allocations > 0);
        --heap->span_counter.current_allocations;

        //If it was active, reset counter. Otherwise, if not active, remove from
        //partial free list if we had a previous free block (guard for classes with only 1 block)
        if (is_active)
            block_data->free_count = 0;
        else if (block_data->free_count > 0)
            _memory_list_remove(&heap->size_cache[class_idx], span);

        //Add to span cache
        _memory_heap_cache_insert(heap, span);
        return;
    }

    //Check if first free block for this span (previously fully allocated)
    if (block_data->free_count == 0) {
        //add to free list and disable autolink
        _memory_list_add(&heap->size_cache[class_idx], span);
        block_data->first_autolink = (uint16_t)size_class->block_count;
    }
    ++block_data->free_count;
    //Span is not yet completely free, so add block to the linked list of free blocks
    u32* block = static_cast<u32*>(p);
    *block = block_data->free_list;
    count_t block_offset = (count_t)pointer_diff(block, span) - SPAN_HEADER_SIZE;
    count_t block_idx = block_offset / (count_t)size_class->size;
    block_data->free_list = (uint16_t)block_idx;
}

//! Deallocate the given large memory block from the given heap
static void
_memory_deallocate_large_to_heap(Heap* heap, Span* span) {
    //Check if aliased with 64KiB small/medium spans
    if (span->size_class == SIZE_CLASS_COUNT) {
        //Track counters
        --heap->span_counter.current_allocations;
        //Add to span cache
        _memory_heap_cache_insert(heap, span);
        return;
    }

    //Decrease counter
    size_t idx = span->size_class - SIZE_CLASS_COUNT;
    SpanCounter* counter = heap->large_counter + idx;
    assert(counter->current_allocations > 0);
    --counter->current_allocations;

#if MAX_SPAN_CACHE_DIVISOR == 0
    _memory_global_cache_large_insert(span, 1, idx + 1);
#else
    //Insert into cache list
    Span** cache = heap->large_cache + idx;
    span->next_span = *cache;
    if (*cache)
        span->data.list_size = (*cache)->data.list_size + 1;
    else
        span->data.list_size = 1;
    *cache = span;
#if MAX_SPAN_CACHE_DIVISOR > 1
    //Check if cache exceeds limit
    if ((span->data.list_size >= (MIN_SPAN_CACHE_RELEASE + MIN_SPAN_CACHE_SIZE)) &&
        (span->data.list_size > counter->cache_limit)) {
        //Release to global cache
        count_t list_size = 1;
        Span* next = span->next_span;
        Span* last = span;
        while (list_size < MIN_SPAN_CACHE_RELEASE) {
            last = next;
            next = next->next_span;
            ++list_size;
        }
        assert(next->next_span);
        next->data.list_size = span->data.list_size - list_size;
        last->next_span = nullptr; //Terminate list
        *cache = next;
        _memory_global_cache_large_insert(span, list_size, idx + 1);
#if ENABLE_STATISTICS
        heap->thread_to_global += list_size * (idx + 1) * SPAN_MAX_SIZE;
#endif
    }
#endif
#endif
}

//! Process pending deferred cross-thread deallocations
static int
_memory_deallocate_deferred(Heap* heap, size_t size_class) {
    //Grab the current list of deferred deallocations
    std::atomic_thread_fence(std::memory_order_acquire);
    void* p = heap->defer_deallocate.load();
    if (!p)
        return 0;
    if (!std::atomic_compare_exchange_strong(&heap->defer_deallocate, &p, reinterpret_cast<void*>(0)))
        return 0;
    //Keep track if we deallocate in the given size class
    int got_class = 0;
    do {
        void* next = *(void**)p;
        //Get span and check which type of block
        Span* span = static_cast<Span*>((void*)((uptr)p & SPAN_MASK));
        if (span->size_class < SIZE_CLASS_COUNT) {
            //Small/medium block
            got_class |= (span->size_class == size_class);
            _memory_deallocate_to_heap(heap, span, p);
        }
        else {
            //Large block
            got_class |= ((span->size_class >= size_class) && (span->size_class <= (size_class + 2)));
            _memory_deallocate_large_to_heap(heap, span);
        }
        //Loop until all pending operations in list are processed
        p = next;
    } while (p);
    return got_class;
}

//! Defer deallocation of the given block to the given heap
static void
_memory_deallocate_defer(i32 heap_id, void* p) {
    //Get the heap and link in pointer in list of deferred opeations
    Heap* heap = _memory_heap_lookup(heap_id);
    void* last_ptr = heap->defer_deallocate.load();
    do {
        *(void**)p = last_ptr; //Safe to use block, it's being deallocated
    } while (!std::atomic_compare_exchange_weak(&heap->defer_deallocate, &last_ptr, p));
}

//! Allocate a block of the given size
static void*
_memory_allocate(size_t size) {
    if (size <= MEDIUM_SIZE_LIMIT)
        return _memory_allocate_from_heap(GET_THREAD_LOCAL(Heap*, _memory_thread_heap), size);
    else if (size <= LARGE_SIZE_LIMIT)
        return _memory_allocate_large_from_heap(GET_THREAD_LOCAL(Heap*, _memory_thread_heap), size);

    //Oversized, allocate pages directly
    size += SPAN_HEADER_SIZE;
    size_t num_pages = size / MALLOC_PAGE_SIZE;
    if (size % MALLOC_PAGE_SIZE)
        ++num_pages;
    Span* span = _memory_map(num_pages);
    span->heap_id = 0;
    //Store page count in next_span
    span->next_span = (Span*)((uptr)num_pages);

    return pointer_offset(span, SPAN_HEADER_SIZE);
}

//! Deallocate the given block
static void
_memory_deallocate(void* p) {
    if (!p)
        return;

    //Grab the span (always at start of span, using 64KiB alignment)
    Span* span = static_cast<Span*>((void*)((uptr)p & SPAN_MASK));
    i32 heap_id = span->heap_id.load();
    Heap* heap = GET_THREAD_LOCAL(Heap*, _memory_thread_heap);
    //Check if block belongs to this heap or if deallocation should be deferred
    if (heap_id == heap->id) {
        if (span->size_class < SIZE_CLASS_COUNT)
            _memory_deallocate_to_heap(heap, span, p);
        else
            _memory_deallocate_large_to_heap(heap, span);
    }
    else if (heap_id > 0) {
        _memory_deallocate_defer(heap_id, p);
    }
    else {
        _memory_unmap(span);
    }
}

//! Reallocate the given block to the given size
static void*
_memory_reallocate(void* p, size_t size, size_t oldsize, unsigned int flags) {
    if (p) {
        //Grab the span (always at start of span, using 64KiB alignment)
        Span* span = static_cast<Span*>((void*)((uptr)p & SPAN_MASK));
        i32 heap_id = span->heap_id.load();
        if (heap_id) {
            if (span->size_class < SIZE_CLASS_COUNT) {
                //Small/medium sized block
                SizeClass* size_class = _memory_size_class + span->size_class;
                if ((size_t)size_class->size >= size)
                    return p; //Still fits in block, never mind trying to save memory
                if (!oldsize)
                    oldsize = size_class->size;
            }
            else {
                //Large block
                size_t total_size = size + SPAN_HEADER_SIZE;
                size_t num_spans = total_size / SPAN_MAX_SIZE;
                if (total_size % SPAN_MAX_SIZE)
                    ++num_spans;
                size_t current_spans = (span->size_class - SIZE_CLASS_COUNT) + 1;
                if ((current_spans >= num_spans) && (num_spans >= (current_spans / 2)))
                    return p; //Still fits and less than half of memory would be freed
                if (!oldsize)
                    oldsize = (current_spans * (size_t)SPAN_MAX_SIZE) - SPAN_HEADER_SIZE;
            }
        }
        else {
            //Oversized block
            size_t total_size = size + SPAN_HEADER_SIZE;
            size_t num_pages = total_size / MALLOC_PAGE_SIZE;
            if (total_size % MALLOC_PAGE_SIZE)
                ++num_pages;
            //Page count is stored in next_span
            size_t current_pages = (size_t)span->next_span;
            if ((current_pages >= num_pages) && (num_pages >= (current_pages / 2)))
                return p; //Still fits and less than half of memory would be freed
            if (!oldsize)
                oldsize = (current_pages * (size_t)MALLOC_PAGE_SIZE) - SPAN_HEADER_SIZE;
        }
    }

    //Size is greater than block size, need to allocate a new block and deallocate the old
    //Avoid hysteresis by overallocating if increase is small (below 37%)
    size_t lower_bound = oldsize + (oldsize >> 2) + (oldsize >> 3);
    void* block = _memory_allocate(size > lower_bound ? size : lower_bound);
    if (p) {
        if (!(flags & NO_PRESERVE))
            std::memcpy(block, p, oldsize < size ? oldsize : size);
        _memory_deallocate(p);
    }

    return block;
}

#ifdef CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

//! Get the usable size of the given block
static size_t
_memory_usable_size(void* p) {
    //Grab the span (always at start of span, using 64KiB alignment)
    Span* span = static_cast<Span*>((void*)((uptr)p & SPAN_MASK));
    i32 heap_id = span->heap_id.load();
    if (heap_id) {
        if (span->size_class < SIZE_CLASS_COUNT) {
            //Small/medium block
            SizeClass* size_class = _memory_size_class + span->size_class;
            return size_class->size;
        }

        //Large block
        size_t current_spans = (span->size_class - SIZE_CLASS_COUNT) + 1;
        return (current_spans * (size_t)SPAN_MAX_SIZE) - SPAN_HEADER_SIZE;
    }

    //Oversized block, page count is stored in next_span
    size_t current_pages = (size_t)span->next_span;
    return (current_pages * (size_t)MALLOC_PAGE_SIZE) - SPAN_HEADER_SIZE;
}

#endif // CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

//! Adjust and optimize the size class properties for the given class
static void
_memory_adjust_size_class(size_t iclass) {
    //Calculate how many pages are needed for 255 blocks
    size_t block_size = _memory_size_class[iclass].size;

    //TODO: STNK Hardcode always 16 pages
    size_t page_count = QUICK_ALLOCATION_PAGES_COUNT;
    size_t block_count = ((page_count * MALLOC_PAGE_SIZE) - SPAN_HEADER_SIZE) / block_size;
    //Store the final configuration
    _memory_size_class[iclass].page_count = (uint16_t)page_count;
    _memory_size_class[iclass].block_count = (uint16_t)block_count;
    _memory_size_class[iclass].class_idx = (uint16_t)iclass;

    //Check if previous size classes can be merged
    size_t prevclass = iclass;
    while (prevclass > 0) {
        --prevclass;
        //A class can be merged if number of pages and number of blocks are equal
        if ((_memory_size_class[prevclass].page_count == _memory_size_class[iclass].page_count) &&
            (_memory_size_class[prevclass].block_count == _memory_size_class[iclass].block_count)) {
            memcpy(_memory_size_class + prevclass, _memory_size_class + iclass, sizeof(_memory_size_class[iclass]));
        }
        else {
            break;
        }
    }
}

//! Initialize the allocator and setup global data
int
initialize(void) {
    INITIALIZE_THREAD_LOCAL(_memory_thread_heap);
    INITIALIZE_THREAD_LOCAL(_memory_preferred_heap);
    _memory_heap_id = 0;

    //Setup all small and medium size classes
    size_t iclass;
    for (iclass = 0; iclass < SMALL_CLASS_COUNT; ++iclass) {
        size_t size = (iclass + 1) * SMALL_GRANULARITY;
        _memory_size_class[iclass].size = (uint16_t)size;
        _memory_adjust_size_class(iclass);
    }
    for (iclass = 0; iclass < MEDIUM_CLASS_COUNT; ++iclass) {
        size_t size = SMALL_SIZE_LIMIT + ((iclass + 1) * MEDIUM_GRANULARITY);
        if (size > MEDIUM_SIZE_LIMIT)
            size = MEDIUM_SIZE_LIMIT;
        _memory_size_class[SMALL_CLASS_COUNT + iclass].size = (uint16_t)size;
        _memory_adjust_size_class(SMALL_CLASS_COUNT + iclass);
    }

    _segments_head = nullptr;
    _segments_rw_lock = 0;

    //Initialize this thread
    thread_initialize();
    return 0;
}

//! Finalize the allocator
void
finalize(void) {
    std::atomic_thread_fence(std::memory_order_acquire);

    //Free all thread caches
    for (size_t list_idx = 0; list_idx < HEAP_ARRAY_SIZE; ++list_idx) {
        Heap* heap = _get_heap_ptr(_memory_heaps[list_idx].load());
        if (heap) {
            _memory_deallocate_deferred(heap, 0);

            {
                //const size_t page_count = QUICK_ALLOCATION_PAGES_COUNT;
                Span* span = heap->span_cache;
                unsigned int span_count = span ? span->data.list_size : 0;
                for (unsigned int ispan = 0; ispan < span_count; ++ispan) {
                    Span* next_span = span->next_span;
                    _memory_unmap(span);
                    span = next_span;
                }
            }

            //Free large spans
            for (size_t iclass = 0; iclass < LARGE_CLASS_COUNT; ++iclass) {
                //const size_t span_count = iclass + 1;
                Span* span = heap->large_cache[iclass];
                while (span) {
                    Span* next_span = span->next_span;
                    _memory_unmap(span);
                    span = next_span;
                }
            }
            _memory_deallocate_external(heap);
        }

        _memory_heaps[list_idx] = nullptr;
    }

    {
        //Free global caches
        void* span_ptr = _memory_span_cache.load();
        size_t cache_count = (uptr)span_ptr & ~SPAN_MASK;
        Span* span = (Span*)((void*)((uptr)span_ptr & SPAN_MASK));
        while (cache_count) {
            Span* skip_span = span->prev_span;
            unsigned int span_count = span->data.list_size;
            for (unsigned int ispan = 0; ispan < span_count; ++ispan) {
                Span* next_span = span->next_span;
                _memory_unmap(span);
                span = next_span;
            }
            span = skip_span;
            cache_count -= span_count;
        }
        _memory_span_cache = nullptr;
    }

    for (size_t iclass = 0; iclass < LARGE_CLASS_COUNT; ++iclass) {
        void* span_ptr = _memory_large_cache[iclass].load();
        size_t cache_count = (uptr)span_ptr & ~SPAN_MASK;
        Span* span = (Span*)((void*)((uptr)span_ptr & SPAN_MASK));
        while (cache_count) {
            Span* skip_span = span->prev_span;
            unsigned int span_count = span->data.list_size;
            for (unsigned int ispan = 0; ispan < span_count; ++ispan) {
                Span* next_span = span->next_span;
                _memory_unmap(span);
                span = next_span;
            }
            span = skip_span;
            cache_count -= span_count;
        }
        _memory_large_cache[iclass] = nullptr;
    }

    // Free all segments
    _acquire_segments_lock_write();

    Segment* head = static_cast<Segment*>(_segments_head.load());
    Segment* next;
    while (head)
    {
        next = static_cast<Segment*>(head->next_segment.load());
        _memory_deallocate_external(head);
        head = next;
    }

    _release_segments_lock_write();

    FREE_THREAD_LOCAL(_memory_thread_heap);
    FREE_THREAD_LOCAL(_memory_preferred_heap);

    std::atomic_thread_fence(std::memory_order_release);
}

int _is_heap_in_use(void* heap) {
    return (reinterpret_cast<std::size_t>(heap) & 1);
}

void* _mark_heap_in_use(void* heap) {
    return reinterpret_cast<void*>(reinterpret_cast<std::size_t>(heap) | 1);
}

void* _unmark_heap_in_use(void* heap) {
    return reinterpret_cast<void*>(reinterpret_cast<std::size_t>(heap) & ~1U);
}

void _acquire_heaps_lock()
{
    while (_heaps_lock.test_and_set(std::memory_order_acquire));
}

void _release_heaps_lock()
{
    _heaps_lock.clear(std::memory_order_release);
}

//! Initialize thread, assign heap
void
thread_initialize(void) {
    if (!GET_THREAD_LOCAL(Heap*, _memory_thread_heap))
    {
        _acquire_heaps_lock();

        void* heap_ptr = _memory_heaps[GET_THREAD_LOCAL_LITERAL_VALUE(u32, _memory_preferred_heap)].load();

        for (u32 id = GET_THREAD_LOCAL_LITERAL_VALUE(u32, _memory_preferred_heap);
            id < HEAP_ARRAY_SIZE + GET_THREAD_LOCAL_LITERAL_VALUE(u32, _memory_preferred_heap);
            ++id)
        {
            heap_ptr = _memory_heaps[id % HEAP_ARRAY_SIZE].load();
            if (heap_ptr && !_is_heap_in_use(heap_ptr))
            {
                SET_THREAD_LOCAL(_memory_thread_heap, _get_heap_ptr(heap_ptr));
                _memory_heaps[id % HEAP_ARRAY_SIZE] = _mark_heap_in_use(heap_ptr);
                SET_THREAD_LOCAL(_memory_preferred_heap, (id % HEAP_ARRAY_SIZE));
                break;
            }
        }

        if (!GET_THREAD_LOCAL(Heap*, _memory_thread_heap))
        {
            // We have to allocate a new heap
            Heap* heap = _memory_allocate_heap();

            i32 id = heap->id;
            SET_THREAD_LOCAL(_memory_thread_heap, heap);
            _memory_heaps[id] = _mark_heap_in_use(heap);
            SET_THREAD_LOCAL(_memory_preferred_heap, u32(id));
        }

        _release_heaps_lock();

#if ENABLE_STATISTICS
        GET_THREAD_LOCAL(heap_t*, _memory_thread_heap)->thread_to_global = 0;
        GET_THREAD_LOCAL(heap_t*, _memory_thread_heap)->global_to_thread = 0;
#endif
    }
    assert(GET_THREAD_LOCAL(Heap*, _memory_thread_heap));
}

void
thread_reset(void) {
    if (!GET_THREAD_LOCAL(Heap*, _memory_thread_heap))
        return;

    _acquire_heaps_lock();
    void* heap_ptr = _memory_heaps[GET_THREAD_LOCAL_LITERAL_VALUE(u32, _memory_preferred_heap)].load();
    assert(_is_heap_in_use(heap_ptr));
    _memory_heaps[GET_THREAD_LOCAL_LITERAL_VALUE(u32, _memory_preferred_heap)] = _unmark_heap_in_use(heap_ptr);
    _release_heaps_lock();

    SET_THREAD_LOCAL(_memory_thread_heap, nullptr);
}

int
is_thread_initialized(void) {
    return (GET_THREAD_LOCAL(Heap*, _memory_thread_heap) != nullptr) ? 1 : 0;
}

FORCEINLINE int _get_readers_count(i32 l)
{
    return l & 0x7FFFFFFF;
}

void _acquire_segments_lock_read()
{
    i32 l = _segments_rw_lock.load();
    i32 new_l = 0;
    do {
        // Try to increase the readers count
        new_l = _get_readers_count(l) + 1;
    } while (!std::atomic_compare_exchange_weak(&_segments_rw_lock, &l, new_l));
}

void _release_segments_lock_read()
{
    i32 l = _segments_rw_lock.load();
    i32 new_l = 0;
    do {
        // Try to decrease the readers count
        new_l = _get_readers_count(l) - 1;
    } while (!std::atomic_compare_exchange_weak(&_segments_rw_lock, &l, new_l));
}

void _acquire_segments_lock_write()
{
    i32 exp = 0;
    while (!std::atomic_compare_exchange_weak(&_segments_rw_lock, &exp, 1))
    {
        exp = 0;
    }
}

void _release_segments_lock_write()
{
    i32 exp = 1;
    while (!std::atomic_compare_exchange_weak(&_segments_rw_lock, &exp, 0))
    {
        exp = 1;
    }
}

void _publish_segment_on_global_list(Segment* new_segment)
{
    _acquire_segments_lock_write();

    Segment* head = static_cast<Segment*>(_segments_head.load());
    new_segment->next_segment = head;
    _segments_head = new_segment;

    _release_segments_lock_write();
}

Span* _get_span_from_segment(Segment* new_segment) {
    int slot = SPANS_PER_SEGMENT + 1;
    while (slot == SPANS_PER_SEGMENT + 1) {
        u32 slots = new_segment->free_markers.load();
        #if SPANS_PER_SEGMENT == 32
        if (slots == 0xFFFFFFFF) {
            return nullptr; // It's full
        }
        #elif SPANS_PER_SEGMENT == 16
        if (slots == 0x0000FFFF) {
            return 0; // It's full
        }
        #else
            #pragma error Unsupported spans per-segment
        #endif
        // Find a free slot
        for (int b = 0; b < SPANS_PER_SEGMENT; ++b) {
            const u32 bit_mask = (1 << b);
            if ((slots & bit_mask) == 0)
            {
                u32 new_slots = slots | bit_mask;
                // Try to mark the slot as used
                if (std::atomic_compare_exchange_strong(&new_segment->free_markers, &slots, new_slots))
                {
                    slot = b;
                    break;
                }
            }
        }
    }
    Span* span = static_cast<Span*>(pointer_offset(new_segment->first_span, slot * SPAN_MAX_SIZE));

    span->owner_segment = new_segment;
    // Calculate the address of the slot
    return span;
}

void _return_span_to_segment(Segment* segment, Span* span) {
    Span* first_span = segment->first_span;
    u32 slot = static_cast<u32>(pointer_diff(span, first_span) / SPAN_MAX_SIZE);

    assert(slot < SPANS_PER_SEGMENT);

    u32 slots = segment->free_markers.load();
    u32 new_slots;
    do
    {
        new_slots = slots & ~(1 << slot);
    } while (!std::atomic_compare_exchange_weak(&segment->free_markers, &slots, new_slots));

    // Check if it's completely empty and we should free it
    if (new_slots == 0)
    {
        _acquire_segments_lock_write();

        // Find the segment and delete it (if still empty)
        Segment* prev = nullptr;
        Segment* head = static_cast<Segment*>(_segments_head.load());
        while (head && head != segment)
        {
            prev = head;
            head = static_cast<Segment*>(head->next_segment.load());
        }

        if (head)
        {
            slots = head->free_markers.load();
            new_slots = slots & ~(1 << slot);

            // Still empty?
            if (new_slots == 0)
            {
                assert(head == segment);

                // It's the head
                if (!prev)
                {
                    _segments_head = head->next_segment.load();
                }
                else
                {
                    prev->next_segment = head->next_segment.load();
                }
            }
            else
            {
                head = nullptr;
            }
        }

        _release_segments_lock_write();

        // Now free if everything went ok - the segment has been unlinked
        if (head)
        {
#if ENABLE_STATISTICS
            _mapped_pages -= QUICK_ALLOCATION_PAGES_COUNT;
            _unmapped_total -= QUICK_ALLOCATION_PAGES_COUNT;
#endif
            _memory_deallocate_external(head);
        }
    }
}

//! Map new pages to virtual memory
static Span*
_memory_map(size_t page_count) {
    Span* span = nullptr;
    if (page_count <= QUICK_ALLOCATION_PAGES_COUNT)
    {
        int should_release = 1;
        _acquire_segments_lock_read();
        // Look for a span of correct size within the cache
        Segment* global_segment = static_cast<Segment*>(_segments_head.load());
        for (;;)
        {
            if (!global_segment)
            {
                // Create a new cachable segment and put it on the cache
                size_t size = (SPAN_MAX_SIZE * SPANS_PER_SEGMENT) + sizeof(Segment) + SPAN_ADDRESS_GRANULARITY;
                Segment* segment = static_cast<Segment*>(_memory_allocate_external(size));
                new (segment) Segment ();
                span = static_cast<Span*>(pointer_offset(segment, sizeof(Segment)));
                // Align to the required size of the spans
                if ((uptr)span & (SPAN_ADDRESS_GRANULARITY - 1))
                    span = static_cast<Span*>((void*)(((uptr)span & ~((uptr)SPAN_ADDRESS_GRANULARITY - 1)) + SPAN_ADDRESS_GRANULARITY));
                span->owner_segment = segment;

                segment->first_span = span;
                // Directly use this first span
                segment->free_markers = 1;

                _release_segments_lock_read();
                should_release = 0;

                _publish_segment_on_global_list(segment);
            }
            else
            {
                span = _get_span_from_segment(global_segment);
                if (!span)
                {
                    // Try the next one
                    global_segment = static_cast<Segment*>(global_segment->next_segment.load());
                }
            }

            if (span)
            {
                break;
            }
        }

        if (should_release)
        {
            _release_segments_lock_read();
        }
    }
    // Allocate a segment of the minimal required size
    else
    {
        size_t size = page_count * MALLOC_PAGE_SIZE + sizeof(Segment) + SPAN_ADDRESS_GRANULARITY;
        Segment* segment = static_cast<Segment*>(_memory_allocate_external(size));
        span = static_cast<Span*>(pointer_offset(segment, sizeof(Segment)));
        // Align to the required size of the spans
        if ((uptr)span & (SPAN_ADDRESS_GRANULARITY - 1))
            span = static_cast<Span*>((void*)(((uptr)span & ~((uptr)SPAN_ADDRESS_GRANULARITY - 1)) + SPAN_ADDRESS_GRANULARITY));

        segment->first_span = span;
        segment->next_segment = (void*)SINGLE_SEGMENT_MARKER;

        // Allocate a segment that will not be re-used
        span->owner_segment = segment;
    }
    assert(span && span->owner_segment);
    return span;
}

//! Unmap pages from virtual memory
static void
_memory_unmap(Span* span) {
    Segment* segment = span->owner_segment;
    assert(segment);
    Segment* nn = static_cast<Segment*>(segment->next_segment.load());
    if (nn == (void*)SINGLE_SEGMENT_MARKER)
    {
        _memory_deallocate_external(segment);

#if ENABLE_STATISTICS
        _mapped_pages -= QUICK_ALLOCATION_PAGES_COUNT;
        _unmapped_total -= QUICK_ALLOCATION_PAGES_COUNT;
#endif
    }
    else
    {
        _return_span_to_segment(segment, span);
    }
}

static void*
_memory_allocate_external(size_t bytes)
{
#if ENABLE_STATISTICS
    _mapped_total += (int)(bytes / MALLOC_PAGE_SIZE);
    _mapped_pages += (int)(bytes / MALLOC_PAGE_SIZE);
#endif
    return std::malloc (bytes);
}

static void
_memory_deallocate_external(void* ptr)
{
    std::free (ptr);
}

//! Yield the thread remaining timeslice
static void
thread_yield()
{ }

void* allocate (std::size_t bytes)
{
#if ENABLE_VALIDATE_ARGS
    if (size >= MAX_ALLOC_SIZE)
    {
        errno = EINVAL;
        return 0;
    }
#endif

    return _memory_allocate (bytes);
}

void deallocate (void* ptr)
{
    _memory_deallocate (ptr);
}

void* reallocate (void* ptr, std::size_t old_size, std::size_t new_size)
{
#if ENABLE_VALIDATE_ARGS
    if (size >= MAX_ALLOC_SIZE) {
        errno = EINVAL;
        return ptr;
    }
#endif
    return _memory_reallocate(ptr, new_size, old_size, 0);
}

// Extern interface

#ifdef CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

MALLOC_CALL void*
alloc(size_t size)
{
#if ENABLE_VALIDATE_ARGS
    if (size >= MAX_ALLOC_SIZE)
    {
        errno = EINVAL;
        return 0;
    }
#endif

    return _memory_allocate(size);
}

void
free(void* ptr)
{
    _memory_deallocate(ptr);
}

MALLOC_CALL void*
calloc(size_t num, size_t size)
{
    size_t total;
#if ENABLE_VALIDATE_ARGS
#ifdef OS_WINDOWS
    int err = ::SizeTMult(num, size, &total);
    if ((err != S_OK) || (total >= MAX_ALLOC_SIZE))
    {
        errno = EINVAL;
        return 0;
    }
#else
    int err = __builtin_umull_overflow(num, size, &total);
    if (err || (total >= MAX_ALLOC_SIZE))
    {
        errno = EINVAL;
        return 0;
    }
#endif
#else
    total = num * size;
#endif
    void* ptr = _memory_allocate(total);
    std::memset(ptr, 0, total);
    return ptr;
}

void*
realloc(void* ptr, size_t size)
{
#if ENABLE_VALIDATE_ARGS
    if (size >= MAX_ALLOC_SIZE) {
        errno = EINVAL;
        return ptr;
    }
#endif
    return _memory_reallocate(ptr, size, 0, 0);
}

void*
aligned_realloc(void* ptr, size_t alignment, size_t size, size_t oldsize,
    unsigned int flags)
{
    (void)alignment; // Silence warning
#if ENABLE_VALIDATE_ARGS
    if (size + alignment < size)
    {
        errno = EINVAL;
        return 0;
    }
#endif
    //TODO: If alignment > 16, we need to copy to new aligned position
    (void)sizeof(alignment);
    return _memory_reallocate(ptr, size, oldsize, flags);
}

MALLOC_CALL void*
aligned_alloc(size_t alignment, size_t size)
{
    if (alignment <= 16)
        return alloc(size);

#if ENABLE_VALIDATE_ARGS
    if (size + alignment < size)
    {
        errno = EINVAL;
        return 0;
    }
#endif

    if (alignment >= (SPAN_MAX_SIZE - SPAN_HEADER_SIZE))
    {
        assert(false && "Alignment is too large - not supported!");
        return nullptr;
    }

    void* ptr = alloc(size + alignment);
    if ((uptr)ptr & (alignment - 1))
        ptr = (void*)(((uptr)ptr & ~((uptr)alignment - 1)) + alignment);

    return ptr;
}

size_t
alloc_usable_size(void* ptr)
{
    return ptr ? _memory_usable_size(ptr) : 0;
}

#endif // CPPUAL_ENABLE_MEMORY_MODEL_GLOBALLY

void
thread_collect(void)
{
    _memory_deallocate_deferred(GET_THREAD_LOCAL(Heap*, _memory_thread_heap), 0);
}

void
thread_statistics(ThreadStatistics* stats)
{
    new (stats) ThreadStatistics ();
    Heap* heap = GET_THREAD_LOCAL(Heap*, _memory_thread_heap);
#if ENABLE_STATISTICS
    stats->allocated = heap->allocated;
    stats->requested = heap->requested;
#endif
    void* p = heap->defer_deallocate.load();
    while (p)
    {
        void* next = *(void**)p;
        Span* span = static_cast<Span*>((void*)((uptr)p & SPAN_MASK));
        stats->deferred += _memory_size_class[span->size_class].size;
        p = next;
    }

    for (size_t isize = 0; isize < SIZE_CLASS_COUNT; ++isize)
    {
        if (heap->active_block[isize].free_count)
            stats->active += heap->active_block[isize].free_count *
                             _memory_size_class[heap->active_span[isize]->size_class].size;

        Span* cache = heap->size_cache[isize];

        while (cache)
        {
            stats->sizecache = cache->data.block.free_count * _memory_size_class[cache->size_class].size;
            cache = cache->next_span;
        }
    }

    if (heap->span_cache)
        stats->spancache = static_cast<size_t>(heap->span_cache->data.list_size) *
                           QUICK_ALLOCATION_PAGES_COUNT * MALLOC_PAGE_SIZE;
}

void
global_statistics(GlobalStatistics* stats)
{
    new (stats) GlobalStatistics ();
#if ENABLE_STATISTICS
    stats->mapped = _mapped_pages * MALLOC_PAGE_SIZE;
    stats->mapped_total = _mapped_total * MALLOC_PAGE_SIZE;
    stats->unmapped_total = _unmapped_total * MALLOC_PAGE_SIZE;
#endif
    {
        void* global_span_ptr = _memory_span_cache.load();
        while (global_span_ptr == SPAN_LIST_LOCK_TOKEN) {
            thread_yield();
            global_span_ptr = _memory_span_cache.load();
        }
        uptr global_span_count = reinterpret_cast<uptr>(global_span_ptr) & ~SPAN_MASK;
        size_t list_bytes = global_span_count * QUICK_ALLOCATION_PAGES_COUNT * MALLOC_PAGE_SIZE;
        stats->cached += list_bytes;
    }

    for (size_t iclass = 0; iclass < LARGE_CLASS_COUNT; ++iclass) {
        void* global_span_ptr = _memory_large_cache[iclass].load();
        while (global_span_ptr == SPAN_LIST_LOCK_TOKEN) {
            thread_yield();
            global_span_ptr = _memory_large_cache[iclass].load();
        }
        uptr global_span_count = reinterpret_cast<uptr>(global_span_ptr) & ~SPAN_MASK;
        size_t list_bytes = global_span_count * (iclass + 1) * SPAN_MAX_PAGE_COUNT * MALLOC_PAGE_SIZE;
        stats->cached_large += list_bytes;
    }
}

} } } // namespace Model
