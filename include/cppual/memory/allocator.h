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

#ifndef CPPUAL_MEMORY_ALLOCATOR_H_
#define CPPUAL_MEMORY_ALLOCATOR_H_
#ifdef __cplusplus

#include <limits>
#include <memory>
#include <cppual/types.h>

namespace cppual { namespace Memory {

struct Allocator
{
	typedef std::size_t     align_type;
	typedef std::size_t	    size_type;
	typedef size_type const const_size;
	typedef u8*             math_pointer;
	typedef void*           pointer;
	typedef cvoid*          const_pointer;
	typedef std::ptrdiff_t  difference_type;

	virtual size_type count          () const { return 0; }
	virtual void      clear          () { }
	virtual bool      is_thread_safe () const noexcept { return false;  }
	virtual bool      is_lock_free   () const noexcept { return true;   }
	virtual bool      is_shared      () const noexcept { return false;  }

	virtual size_type max_size () const
	{ return std::numeric_limits<size_type>::max (); }

	virtual size_type size () const
	{ return std::numeric_limits<size_type>::max (); }

	virtual bool is_equal (Allocator const& gObj) const
	{ return std::numeric_limits<size_type>::max () == gObj.size (); }

	virtual void* allocate (size_type size, align_type)
	{ return ::operator new (size); }

	virtual void deallocate (void* pointer, size_type)
	{ ::operator delete (pointer); }

	virtual Allocator& owner () const noexcept
	{ return const_cast<Allocator&> (*this); }

	static Allocator& new_allocator () noexcept
	{
		static Allocator alloc;
		return alloc;
	}
};

// =========================================================

template <typename T, class Ator = void>
class AllocatorPolicy
{
public:
	typedef T			    value_type;
	typedef T*			    pointer;
	typedef T const*	    const_pointer;
	typedef T&			    reference;
	typedef T const&	    const_reference;
	typedef Ator            allocator_type;
	typedef std::size_t	    size_type;
	typedef std::ptrdiff_t  difference_type;
	typedef std::false_type propagate_on_container_copy_assignment;
	typedef std::true_type  propagate_on_container_move_assignment;
	typedef std::true_type  propagate_on_container_swap;

	AllocatorPolicy () = delete;
	inline    AllocatorPolicy (AllocatorPolicy&&) noexcept = default;
	constexpr AllocatorPolicy (AllocatorPolicy const&) noexcept = default;
	inline    AllocatorPolicy& operator = (AllocatorPolicy&&) noexcept = default;
	inline    AllocatorPolicy& operator = (AllocatorPolicy const&) noexcept = default;

	template <class U>
	struct rebind { typedef AllocatorPolicy<U, allocator_type> other; };

	void deallocate (pointer p, size_type n)
	{ get ().deallocate (p, n * sizeof (T)); }

	pointer allocate (size_type n, cvoid* = nullptr)
	{ return static_cast<pointer> (get ().allocate (sizeof (T) * n, alignof (T))); }

	size_type max_size () const noexcept
	{ return get ().max_size () / sizeof (T); }

	constexpr static pointer address (reference x) noexcept
	{ return std::addressof (x); }

	constexpr static const_pointer address (const_reference x) noexcept
	{ return std::addressof (x); }

	template <class U, typename... Args>
	static void construct (U* p, Args&&... args)
	{ new (p) U (std::forward<Args> (args)...); }

	template <class U>
	static void destroy (U* p)
	{ p->~U (); }

	constexpr allocator_type& get () const noexcept
	{ return *m_pAtor; }

	constexpr AllocatorPolicy (allocator_type& gAtor) noexcept
	: m_pAtor (&gAtor)
	{ }

	template <class U>
	constexpr
	explicit
	AllocatorPolicy (AllocatorPolicy<U, allocator_type> const& gObj) noexcept
	: m_pAtor (gObj.m_pAtor)
	{ }

	template <typename, class>
	friend class AllocatorPolicy;

