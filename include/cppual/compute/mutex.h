/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/compute/cv.h>

#include <atomic>

namespace cppual { namespace compute {

// ====================================================

struct defer_lock { };

template <class T>
class unique_lock final
{
private:
    //T& _M_gMutex;
};

template <class T>
class shared_lock final
{
public:
    typedef std::atomic_size_t atomic_size;

private:
    atomic_size _M_uLockCount;
    //T&          _M_mutex;
};

// ====================================================

// fast mutex - doesn't do any checks
// still provides some safety but at no performance cost
class mutex final : public non_copyable
{
public:
    typedef std::atomic<thread::id> atomic_id;

    mutex ();
    bool tryLock () noexcept;
    bool tryLockFor (uint millisec);
    void lock ();
    void unlock ();

private:
    condition_variable _M_gCond;
    atomic_id          _M_gOwner;
};


} } // Concurency

#endif // __cplusplus
#endif // CPPUAL_CONCURENCY_MUTEX_H_
