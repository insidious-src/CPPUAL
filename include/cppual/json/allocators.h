// Tencent is pleased to support the open source community by making RapidJSON available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef RAPIDJSON_ALLOCATORS_H_
#define RAPIDJSON_ALLOCATORS_H_

#include <cppual/memory/allocator.h>

#include "rapidjson.h"
#include "internal/meta.h"

#include <memory>

#if RAPIDJSON_HAS_CXX11
#include <type_traits>
#endif

namespace cppual { namespace json {

///////////////////////////////////////////////////////////////////////////////
// Allocator

/*! \class rapidjson::Allocator
    \brief Concept for allocating, resizing and freeing memory block.

    Note that Malloc() and Realloc() are non-static but Free() is static.

    So if an allocator need to support Free(), it needs to put its pointer in
    the header of memory block.

\code
concept Allocator {
    static const bool kNeedFree;    //!< Whether this allocator needs to call Free().

    // Allocate a memory block.
    // \param size of the memory block in bytes.
    // \returns pointer to the memory block.
    void* Malloc(size_t size);

    // Resize a memory block.
    // \param originalPtr The pointer to current memory block. Null pointer is permitted.
    // \param originalSize The current size in bytes. (Design issue: since some allocator may not book-keep this, explicitly pass to it can save memory.)
    // \param newSize the new size in bytes.
    void* Realloc(void* originalPtr, size_t originalSize, size_t newSize);

    // Free a memory block.
    // \param pointer to the memory block. Null pointer is permitted.
    static void Free(void *ptr);
};
\endcode
*/


/*! \def RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY
    \ingroup RAPIDJSON_CONFIG
    \brief User-defined kDefaultChunkCapacity definition.

    User can define this as any \c size that is a power of 2.
*/

#ifndef RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY
#define RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY (64 * 1024)
#endif

///////////////////////////////////////////////////////////////////////////////
// MemoryPoolResource

//! Default memory allocator used by the parser and DOM.
/*! This allocator allocate memory blocks from pre-allocated memory chunks.

    It does not free memory blocks. And Realloc() only allocate new memory.

    The memory chunks are allocated by BaseAllocator, which is cppual::memory::memory_resource by default.

    User may also supply a buffer as the first chunk.

    If the user-buffer is full then additional chunks are allocated by BaseAllocator.

    The user-buffer is not deallocated by this allocator.

    \tparam BaseAllocator the allocator type for allocating memory chunks. Default is cppual::memory::memory_resource.
    \note implements Allocator concept
*/
template <typename BaseResource = cppual::memory::memory_resource>
class MemoryPoolResource : public cppual::memory::memory_resource
{
    //! Chunk header for perpending to each chunk.
    /*! Chunks are stored as a singly linked list.
    */
    struct ChunkHeader {
        size_type capacity;    //!< Capacity of the chunk in bytes (excluding the header itself).
        size_type size;        //!< Current size of allocated memory in bytes.
        ChunkHeader *next;  //!< Next chunk in the linked list.
    };

    struct SharedData {
        ChunkHeader *chunkHead;  //!< Head of the chunk linked-list. Only the head chunk serves allocation.
        BaseResource* ownBaseAllocator; //!< base allocator created by this object.
        size_type refcount;
        bool ownBuffer;
    };

    static const size_type SIZEOF_SHARED_DATA = RAPIDJSON_ALIGN(sizeof(SharedData));
    static const size_type SIZEOF_CHUNK_HEADER = RAPIDJSON_ALIGN(sizeof(ChunkHeader));

    static inline ChunkHeader* GetChunkHead(SharedData* shared)
    {
        return reinterpret_cast<ChunkHeader*>
                (reinterpret_cast<uptr> (reinterpret_cast<uint8_t*>(shared) + SIZEOF_SHARED_DATA));
    }
    static inline uint8_t* GetChunkBuffer(SharedData* shared)
    {
        return reinterpret_cast<uint8_t*>(shared->chunkHead) + SIZEOF_CHUNK_HEADER;
    }