	template <class T1, class T2>
	friend
	bool
	operator == (AllocatorPolicy<T1, Allocator> const&,
				 AllocatorPolicy<T2, Allocator> const&) noexcept;

private:
	allocator_type* m_pAtor;
};

// =========================================================

template <typename T>
class AllocatorPolicy <T, Allocator>
{
public:
	typedef T			    value_type;
	typedef T*			    pointer;
	typedef T const*	    const_pointer;
	typedef T&			    reference;
	typedef T const&	    const_reference;
	typedef Allocator       allocator_type;
	typedef std::size_t	    size_type;
	typedef std::ptrdiff_t  difference_type;
	typedef std::false_type propagate_on_container_copy_assignment;
	typedef std::true_type  propagate_on_container_move_assignment;
	typedef std::true_type  propagate_on_container_swap;

	inline    AllocatorPolicy (AllocatorPolicy&&) noexcept = default;
	constexpr AllocatorPolicy (AllocatorPolicy const&) noexcept = default;
	inline    AllocatorPolicy& operator = (AllocatorPolicy&&) noexcept = default;
	inline    AllocatorPolicy& operator = (AllocatorPolicy const&) noexcept = default;

	template <class U>
	struct rebind { typedef AllocatorPolicy<U, allocator_type> other; };

	void deallocate (pointer p, size_type n)
	{ get ().deallocate (p, n * sizeof (T)); }

	pointer allocate (size_type n, cvoid* = nullptr)
	{ return static_cast<pointer> (get ().allocate (sizeof (T) * n, alignof (T))); }

	size_type max_size () const noexcept
	{ return get ().max_size () / sizeof (T); }

	static pointer address (reference x) noexcept
	{ return std::addressof (x); }

	static const_pointer address (const_reference x) noexcept
	{ return std::addressof (x); }

	template<class U, typename... Args>
	static void construct (U* p, Args&&... args)
	{ new (p) U (std::forward<Args> (args)...); }

	template <class U>
	static void destroy (U* p)
	{ p->~U (); }

	constexpr allocator_type& get () const noexcept
	{ return *m_pAtor; }

	constexpr AllocatorPolicy (allocator_type& gAtor) noexcept
	: m_pAtor (&gAtor)
	{ }

	AllocatorPolicy () noexcept
	: m_pAtor (&Allocator::new_allocator ())
	{ }

	template <class U>
	constexpr
	explicit
	AllocatorPolicy (AllocatorPolicy<U, allocator_type> const& gObj) noexcept
	: m_pAtor (gObj.m_pAtor)
	{ }

	template <typename, class>
	friend class AllocatorPolicy;

	template <class T1, class T2>
	friend
	bool
	operator == (AllocatorPolicy<T1, Allocator> const&,
				 AllocatorPolicy<T2, Allocator> const&) noexcept;

private:
	allocator_type* m_pAtor;
};

// =========================================================

template <typename T>
struct is_allocator_helper : public std::false_type
{ };

template <typename T>
struct is_allocator_helper < std::allocator<T> > : public std::true_type
{ };

template <typename T, typename U>
struct is_allocator_helper < AllocatorPolicy <T, U> > : public std::true_type
{ };

template <typename T>
struct is_allocator : public is_allocator_helper<T>
{ };

// =========================================================

template <class T>
struct AllocatorPolicy <T, void> : std::allocator<T>
{ using std::allocator<T>::allocator; };

template <typename T>
using GenericPolicy = AllocatorPolicy<T, Allocator>;

template <typename T>
using AllocatorType = typename
std::enable_if<is_allocator<T>::value, T>::type;

// =========================================================

template <class T, class U, class Allocator>
inline
bool
operator == (AllocatorPolicy<T, Allocator> const& x,
			 AllocatorPolicy<U, Allocator> const& y) noexcept
{ return x.m_pAtor == y.m_pAtor; }

template <class T, class U, class Allocator>
inline
bool
operator != (AllocatorPolicy<T, Allocator> const& x,
			 AllocatorPolicy<U, Allocator> const& y) noexcept
{ return !(x == y); }

template <class T, class Allocator, class RAllocator>
constexpr
bool
operator != (AllocatorPolicy<T, Allocator> const&,
			 RAllocator const&) noexcept
{ return true; }

template <class T, class Allocator, class RAllocator>
constexpr
bool
operator == (AllocatorPolicy<T, Allocator> const&,
			 RAllocator const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator != (AllocatorPolicy<T, void> const&,
			 std::allocator<U> const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator == (AllocatorPolicy<T, void> const&,
			 std::allocator<U> const&) noexcept
{ return true; }

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
