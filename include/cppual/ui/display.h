/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 fymfifa
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

#ifndef CPPUAL_GFX_DISPLAY_H_
#define CPPUAL_GFX_DISPLAY_H_
#ifdef __cplusplus

#include <memory>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/dsp_details.h>

using std::shared_ptr;
using std::weak_ptr;
using std::string;
using cppual::Graphics::Connection;

namespace cppual { namespace Ui {

struct  IDisplay;
typedef shared_ptr<IDisplay> shared_display;
typedef weak_ptr  <IDisplay> weak_display;

// ====================================================

struct IDisplay : NonCopyable
{
	typedef Connection     value_type;
	typedef shared_display pointer;

	virtual string name () const = 0;
	virtual int    screenCount () const = 0;
	virtual void   flush () = 0;

	static  IDisplay* instance ();
	static  bool      hasValidInstance () noexcept;
	static  bool      set (cchar*);
	static  pointer   connect (cchar*);

	constexpr value_type native () const noexcept { return m_native; }
	constexpr value_type gl     () const noexcept { return m_gl;     }

	constexpr IDisplay () noexcept
	: m_native (), m_gl ()
	{ }

	constexpr IDisplay (value_type native, value_type gl) noexcept
	: m_native (native), m_gl (gl)
	{ }

private:
	value_type m_native, m_gl;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_DISPLAY_H_