    static const size_type kDefaultChunkCapacity = RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY; //!< Default chunk capacity.

public:
    static const bool kNeedFree = false;    //!< Tell users that no need to call Free() with this allocator. (concept Allocator)
    static const bool kRefCounted = true;   //!< Tell users that this allocator is reference counted on copy

    //! Constructor with chunkSize.
    /*! \param chunkSize The size of memory chunk. The default is kDefaultChunkSize.
        \param baseAllocator The allocator for allocating memory chunks.
    */
    explicit
    MemoryPoolResource(size_type chunkSize = kDefaultChunkCapacity,
                       BaseResource* baseAllocator = &cppual::memory::get_default_resource()) :
        chunk_capacity_(chunkSize),
        baseAllocator_(baseAllocator ? baseAllocator : &cppual::memory::get_default_resource()),
        shared_(static_cast<SharedData*>(baseAllocator_ ?
                                             baseAllocator_->allocate(SIZEOF_SHARED_DATA +
                                                                      SIZEOF_CHUNK_HEADER) : nullptr))
    {
        RAPIDJSON_ASSERT(baseAllocator_ != 0);
        RAPIDJSON_ASSERT(shared_ != 0);
        if (baseAllocator) {
            shared_->ownBaseAllocator = 0;
        }
        else {
            shared_->ownBaseAllocator = baseAllocator_;
        }
        shared_->chunkHead = GetChunkHead(shared_);
        shared_->chunkHead->capacity = 0;
        shared_->chunkHead->size = 0;
        shared_->chunkHead->next = 0;
        shared_->ownBuffer = true;
        shared_->refcount = 1;
    }

    //! Constructor with user-supplied buffer.
    /*! The user buffer will be used firstly. When it is full,
        memory pool allocates new chunk with chunk size.

        The user buffer will not be deallocated when this allocator is destructed.

        \param buffer User supplied buffer.
        \param size Size of the buffer in bytes. It must at least larger than sizeof(ChunkHeader).
        \param chunkSize The size of memory chunk. The default is kDefaultChunkSize.
        \param baseAllocator The allocator for allocating memory chunks.
    */
    MemoryPoolResource(void *buffer, size_type size, size_type chunkSize = kDefaultChunkCapacity,
                       BaseResource* baseAllocator = &cppual::memory::get_default_resource()) :
        chunk_capacity_(chunkSize),
        baseAllocator_(baseAllocator),
        shared_(static_cast<SharedData*>(AlignBuffer(buffer, size)))
    {
        RAPIDJSON_ASSERT(size >= SIZEOF_SHARED_DATA + SIZEOF_CHUNK_HEADER);
        shared_->chunkHead = GetChunkHead(shared_);
        shared_->chunkHead->capacity = size - SIZEOF_SHARED_DATA - SIZEOF_CHUNK_HEADER;
        shared_->chunkHead->size = 0;
        shared_->chunkHead->next = 0;
        shared_->ownBaseAllocator = 0;
        shared_->ownBuffer = false;
        shared_->refcount = 1;
    }

    MemoryPoolResource(const MemoryPoolResource& rhs) RAPIDJSON_NOEXCEPT :
        chunk_capacity_(rhs.chunk_capacity_),
        baseAllocator_(rhs.baseAllocator_),
        shared_(rhs.shared_)
    {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        ++shared_->refcount;
    }
    MemoryPoolResource& operator=(const MemoryPoolResource& rhs) RAPIDJSON_NOEXCEPT
    {
        RAPIDJSON_NOEXCEPT_ASSERT(rhs.shared_->refcount > 0);
        ++rhs.shared_->refcount;
        this->~MemoryPoolResource();
        baseAllocator_ = rhs.baseAllocator_;
        chunk_capacity_ = rhs.chunk_capacity_;
        shared_ = rhs.shared_;
        return *this;
    }

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
    MemoryPoolResource(MemoryPoolResource&& rhs) RAPIDJSON_NOEXCEPT :
        chunk_capacity_(rhs.chunk_capacity_),
        baseAllocator_(rhs.baseAllocator_),
        shared_(rhs.shared_)
    {
        RAPIDJSON_NOEXCEPT_ASSERT(rhs.shared_->refcount > 0);
        rhs.shared_ = 0;
    }
    MemoryPoolResource& operator=(MemoryPoolResource&& rhs) RAPIDJSON_NOEXCEPT
    {
        RAPIDJSON_NOEXCEPT_ASSERT(rhs.shared_->refcount > 0);
        this->~MemoryPoolResource();
        baseAllocator_ = rhs.baseAllocator_;
        chunk_capacity_ = rhs.chunk_capacity_;
        shared_ = rhs.shared_;
        rhs.shared_ = 0;
        return *this;
    }
#endif

