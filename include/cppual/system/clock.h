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

#ifndef CPPUAL_SYSTEM_CLOCK_H_
#define CPPUAL_SYSTEM_CLOCK_H_
#ifdef __cplusplus

#include <chrono>

namespace cppual { namespace Clock {

template <class TClock = std::chrono::high_resolution_clock>
class Timer
{
public:
	typedef typename TClock::time_point time_point;

	Timer () noexcept : m_gEpoch (TClock::now ()) { }
	void start () noexcept { m_gEpoch = TClock::now (); }

	template <typename T>
	T elapsed () const noexcept
	{ return std::chrono::duration_cast<T> (TClock::now () - m_gEpoch); }

private:
	time_point m_gEpoch;
};

} } // namespace Clock

#endif // __cplusplus
#endif // CPPUAL_SYSTEM_CLOCK_H_
