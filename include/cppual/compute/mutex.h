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

#ifndef CPPUAL_CONCURENCY_MUTEX_H_
#define CPPUAL_CONCURENCY_MUTEX_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/compute/cv.h>


namespace cppual { namespace Compute {

// ====================================================

struct DeferLock { };

template <class T>
class UniqueLock final
{
private:
	//T& m_gMutex;
};

template <class T>
class SharedLock final
{
public:
	typedef std::atomic_size_t atomic_size;

private:
	atomic_size m_uLockCount;
	//T&          m_mutex;
};

// ====================================================

// fast mutex - doesn't do any checks
// still provides some safety but at no performance cost
class Mutex final : public NonCopyable
{
public:
	typedef std::atomic<Thread::Id> atomic_id;

	Mutex ();
	bool tryLock () noexcept;
	bool tryLockFor (uint millisec);
	void lock ();
	void unlock ();

private:
	ConditionVariable m_gCond;
	atomic_id         m_gOwner;
};


} } // Concurency

#endif // __cplusplus
#endif // CPPUAL_CONCURENCY_MUTEX_H_