    //! Destructor.
    /*! This deallocates all memory chunks, excluding the user-supplied buffer.
    */
    ~MemoryPoolResource() RAPIDJSON_NOEXCEPT {
        if (!shared_) {
            // do nothing if moved
            return;
        }
        if (shared_->refcount > 1) {
            --shared_->refcount;
            return;
        }
        Clear();
        //BaseResource *a = shared_->ownBaseAllocator;
        if (shared_->ownBuffer) {
            baseAllocator_->deallocate(shared_, shared_->chunkHead->size);
        }
        //RAPIDJSON_DELETE(a);
    }

    //! Deallocates all memory chunks, excluding the first/user one.
    void Clear() RAPIDJSON_NOEXCEPT {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        for (;;) {
            ChunkHeader* c = shared_->chunkHead;
            if (!c->next) {
                break;
            }
            shared_->chunkHead = c->next;
            baseAllocator_->deallocate(c, c->size);
        }
        shared_->chunkHead->size = 0;
    }

    //! Computes the total capacity of allocated memory chunks.
    /*! \return total capacity in bytes.
    */
    size_type capacity() const RAPIDJSON_NOEXCEPT {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        size_type capacity = 0;
        for (ChunkHeader* c = shared_->chunkHead; c != 0; c = c->next)
            capacity += c->capacity;
        return capacity;
    }

    //! Computes the memory blocks allocated.
    /*! \return total used bytes.
    */
    size_type Size() const RAPIDJSON_NOEXCEPT {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        size_type size = 0;
        for (ChunkHeader* c = shared_->chunkHead; c != 0; c = c->next)
            size += c->size;
        return size;
    }

    //! Whether the allocator is shared.
    /*! \return true or false.
    */
    bool Shared() const RAPIDJSON_NOEXCEPT {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        return shared_->refcount > 1;
    }

private:
    //! Allocates a memory block. (concept Allocator)
    void* do_allocate(size_type size, size_type /*align*/)
    {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        if (!size)
            return NULL;

        size = RAPIDJSON_ALIGN(size);
        if (RAPIDJSON_UNLIKELY(shared_->chunkHead->size + size > shared_->chunkHead->capacity))
            if (!AddChunk(chunk_capacity_ > size ? chunk_capacity_ : size))
                return NULL;

        void *buffer = GetChunkBuffer(shared_) + shared_->chunkHead->size;
        shared_->chunkHead->size += size;
        return buffer;
    }

    //! Resizes a memory block (concept Allocator)
    void* do_reallocate(void* originalPtr, size_type originalSize, size_type newSize, size_type /*align*/)
    {
        if (originalPtr == 0)
            return allocate(newSize);

        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        if (newSize == 0)
            return NULL;

        originalSize = RAPIDJSON_ALIGN(originalSize);
        newSize = RAPIDJSON_ALIGN(newSize);

        // Do not shrink if new size is smaller than original
        if (originalSize >= newSize)
            return originalPtr;

        // Simply expand it if it is the last allocation and there is sufficient space
        if (originalPtr == GetChunkBuffer(shared_) + shared_->chunkHead->size - originalSize) {
            size_type increment = static_cast<size_type>(newSize - originalSize);
            if (shared_->chunkHead->size + increment <= shared_->chunkHead->capacity) {
                shared_->chunkHead->size += increment;
                return originalPtr;
            }
        }

        // Realloc process: allocate and copy memory, do not free original buffer.
        if (void* newBuffer = allocate(newSize)) {
            if (originalSize)
                std::memcpy(newBuffer, originalPtr, originalSize);
            return newBuffer;
        }
        else
            return NULL;
    }

