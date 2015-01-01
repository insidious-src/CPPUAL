/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_DEVICES_JOYSTICK_H_
#define CPPUAL_DEVICES_JOYSTICK_H_
#ifdef __cplusplus

#include <string>
#include <cppual/types.h>
#include <cppual/gfx/coord.h>
#include <cppual/noncopyable.h>

using std::string;

namespace cppual { namespace Input {

struct Joystick;

template <typename>
class Event;

template <typename>
class Queue;

// =========================================================

template <>
class Event <Joystick>
{
public:
	typedef std::size_t size_type;

	enum class Type : unsigned char
	{
		Connect,
		Disconnect,
		Press,
		Release,
		Move,
		Trigger,
		TrackMove
	};

	struct JoyButtonData
	{
		size_type id;
		int32     button;
	};

	struct JoyAxisData
	{
		size_type id;
		u8        axis;
		int16     value;
	};

	struct JoyTriggerData
	{
		size_type id;
		u8        trigger;
		int16     threshold;
	};

	struct JoyTrackData
	{
		size_type id;
		u8        track;
		point2i   pos;
	};

	struct JoyPlugData
	{
		size_type id;
	};

private:
};

// =========================================================

template <>
class Queue <Joystick> : NonConstructible
{
public:
	typedef Event<Joystick> event_type;

	static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

struct Joystick : NonCopyableVirtual
{
	typedef std::size_t size_type;

	enum
	{
		MaxCount	= 16,
		ButtonCount = 32,
		AxisCount	=  8
	};

	enum class Axis : unsigned char
	{
		X,
		Y,
		Z,
		R,
		U,
		V,
		PovX,
		PovY
	};

	enum class Surface : unsigned char
	{
		TrackPad,
		TrackBall,
		Touch
	};

	virtual string    getModelName () const = 0;
	virtual float     getAxisPosition (Axis) = 0;
	virtual size_type getId () const = 0;
	virtual size_type getButtonCount () const = 0;
	virtual bool      isValid () const = 0;
	virtual bool      isButtonPressed (int) = 0;
	virtual bool      hasAxis (Axis) const = 0;
	virtual bool      hasSurface (Surface) const = 0;
	virtual bool      hasMotor () const = 0;
	virtual void      vibrate (ushort) noexcept = 0;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_JOYSTICK_H_