    //! Frees a memory block (concept Allocator)
    void do_deallocate(void* /*ptr*/, size_type /*bytes*/, size_type /*align*/) RAPIDJSON_NOEXCEPT
    { } // Do nothing

    bool do_is_equal(base_type const& other) const noexcept
    { return this == &other; }

public:
    //! Compare (equality) with another MemoryPoolResource
    bool operator==(const MemoryPoolResource& rhs) const RAPIDJSON_NOEXCEPT {
        RAPIDJSON_NOEXCEPT_ASSERT(shared_->refcount > 0);
        RAPIDJSON_NOEXCEPT_ASSERT(rhs.shared_->refcount > 0);
        return shared_ == rhs.shared_;
    }
    //! Compare (inequality) with another MemoryPoolResource
    bool operator!=(const MemoryPoolResource& rhs) const RAPIDJSON_NOEXCEPT {
        return !operator==(rhs);
    }

private:
    //! Creates a new chunk.
    /*! \param capacity Capacity of the chunk in bytes.
        \return true if success.
    */
    bool AddChunk(size_type capacity) {
        if (!baseAllocator_)
            shared_->ownBaseAllocator = baseAllocator_ = &cppual::memory::get_default_resource();
        if (ChunkHeader* chunk = static_cast<ChunkHeader*>(baseAllocator_->allocate
                                                           (SIZEOF_CHUNK_HEADER + capacity)))
        {
            chunk->capacity = capacity;
            chunk->size = 0;
            chunk->next = shared_->chunkHead;
            shared_->chunkHead = chunk;
            return true;
        }
        else
            return false;
    }

    static inline void* AlignBuffer(void* buf, size_type &size)
    {
        RAPIDJSON_NOEXCEPT_ASSERT(buf != 0);
        const uintptr_t mask = sizeof(void*) - 1;
        const uintptr_t ubuf = reinterpret_cast<uintptr_t>(buf);
        if (RAPIDJSON_UNLIKELY(ubuf & mask)) {
            const uintptr_t abuf = (ubuf + mask) & ~mask;
            RAPIDJSON_ASSERT(size >= abuf - ubuf);
            buf = reinterpret_cast<void*>(abuf);
            size -= abuf - ubuf;
        }
        return buf;
    }

    size_type     chunk_capacity_ { }; //!< The minimum capacity of chunk when they are allocated.
    BaseResource* baseAllocator_  { }; //!< base allocator for allocating memory chunks.
    SharedData*   shared_         { }; //!< The shared data of the allocator
};

inline MemoryPoolResource<cppual::memory::memory_resource>* memory_pool_resource()
{
    static auto rc = MemoryPoolResource<cppual::memory::memory_resource> ();
    return &rc;
}

namespace internal {
    template<typename, typename = void>
    struct IsRefCounted :
        public FalseType
    { };
    template<typename T>
    struct IsRefCounted<T, typename internal::EnableIfCond<T::kRefCounted>::Type> :
        public TrueType
    { };
}

template<typename T, typename A>
inline T* Realloc(A& a, T* old_p, size_t old_n, size_t new_n)
{
    RAPIDJSON_NOEXCEPT_ASSERT(old_n <= SIZE_MAX / sizeof(T) && new_n <= SIZE_MAX / sizeof(T));
    return static_cast<T*>(a.reallocate(old_p, old_n * sizeof(T), new_n * sizeof(T)));
}

template<typename T, typename A>
inline T* Malloc(A& a, size_t n = 1)
{
    return static_cast<T*>(a.allocate(n * sizeof(T)));
}

template<typename T, typename A>
inline void Free(A& a, T* p, size_t n = 1)
{
    a.deallocate(p, n * sizeof(T));
}

} } // namespace Json

#endif // RAPIDJSON_ENCODINGS_H_
